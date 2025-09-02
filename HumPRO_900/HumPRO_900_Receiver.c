/*
 * HumPRO_900_Receiver.c
 *
 *  Created on: Jun 19, 2025
 *      Author: jason.p
 */

#include "HumPRO_900_Receiver.h"
#include "Middlewares/Scheduler/Scheduler.h"
#include <string.h>
#include <stdlib.h>

#define UNECRYPTED_PACKET_HEADER_TAG	0x01
#define UNECRYPTED_PACKET_DATA_TAG		0x02
#define ECRYPTED_PACKET_HEADER_TAG		0x11
#define ECRYPTED_PACKET_DATA_TAG		0x12

#define TICK_PERIOD_MS       10    // Scheduler tick period



void HumPRO_900_Receiver_Task(void * task_input);
static void HumPRO_900_Process_RX_Data(HumPRO_900_Receiver_t * Receiver);
static void HumPRO_900_Request_Data_TX_Complete(HumPRO_900_Receiver_t * Receiver);

void Init_HumPRO_900_Receiver(HumPRO_900_Receiver_t * Receiver, UART * Bus, GPIO * CMD, GPIO * EX)
{
	Receiver->Bus = Bus;
	Receiver->CMD = CMD;
	Receiver->EX = EX;
	/* initialize buffers and flags to safe defaults */
	Receiver->DMA_Buff_Size = 0;
	Receiver->RX_Buff_Size = 0;
	Receiver->Receiving_Command = true;
	Receiver->Receiving_Header = true;
	Receiver->Receiving_Data = false;
	Receiver->Expected_Return = eExpecting_Nothing;
	Receiver->RX_Complete_Callback = NULL;
	Receiver->Callback_Data = NULL;
	Receiver->RX_Que_Message[0] = Receiver->RX_Que_Message[1] = Receiver->RX_Que_Message[2] = Receiver->RX_Que_Message[3] = 0;

	/* clear buffers (optional but safer) */
	memset(Receiver->DMA_Buff, 0, sizeof(Receiver->DMA_Buff));
	memset(Receiver->RX_Buff, 0, sizeof(Receiver->RX_Buff));

	Prep_Queue(&Receiver->RX_Queue);

	Receiver->TaskID = Start_Task(HumPRO_900_Receiver_Task, Receiver, 1);
	Set_Task_Name(Receiver->TaskID, "HumPRO-900-RX");
}

void HumPRO_900_Receiver_Task(void * task_input)
{
	HumPRO_900_Receiver_t * Receiver = (HumPRO_900_Receiver_t *)task_input;

	// Set buffer size before calling UART_Receive (input/output parameter)
	Receiver->DMA_Buff_Size = sizeof(Receiver->DMA_Buff);
	UART_Receive(Receiver->Bus, Receiver->DMA_Buff, &Receiver->DMA_Buff_Size);

	if(Receiver->DMA_Buff_Size)
	{
		HumPRO_900_Process_RX_Data(Receiver);
		// Reset for next iteration
		Receiver->DMA_Buff_Size = 0;
	}
	Modify_Task_Timeout(Receiver->TaskID, TICK_PERIOD_MS);
}

void HumPRO_900_Write_Register(HumPRO_900_Receiver_t * Receiver, uint8_t Register, uint8_t Value, bool Request_ACK)
{
	uint8_t buff[6] = {0xFF};

	// A different action is needed if either the Register or the Value is >= 0x80
	if((Register & 0x80) && (Value & 0x80))
	{
		buff[1] = 0x04;
		buff[2] = 0xFE;
		buff[3] = Register - 0x80;
		buff[4] = 0xFE;
		buff[5] = Value - 0x80;

		Set_GPIO_State_Low(Receiver->CMD);
		UART_Transmit(Receiver->Bus, buff, 6);
	}
	else if(Register & 0x80)
	{
		buff[1] = 0x03;
		buff[2] = 0xFE;
		buff[3] = Register - 0x80;
		buff[4] = Value;

		Set_GPIO_State_Low(Receiver->CMD);
		UART_Transmit(Receiver->Bus, buff, 5);
	}
	else if(Value & 0x80)
	{
		buff[1] = 0x03;
		buff[2] = Register;
		buff[3] = 0xFE;
		buff[4] = Value - 0x80;

		Set_GPIO_State_Low(Receiver->CMD);
		UART_Transmit(Receiver->Bus, buff, 5);
	}
	else
	{
		buff[1] = 0x02;
		buff[2] = Register;
		buff[3] = Value;

		Set_GPIO_State_Low(Receiver->CMD);
		UART_Transmit(Receiver->Bus, buff, 4);
	}

	if(Request_ACK)
		Receiver->Expected_Return = eExpecting_ACK_NACK;
}

