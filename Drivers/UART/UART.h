/*
 * UART.h
 *
 *  Created on: Jul 16, 2024
 *      Author: devink
 */

#ifndef UART_UART_H_
#define UART_UART_H_

#include "main.h"
#include "Middlewares/Scheduler/Scheduler.h"
#include <stdbool.h>
#include <stdint.h>

typedef struct
{
	/// Holds the pointer to the STM32 uart handle that was passed in Init_DMA_UART()
	UART_HandleTypeDef * UART_Handle;

	uint8_t * TX_Buffer;
	volatile uint32_t TX_Buffer_Head;
	volatile uint32_t TX_Buffer_Tail;
	uint32_t TX_Buffer_Size;
	volatile bool Transmitting;

	uint8_t * RX_Buffer;
	uint32_t RX_Buffer_Tail;
	uint32_t RX_Buffer_Size;

	void (*TX_Complete_Callback)(void * Callback_Data);
	void * Callback_Data;

	uint32_t UART_Task_ID;
}UART;

void Init_DMA_UART(UART * uart, UART_HandleTypeDef * UART_Handle, uint8_t * TX_Buff, uint32_t TX_Buff_Size, uint8_t * RX_Buff, uint32_t RX_Buff_Size);
void Modify_UART_Baudrate(UART * uart, uint32_t New_Baudrate);

int32_t UART_Transmit(UART * uart, uint8_t * Data, uint16_t Data_Size);
int32_t UART_Transmit_With_Callback(UART * uart, uint8_t * Data, uint16_t Data_Size, void * Callback, void * Callback_Data);
int32_t UART_Transmit_Blocking(UART * uart, uint8_t * Data, uint16_t Data_Size);

int32_t UART_Receive(UART * uart, uint8_t * Data, uint16_t * Data_Size);

#endif /* UART_UART_H_ */
