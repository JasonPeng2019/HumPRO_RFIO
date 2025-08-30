/*
 * GPIO.c
 *
 *  Created on: Jul 5, 2020
 *      Author: devin
 */

#include "GPIO.h"
#include <stdlib.h>

void Init_GPIO(GPIO * gpio, GPIO_TypeDef * Port, uint16_t Pin)
{
	gpio->Port = Port;
	gpio->Pin = Pin;
	Read_GPIO_State(gpio);
}

void Change_GPIO_Configuration(GPIO * gpio, GPIO_Mode Mode, GPIO_Pull Pull)
{
	if(gpio != NULL)
	{
		GPIO_InitTypeDef GPIO_InitStruct = {0};

		GPIO_InitStruct.Pin = gpio->Pin;

		switch(Mode)
		{
		case eGPIO_Input: GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
			break;
		case eGPIO_Output_PP: GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
			break;
		case eGPIO_Output_OD: GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
			break;
		}

		switch(Pull)
		{
		case eGPIO_No_Pull: GPIO_InitStruct.Pull = GPIO_NOPULL;
			break;
		case eGPIO_Pull_Up: GPIO_InitStruct.Pull = GPIO_PULLUP;
			break;
		case eGPIO_Pull_Down: GPIO_InitStruct.Pull = GPIO_PULLDOWN;
			break;
		}

		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

		HAL_GPIO_Init(gpio->Port, &GPIO_InitStruct);
	}
}

GPIO_State Read_GPIO_State(GPIO * gpio)
{
	if(gpio != NULL)
	{
		gpio->Current_State = (GPIO_State)HAL_GPIO_ReadPin(gpio->Port, gpio->Pin);
		return gpio->Current_State;
	}

	return eGPIO_Low;
}

void Set_GPIO_State_High(GPIO * gpio)
{
	if(gpio != NULL)
	{
		gpio->Current_State = eGPIO_High;
		HAL_GPIO_WritePin(gpio->Port, gpio->Pin, (GPIO_PinState)gpio->Current_State);
	}
}

void Set_GPIO_State_Low(GPIO * gpio)
{
	if(gpio != NULL)
	{
		gpio->Current_State = eGPIO_Low;
		HAL_GPIO_WritePin(gpio->Port, gpio->Pin, (GPIO_PinState)gpio->Current_State);
	}
}