void HumPRO_900_Read_Register(HumPRO_900_Receiver_t * Receiver, uint8_t Register)
{
	uint8_t buff[5] = {0xFF};

	if(Register & 0x80)
	{
		buff[1] = 0x03;
		buff[2] = 0xFE;
		buff[3] = 0xFE;
		buff[4] = Register - 0x80;

		Set_GPIO_State_Low(Receiver->CMD);
		UART_Transmit(Receiver->Bus, buff, 5);
	}
	else
	{
		buff[1] = 0x02;
		buff[2] = 0xFE;
		buff[3] = Register;

		Set_GPIO_State_Low(Receiver->CMD);
		UART_Transmit(Receiver->Bus, buff, 4);
	}

	Receiver->Expected_Return = eExpecting_Register_Return;
}

void HumPRO_900_Read_Received_Data(HumPRO_900_Receiver_t * Receiver, void * RX_Callback, void * Callback_Data)
{
	uint8_t buff[5] = {0xFF, 0x03, 0xFE, 0x47, 0x04};
	Set_GPIO_State_Low(Receiver->CMD);
	UART_Transmit_Blocking(Receiver->Bus, buff, 5);

//	UART_Transmit_With_Callback(Receiver->Bus, buff, 5, HumPRO_900_Request_Data_TX_Complete, (void *)Receiver);
//	HumPRO_900_Write_Register(Receiver, 0xC7, 0x04, false);
	Receiver->Expected_Return = eExpecting_Received_Data;
	Receiver->Receiving_Header = true;
	Receiver->Receiving_Data = true;
	Modify_Task_Timeout(Receiver->TaskID, TICK_PERIOD_MS);
	Set_GPIO_State_High(Receiver->CMD);

		// Store callback info for when data reception completes
	Receiver->RX_Complete_Callback = RX_Callback;
	Receiver->Callback_Data = Callback_Data;
	
	// Don't block - let receiver task handle data parsing and callback when ready
//	TX_THREAD_SLEEP(1);
//	SET_GPIO_State_High(Receiver->CMD);
}

