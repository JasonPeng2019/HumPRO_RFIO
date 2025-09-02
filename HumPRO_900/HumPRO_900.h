/*
 * HumPRO_900.h
 *
 *  Created on: Aug 2, 2025
 *      Author: jason.p
 */

#ifndef HUMPRO_900_HUMPRO_900_H_
#define HUMPRO_900_HUMPRO_900_H_

#include "HumPRO_900_Receiver.h"
#include "GPIO/GPIO.h"
#include "UART/UART.h"
#include "Scheduler/Scheduler.h"
#include "Queue/Queue.h"

typedef enum {
	HUMPRO_FAIL = 0,
	HUMPRO_SUCCESS
}HumPRO_Result;

typedef enum
{
	eHumPRO_Booting = 0,
	eHumPRO_Config_BAUD,
	eHumPRO_Configuring_PKTOPT,
	eHumPRO_Configuring_ADDMODE,
	eHumPRO_Configuring_Exceptions,
	eHumPRO_Setting_Device_Address,
	eHumPRO_Idle,
	eHumPRO_Read_EX_Register,
	eHumPRO_Get_RX_Packet,
	eHumPRO_Transmitting_Packet
}HumPRO_900_States_e;

typedef struct
{
	GPIO * Power_Down_GPIO;
	GPIO * Reset_GPIO;
	GPIO * CMD_GPIO;
	GPIO * BE_GPIO;
	GPIO * EX_GPIO;
	GPIO * CRESP_GPIO;
	UART * UART_Bus;

	volatile HumPRO_900_States_e Current_State;
	uint8_t Device_Address[4];

	HumPRO_900_Receiver_t Receiver;

	uint32_t TaskID;

	uint8_t TX_Buff[256];
	uint8_t TX_Buff_Size;
	uint8_t TX_Address[4];
	bool TX_Ready;
	bool TX_Complete;

}HumPRO_900_t;

void Init_HumPRO_900(HumPRO_900_t * Device, UART * UART_Bus, GPIO * Power_Down_GPIO, GPIO * Reset_GPIO,
		GPIO * CMD_GPIO, GPIO * BE_GPIO, GPIO * EX_GPIO, GPIO * CRESP_GPIO);

void HumPRO_900_Transmit(HumPRO_900_t * Device, uint8_t Address[4], uint8_t * Data, uint8_t Data_Size);

#endif /* HUMPRO_900_HUMPRO_900_H_ */
