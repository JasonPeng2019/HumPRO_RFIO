/*
 * HumPRO_900_Receiver.h
 *
 *  Created on: Jun 19, 2025
 *      Author: jason.p
 */

#ifndef HUMPRO_900_HUMPRO_900_RECEIVER_H_
#define HUMPRO_900_HUMPRO_900_RECEIVER_H_

#include "UART/UART.h"
#include "GPIO/GPIO.h"
#include "Middlewares/Queue/Queue.h"
#include <stdint.h>
#include <stdbool.h>

// Using HumPRO_Result from HumPRO_900.h instead of duplicate typedef

typedef enum
{
	eExpecting_Nothing = 0,
	eExpecting_ACK_NACK,
	eExpecting_Register_Return,
	eExpecting_Received_Data
}HumPRO_900_Expected_Returns_e;

typedef struct
{
	UART * Bus;
	GPIO * CMD;
	GPIO * EX;

	uint8_t DMA_Buff[256];
	uint16_t DMA_Buff_Size;

	uint8_t RX_Buff[256];
	uint8_t RX_Buff_Size;

	HumPRO_900_Expected_Returns_e Expected_Return;

	bool Receiving_Command;
	bool Receiving_Header;
	volatile bool Receiving_Data;

	Queue RX_Queue;
	uint8_t RX_Que_Message[4];

	uint32_t TaskID;

	void (*RX_Complete_Callback)(void * Callback_Data, uint8_t * Address, uint8_t * Data, uint8_t Data_Size);
	void * Callback_Data;
}HumPRO_900_Receiver_t;

void Init_HumPRO_900_Receiver(HumPRO_900_Receiver_t * Receiver, UART * Bus, GPIO * CMD, GPIO * EX);

void HumPRO_900_Write_Register(HumPRO_900_Receiver_t * Receiver, uint8_t Register, uint8_t Value, bool Request_ACK);
void HumPRO_900_Read_Register(HumPRO_900_Receiver_t * Receiver, uint8_t Register);
void HumPRO_900_Read_Received_Data(HumPRO_900_Receiver_t * Receiver, void * RX_Callback, void * Callback_Data);

// CALLBACKS
void HumPRO_900_Received_Data(void);

#endif /* HUMPRO_900_HUMPRO_900_RECEIVER_H_ */