static void HumPRO_900_Process_RX_Data(HumPRO_900_Receiver_t * Receiver)
{
	// Ignore data if we are not expecting any
	if(Receiver->Expected_Return == eExpecting_Nothing)
		return;

	for(int i = 0; i < Receiver->DMA_Buff_Size; i++)
	{
		// Handle the data differently if we are receiving a command response vs. receiving data from the network
		if(Receiver->Receiving_Command)
		{
			if(Receiver->RX_Buff_Size == 0)
			{
				// This needs to be either a 0x06(ACK) or 0x15(NACK)
				if(Receiver->DMA_Buff[i] == 0x06 || Receiver->DMA_Buff[i] == 0x15)
				{
					if(Receiver->RX_Buff_Size < sizeof(Receiver->RX_Buff))
						Receiver->RX_Buff[Receiver->RX_Buff_Size++] = Receiver->DMA_Buff[i];

					// This can either be a ACK/NACK or can be a response to a register request
					if(Receiver->Expected_Return == eExpecting_ACK_NACK)
					{
						/* We have received the complete return. Allocate a small message and enqueue; if enqueue fails, free. */
						uint8_t * message = malloc(4);
						if(message)
						{
							message[0] = Receiver->RX_Buff_Size;
							message[1] = Receiver->RX_Buff[0];
							if(!Enqueue(&Receiver->RX_Queue, message))
							{
								/* Enqueue failed — free to avoid leak */
								free(message);
							}
						}
						Receiver->RX_Buff_Size = 0;
					}
					else if(Receiver->Expected_Return == eExpecting_Received_Data)
					{
						Receiver->Receiving_Command = false;
					}
				}
			}
			else
			{
				// Save the data until we have the three bytes total
				if(Receiver->RX_Buff_Size < sizeof(Receiver->RX_Buff))
					Receiver->RX_Buff[Receiver->RX_Buff_Size++] = Receiver->DMA_Buff[i];

				if(Receiver->Expected_Return == eExpecting_Register_Return && Receiver->RX_Buff_Size >= 3)
				{
					/* We have the full command response. Allocate and enqueue; free on failure. */
					uint8_t * message = malloc(4);
					if(message)
					{
						message[0] = Receiver->RX_Buff_Size;
						// Bounds check: only copy max 3 bytes to avoid buffer overflow
						uint8_t copy_size = (Receiver->RX_Buff_Size > 3) ? 3 : Receiver->RX_Buff_Size;
						memcpy(&message[1], Receiver->RX_Buff, copy_size);
						if(!Enqueue(&Receiver->RX_Queue, message))
						{
							free(message);
						}
					}
					Receiver->RX_Buff_Size = 0;
				}
			}
		}
		else
		{
			// Save the data until we have the three bytes total
			if(Receiver->RX_Buff_Size < sizeof(Receiver->RX_Buff))
				Receiver->RX_Buff[Receiver->RX_Buff_Size++] = Receiver->DMA_Buff[i];

			if(Receiver->Receiving_Header)
			{
				// In this receiver we always request the packet header and packet data to be transmitted together, so this handler needs to handle both packet types
				// We need at least 3 bytes received before we can make any decisions
				if(Receiver->RX_Buff_Size >= 3)
				{
					// Byte 2 will tell us the length of the packet minus the first 2 bytes
					uint8_t * Packet_Size = &Receiver->RX_Buff[2];

					// Wait until we have received all of our data, we add 3 to account for the start ACK(0x06) and the first to packet bytes
					if(Receiver->RX_Buff_Size == (*Packet_Size + 3))
					{
						// We have received all the data for this header
						Receiver->Receiving_Header = false;
					}
				}
			}
			else
			{
				// We are receiving the data portion of this packet

				// Byte 2 will tell us the length of the packet header minus the first 2 bytes and the ACK(0x06)
				uint8_t Header_Size = Receiver->RX_Buff[2] + 3;

				// Wait until we have received the header portion of the data section
				if(Receiver->RX_Buff_Size >= Header_Size + 2)
				{
					uint8_t Data_Size = Receiver->RX_Buff[Header_Size + 1] + 2;
					// We now know how much data is being passed in the data portion of the packet
					uint8_t Packet_Size = Header_Size + Data_Size;

					// Wait until we have received Data_Size worth of data
					if(Receiver->RX_Buff_Size >= Packet_Size)
					{
						// We have received the entire packet - call callback if set
						if(Receiver->RX_Complete_Callback)
						{
							uint8_t Header_Size = Receiver->RX_Buff[2] + 3;
							uint8_t Data_Size = Receiver->RX_Buff[Header_Size + 1] + 2;
							Receiver->RX_Complete_Callback(Receiver->Callback_Data, &Receiver->RX_Buff[16], &Receiver->RX_Buff[23], Data_Size - 2);
							Receiver->RX_Complete_Callback = NULL;
							Receiver->Callback_Data = NULL;
						}

						Receiver->Expected_Return = eExpecting_Nothing;
						Receiver->Receiving_Command = true;
						Receiver->Receiving_Data = false;
						Receiver->RX_Buff_Size = 0;
					}
				}
			}
		}
	}
}

static void HumPRO_900_Request_Data_TX_Complete(HumPRO_900_Receiver_t * Receiver)
{
	Set_GPIO_State_High(Receiver->CMD);
}
