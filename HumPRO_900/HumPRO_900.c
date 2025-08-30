/*
 * HumPRO_900.c
 *
 *  Created on: Jul 16, 2024
 *      Author: devink
 */

#include "HumPRO_900.h"
#include <stdio.h>

#define TICK_PERIOD_MS       10    // Scheduler tick period
#define BOOT_DELAY_TICKS     100   // Boot delay in ticks
#define BOOT_DELAY_MS        (BOOT_DELAY_TICKS * TICK_PERIOD_MS)

#define PKOPT_ADDRESS			0xD3
#define PKOPT_VALUE				0x04//0x07
#define ADDMODE_ADDRESS			0x4F
#define ADDMODE_VALUE			0x07//(0x07 + 0x08)
#define EX_ADDRESS				0xD1
#define EX_VALUE				0x02

#define AMBIENT_RSSI_ADDRESS	0x7C

void HumPRO_900_Task(void * thread_input);
void HumPRO_900_RX_Complete(HumPRO_900_t * Device, uint8_t * Source_Address, uint8_t * Data, uint8_t Data_Size);
void HumPRO_900_TX_Complete(HumPRO_900_t * Device);

void Init_HumPRO_900(HumPRO_900_t * Device, UART * UART_Bus, GPIO * Power_Down_GPIO, GPIO * Reset_GPIO,
		GPIO * CMD_GPIO, GPIO * BE_GPIO, GPIO * EX_GPIO, GPIO * CRESP_GPIO)
{
	Device->BE_GPIO = BE_GPIO;
	Device->CMD_GPIO = CMD_GPIO;
	Device->CRESP_GPIO = CRESP_GPIO;
	Device->EX_GPIO = EX_GPIO;
	Device->Power_Down_GPIO = Power_Down_GPIO;
	Device->Reset_GPIO = Reset_GPIO;
	Device->UART_Bus = UART_Bus;
	Device->Current_State = eHumPRO_Booting;
	Device->TX_Buff_Size = 0;
	Device->TX_Ready = false;
	Device->TX_Complete = false;

	Set_GPIO_State_High(Reset_GPIO);
	Set_GPIO_State_High(Power_Down_GPIO);

	Prep_Queue(&Device->Receiver.RX_Queue);

	Init_HumPRO_900_Receiver(&Device->Receiver, UART_Bus, CMD_GPIO, EX_GPIO); // changes to expecting_data state

	Device->TaskID = Start_Task(HumPRO_900_Task, Device, BOOT_DELAY_MS);
	Set_Task_Name(Device->TaskID, "HumPRO-900");
}

void HumPRO_900_Transmit(HumPRO_900_t * Device, uint8_t Address[4], uint8_t * Data, uint8_t Data_Size)
{
	if(!Device->TX_Ready)
	{
		// Bounds check to prevent buffer overflow
		if(Data_Size > sizeof(Device->TX_Buff))
			Data_Size = sizeof(Device->TX_Buff);
			
		memcpy(Device->TX_Buff, Data, Data_Size);
		memcpy(Device->TX_Address, Address, 4);
		Device->TX_Buff_Size = Data_Size;
		Device->TX_Ready = true;
	}
}

