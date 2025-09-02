/*
 * rfio_driver.c
 *
 *  Created on: Sep 1, 2025
 *      Author: 14005
 */

#include "rfio_driver.h"
#include "Module_Bus_Driver.h"
#include <string.h>

extern UART_HandleTypeDef huart1;

static HumPRO_900_t HumPRO_900_Struct;
static HumPRO_900_t * HumPRO_900 = &HumPRO_900_Struct;

//reciever already initialized inside HumPro_900_Struct

static UART RFIO_UART_Bus;
static UART * RFIO_uart = &RFIO_UART_Bus;

static uint8_t UART_TX_Buffer[256];
static uint8_t UART_RX_Buffer[256];

static GPIO Power_Down_GPIO_Struct;
static GPIO * Power_Down_GPIO = &Power_Down_GPIO_Struct;

static GPIO Reset_GPIO_Struct;
static GPIO * Reset_GPIO = &Reset_GPIO_Struct;

static GPIO CMD_GPIO_Struct;
static GPIO * CMD_GPIO = &CMD_GPIO_Struct;

static GPIO BE_GPIO_Struct;
static GPIO * BE_GPIO = &BE_GPIO_Struct;

static GPIO EX_GPIO_Struct;
static GPIO * EX_GPIO = &EX_GPIO_Struct;

static GPIO CRESP_GPIO_Struct;
static GPIO * CRESP_GPIO = &CRESP_GPIO_Struct;

static latch RFIO_latch_struct;
static latch * RFIO_latch = &RFIO_latch_struct;

const Module_Slave_Bus_t *pbus = NULL;


void Init_RFIO(void){
    Init_RFIO_GPIO_Bus();
    
    Init_DMA_UART(RFIO_uart, &huart1, UART_TX_Buffer, sizeof(UART_TX_Buffer), UART_RX_Buffer, sizeof(UART_RX_Buffer));
    
    pbus = Init_Module_Bus(I2C1, INT_OUT_GPIO_Port, INT_OUT_Pin, &FWVer);

    Init_HumPRO_900(HumPRO_900, RFIO_uart, Power_Down_GPIO, Reset_GPIO, CMD_GPIO, BE_GPIO, EX_GPIO, CRESP_GPIO);

    HumPRO_900->Receiver.RX_Complete_Callback = rfio_RX_Complete;

    RFIO_latch->RFIO_Event_Flag = 0;
    RFIO_latch->latch_states = 0;
}

void Init_RFIO_GPIO_Bus(void){
    Init_GPIO(Power_Down_GPIO, N_CTS_GPIO_Port, N_CTS_Pin);
    Init_GPIO(Reset_GPIO, N_RST_GPIO_Port, N_RST_Pin);
    Init_GPIO(CMD_GPIO, N_CMD_GPIO_Port, N_CMD_Pin);
    Init_GPIO(BE_GPIO, BE_GPIO_Port, BE_Pin);
    Init_GPIO(EX_GPIO, EX_GPIO_Port, EX_Pin);
    Init_GPIO(CRESP_GPIO, N_CRESP_GPIO_Port, N_CRESP_Pin);
}

void Send_Latch_States(void){
    uint8_t Data[2];
    Data[0] = RFIO_latch->latch_states;
    Data[1] = RFIO_latch->RFIO_Event_Flag;
    MODULE_BUS_PACKET * packet = Create_Module_Bus_Packet(rfio_msg_Read_Ports, 1, 1, 2, (unsigned char*)Data);
    Module_Bus_Buffered_Send(packet, 0);
}

void rfio_RX_Complete(void *callback_data, uint8_t *source_id, uint8_t *payload, uint8_t payload_len){
    rfio_src_wrapper(source_id);
    //not an RFIO message, return; need a more complex system as more RFIO is implemented. 
    // i.e, RFIO_MSG_ID, a certain magic number reserved for RFIO messages
    if (payload_len != RFIO_LENGTH){
        return;
    }
    uint8_t new_latch;
    new_latch = payload[0];
    // if latch is the same, return
    if (RFIO_latch->latch_states == new_latch) {
            return;
    }

    // if latch is not the same, send the latch events
    RFIO_latch->RFIO_Event_Flag = RFIO_latch->latch_states ^ new_latch;
    RFIO_latch->latch_states = new_latch;
    uint8_t Data[2];
    Data[0] = RFIO_latch->latch_states;
    Data[1] = RFIO_latch->RFIO_Event_Flag;
    MODULE_BUS_PACKET * packet = Create_Module_Bus_Packet(rfio_msg_Read_Ports, 1, 1, 2, (unsigned char*)Data);
    Module_Bus_Buffered_Send(packet, 1);

}

void Module_Bus_Rx_Callback(MODULE_BUS_RX_PACKET * packet){
    switch(packet->RX_Packet.Formatted_Data.Packet_Header.Formatted_Packet_Header.Command){
        case rfio_msg_Read_Ports:
            {
                Send_Latch_States();
            }
            break;
        // put other cases below if decide to later implement
    }

}


void rfio_src_wrapper(uint8_t *source_id){
    uint8_t local_src_id[4];   
    memcpy(local_src_id, source_id, sizeof(local_src_id));

    //do things with the source id here
    return;
}