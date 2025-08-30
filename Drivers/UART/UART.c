/*
 * UART.c
 *
 *  Created on: Jul 16, 2024
 *      Author: devink
 */

#include "UART.h"

#ifndef NUMBER_OF_UARTS
#define NUMBER_OF_UARTS		2
#endif

static UART * UART_Callback_Handles[NUMBER_OF_UARTS] = {NULL};
static int Number_Of_UARTs = 0;

void UART_Thread(void * thread_input);

void Init_DMA_UART(UART * uart, UART_HandleTypeDef * UART_Handle, uint8_t * TX_Buff, uint32_t TX_Buff_Size, uint8_t * RX_Buff, uint32_t RX_Buff_Size)
{
	uart->UART_Handle = UART_Handle;
	uart->TX_Buffer = TX_Buff;
	uart->TX_Buffer_Size = TX_Buff_Size;
	// Initialize TX indices to a known state
	uart->TX_Buffer_Head = 0;
	uart->TX_Buffer_Tail = 0;
	uart->Transmitting = false;
	uart->RX_Buffer = RX_Buff;
	uart->RX_Buffer_Tail = 0;
	uart->RX_Buffer_Size = RX_Buff_Size;

	// Register callback handle with bounds check
	if(Number_Of_UARTs < NUMBER_OF_UARTS)
	{
		UART_Callback_Handles[Number_Of_UARTs++] = uart;
	}
	else
	{
		// Too many UARTs registered - ignore additional registrations
		// Caller may handle error; for now we avoid overflow.
	}

	// Start the receiver DMA
	HAL_UART_Receive_DMA(UART_Handle, RX_Buff, RX_Buff_Size);

	uart->UART_Task_ID = Start_Task(UART_Thread, (void*)uart, 1);
	Set_Task_Name(uart->UART_Task_ID, "UART");
}

void Modify_UART_Baudrate(UART * uart, uint32_t New_Baudrate)
{
	// Wait for all the current data to be transmitted with timeout (ms)
	uint32_t start = HAL_GetTick();
	uint32_t timeout_ms = 1000; // 1 second timeout
	while(uart->Transmitting)
	{
		if((HAL_GetTick() - start) > timeout_ms)
		{
			// timeout waiting for transmit to complete; proceed to re-init anyway
			break;
		}
		// small busy-wait here; keep short to avoid long blocking
	}

	// Change the UART init baudrate data field
	// Stop the receiver DMA
	HAL_UART_DMAStop(uart->UART_Handle);
	uart->RX_Buffer_Tail = 0;

	// Change the UART init baudrate data field
	uart->UART_Handle->Init.BaudRate = New_Baudrate;

	// Init the UART
	HAL_UART_Init(uart->UART_Handle);

	// restart the receiver DMA
	HAL_UART_Receive_DMA(uart->UART_Handle, uart->RX_Buffer, uart->RX_Buffer_Size);
}

int32_t UART_Transmit(UART * uart, uint8_t * Data, uint16_t Data_Size)
{
	// Fill the TX Buffer with the data that needs to be transmitted, return the amount of data that was able to be put in the buffer
	int32_t return_value = 0;
	for(; return_value < Data_Size; return_value++)
	{
		// Check the special case when the tail == 0, every buffer can have a different wrap around size
		if(uart->TX_Buffer_Tail == 0)
		{
			if(uart->TX_Buffer_Head != (uart->TX_Buffer_Size - 1))
			{
				// We have room to save the byte
				uart->TX_Buffer[uart->TX_Buffer_Head++] = Data[return_value];
				if(uart->TX_Buffer_Head == uart->TX_Buffer_Size)
					uart->TX_Buffer_Head = 0;

			}
			else
			{
				// There is no more room
				return return_value;
			}
		}
		else if(uart->TX_Buffer_Head != (uart->TX_Buffer_Tail - 1))
		{
			// We have room to save the byte
			uart->TX_Buffer[uart->TX_Buffer_Head++] = Data[return_value];
			if(uart->TX_Buffer_Head == uart->TX_Buffer_Size)
				uart->TX_Buffer_Head = 0;
		}
		else
		{
			// There is no more room
			return return_value;
		}
	}

	return return_value;
}