void HumPRO_900_Task(void * thread_input)
{
	HumPRO_900_t * Device = (HumPRO_900_t *)thread_input;
	uint8_t message[4];
	uint32_t message_timeout = BOOT_DELAY_MS;
	HumPRO_Result result;

	uint8_t * queued = (uint8_t*)Dequeue(&Device->Receiver.RX_Queue);
	if (queued) {
		memcpy(message, queued, 4);
		free(queued);
		result = HUMPRO_SUCCESS;
	} else {
		result = HUMPRO_FAIL;
	}

	switch(Device->Current_State)
	{
	case eHumPRO_Booting:

		message_timeout = 10*TICK_PERIOD_MS;
		Device->Current_State = eHumPRO_Config_BAUD;

		break;
	case eHumPRO_Config_BAUD:

		if(result == HUMPRO_SUCCESS && message[1] == 0x06)
		{
			// We have received a response
			Device->Current_State = eHumPRO_Configuring_PKTOPT;
			Modify_UART_Baudrate(Device->UART_Bus, 115200);
		}
		else
			HumPRO_900_Write_Register(&Device->Receiver, 0x4E, 0x05, true);

		break;
	case eHumPRO_Configuring_PKTOPT:

		if(result == HUMPRO_SUCCESS && message[1] == 0x06)
		{
			// We have received a response
			Device->Current_State = eHumPRO_Configuring_ADDMODE;
		}
		else
			HumPRO_900_Write_Register(&Device->Receiver, PKOPT_ADDRESS, PKOPT_VALUE, true);


		break;
	case eHumPRO_Configuring_ADDMODE:

		if(result == HUMPRO_SUCCESS && message[1] == 0x06)
		{
			// We have received a response
			Device->Current_State = eHumPRO_Configuring_Exceptions;
		}
		else
			HumPRO_900_Write_Register(&Device->Receiver, ADDMODE_ADDRESS, ADDMODE_VALUE, true);

		break;
	case eHumPRO_Configuring_Exceptions:

		if(result == HUMPRO_SUCCESS && message[1] == 0x06)
		{
			// We have received a response
			Device->Current_State = eHumPRO_Setting_Device_Address;
		}
		else
			HumPRO_900_Write_Register(&Device->Receiver, EX_ADDRESS, EX_VALUE, true);

		break;
	case eHumPRO_Setting_Device_Address:

		// Read the 4 byte serial number and set that as our device unique address
		static int count = 0;

		if(result == HUMPRO_SUCCESS && message[0] == 3)
		{
			if(count < 4)  // Check bounds before writing
			{
				Device->Device_Address[count] = message[3];
				HumPRO_900_Write_Register(&Device->Receiver, 0x5E + count, message[3], true);
				count++;
			}
			
			if(count >= 4)
			{
				// We have set the entire address
				printf("Address: %02X:%02X:%02X:%02X\r\n", Device->Device_Address[0], Device->Device_Address[1],Device->Device_Address[2],Device->Device_Address[3]);
				message_timeout = 1*TICK_PERIOD_MS;
				Device->Current_State = eHumPRO_Idle;
				break;
			}
		}
		else
			HumPRO_900_Read_Register(&Device->Receiver, 0x34 + count);

		break;
	case eHumPRO_Idle:

		// Test if the EX line has gone high
		if(Read_GPIO_State(Device->EX_GPIO) == eGPIO_High)
		{
			// Issue the Read command
			Device->Current_State = eHumPRO_Read_EX_Register;
			message_timeout = 3*TICK_PERIOD_MS;
			HumPRO_900_Read_Register(&Device->Receiver, 0xCE);
//				HumPRO_900_Write_Register(&Device->Receiver, 0xC7, 0x02, false);
		}

		// Test if there is data that needs to be transmitted
		else if(Device->TX_Ready)
		{
			Set_GPIO_State_High(Device->CMD_GPIO);
//				UART_Transmit_With_Callback(Device->UART_Bus, Device->TX_Buff, Device->TX_Buff_Size, HumPRO_900_TX_Complete, (void *)Device);
			UART_Transmit_Blocking(Device->UART_Bus, Device->TX_Buff, Device->TX_Buff_Size);

			Set_GPIO_State_Low(Device->CMD_GPIO);

			Device->TX_Ready = false;
			Device->TX_Complete = false;
			Device->Current_State = eHumPRO_Idle;

//
//				Device->Current_State = eHumPRO_Transmitting_Packet;
//				Device->TX_Ready = false;
//				Device->TX_Complete = false;
			message_timeout = 1*TICK_PERIOD_MS;
		}
		else if(Device->TX_Complete)
		{
			Set_GPIO_State_Low(Device->CMD_GPIO);
			Device->TX_Complete = false;
		}

//			static uint8_t buff[7] = "Hello\r\n";
//
//			Set_GPIO_State_High(Device->CMD_GPIO);
//			UART_Transmit_With_Callback(Device->UART_Bus, buff, 7, HumPRO_900_TX_Complete, (void *)Device);

		break;
	case eHumPRO_Read_EX_Register:

		if(result == HUMPRO_SUCCESS && message[1] == 0x06)
		{
			if(message[3] & 0x02)
			{
				// We have received a response
				HumPRO_900_Read_Received_Data(&Device->Receiver, HumPRO_900_RX_Complete, (void *)Device);
				message_timeout = 7*TICK_PERIOD_MS;
				Device->Current_State = eHumPRO_Idle;
				//Device->Current_State = eHumPRO_Get_RX_Packet; //do we need this?
			}
		}
		else
			HumPRO_900_Read_Register(&Device->Receiver, 0xCE);

		break;

	case eHumPRO_Get_RX_Packet:
		if (result == HUMPRO_SUCCESS && message[1] == 0x06) {
			Device->Current_State = eHumPRO_Idle;
			message_timeout = 1*TICK_PERIOD_MS;
		} else {
			HumPRO_900_Write_Register(&Device->Receiver, 0xC7, 0x04, true);
			Set_GPIO_State_High(Device->CMD_GPIO);
		}
		break;

	case eHumPRO_Transmitting_Packet:
		break;
	}

	Modify_Task_Timeout(Device->TaskID, message_timeout);
}


void HumPRO_900_RX_Complete(HumPRO_900_t * Device, uint8_t * Source_Address, uint8_t * Data, uint8_t Data_Size)
{
	printf("Received Message:\r\n\t");

	for(int c = 0; c < 3; c++)
		printf("%02X:", Source_Address[c]);
	printf("%02X -> ", Source_Address[3]);

	for(int c = 0; c < Data_Size; c++)
		printf("%c", Data[c]);

	printf("\r\n");
}

void HumPRO_900_TX_Complete(HumPRO_900_t * Device)
{
	Device->TX_Ready = false;
	Device->TX_Complete = true;
	Device->Current_State = eHumPRO_Idle;
}


