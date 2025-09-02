/*
 * rfio.h
 *
 *  Created on: Sep 1, 2025
 *      Author: jason.p
 */

#ifndef APP_INC_RFIO_DRIVER_H_
#define APP_INC_RFIO_DRIVER_H_
#include "HumPRO_900.h"
#include "UART/UART.h"
#include "main.h"
#include "Module_Bus_Driver.h"

#define RFIO_LENGTH     1

typedef struct {
    volatile uint8_t RFIO_Event_Flag;
    volatile uint8_t latch_states;
} latch;

typedef enum
{
	rfio_msg_Event			= 0x22,		// Module Output
	rfio_msg_Config_Ports 	= 0x33,		// Controller Output
	rfio_msg_Set_Outputs	= 0x44,		// Controller Output
	rfio_msg_Read_Ports		= 0x55		// Input/Output
}GPIO_Msg_IDs_t;

// need read and write rfio func
// need to init the hum pro devices
void Init_RFIO(void);
void Init_RFIO_GPIO_Bus(void);
void Send_Latch_States(void);
void rfio_RX_Complete(void *callback_data, uint8_t *source_id, uint8_t *payload, uint8_t payload_len);
void Module_Bus_Rx_Callback(MODULE_BUS_RX_PACKET * packet);
void rfio_src_wrapper(uint8_t *source_id);

 // device assigns the reciever to the static struct

#endif /* APP_INC_RFIO_DRIVER_H_ */