int32_t UART_Transmit_With_Callback(UART * uart, uint8_t * Data, uint16_t Data_Size, void * Callback, void * Callback_Data)
{
	int32_t return_value = 0;

	return_value = UART_Transmit(uart, Data, Data_Size);

	if(return_value)
	{
		uart->TX_Complete_Callback = Callback;
		uart->Callback_Data = Callback_Data;
	}

	return return_value;
}

int32_t UART_Transmit_Blocking(UART * uart, uint8_t * Data, uint16_t Data_Size)
{
	return HAL_UART_Transmit(uart->UART_Handle, Data, Data_Size, 100);
}

int32_t UART_Receive(UART * uart, uint8_t * Data, uint16_t * Data_Size)
{
	uint16_t max_size = *Data_Size;  // Save max buffer size
	*Data_Size = 0;

	if(uart->UART_Handle->RxState == HAL_UART_STATE_BUSY_RX)
	{
		while((uart->RX_Buffer_Tail != (uart->RX_Buffer_Size - uart->UART_Handle->hdmarx->Instance->CNDTR)) && (*Data_Size < max_size))
		{
			Data[(*Data_Size)++] = uart->RX_Buffer[uart->RX_Buffer_Tail++];

			if(uart->RX_Buffer_Tail >= uart->RX_Buffer_Size)
				uart->RX_Buffer_Tail = 0;
		}
	}

	return (int32_t)*Data_Size;
}

void UART_Thread(void * thread_input)
{
	UART * uart = (UART *)thread_input;

	// Test if we are currently not transmitting
	if(!uart->Transmitting)
	{
		// Test if there is any data that needs to be transmitted
		if(uart->TX_Buffer_Tail != uart->TX_Buffer_Head)
		{
			// Determine if the buffer has wrapped around or not
			if(uart->TX_Buffer_Tail < uart->TX_Buffer_Head)
			{
				// We can transmit everything in one go
				uart->Transmitting = true;
				HAL_UART_Transmit_DMA(uart->UART_Handle, &uart->TX_Buffer[uart->TX_Buffer_Tail], uart->TX_Buffer_Head - uart->TX_Buffer_Tail);

				uart->TX_Buffer_Tail = uart->TX_Buffer_Head;
			}
			else
			{
				// The head is behind the tail. Transmit up to the end of the buffer
				uart->Transmitting = true;
				HAL_UART_Transmit_DMA(uart->UART_Handle, &uart->TX_Buffer[uart->TX_Buffer_Tail], uart->TX_Buffer_Size - uart->TX_Buffer_Tail);

				uart->TX_Buffer_Tail = 0;
			}
		}
	}
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	// Find who the callback is for
	for(int c = 0; c < Number_Of_UARTs; c++)
	{
		if(UART_Callback_Handles[c] && UART_Callback_Handles[c]->UART_Handle == huart)
		{
			UART_Callback_Handles[c]->Transmitting = false;
			if(UART_Callback_Handles[c]->TX_Complete_Callback)
			{
				UART_Callback_Handles[c]->TX_Complete_Callback(UART_Callback_Handles[c]->Callback_Data);

				// We are done with the callback
				UART_Callback_Handles[c]->TX_Complete_Callback = NULL;
			}
			return;
		}
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	UNUSED(huart);
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
	// Find who the callback is for
	for(int c = 0; c < Number_Of_UARTs; c++)
	{
		if(UART_Callback_Handles[c] && UART_Callback_Handles[c]->UART_Handle == huart)
		{
			UART_Callback_Handles[c]->RX_Buffer_Tail = 0;
			UART_Callback_Handles[c]->Transmitting = false;
			HAL_DMA_Abort_IT(huart->hdmarx);
			HAL_UART_DMAStop(huart);
			HAL_UART_Receive_DMA(huart, UART_Callback_Handles[c]->RX_Buffer, UART_Callback_Handles[c]->RX_Buffer_Size);
			return;
		}
	}
}
