/*
 * GPIO.h
 *
 *  Created on: Jul 5, 2020
 *      Author: devin
 */

#ifndef GPIO_GPIO_H_
#define GPIO_GPIO_H_

#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
	eGPIO_Low = 0,
	eGPIO_High
}GPIO_State;

typedef enum
{
	eGPIO_Input = 0,
	eGPIO_Output_PP,
	eGPIO_Output_OD
}GPIO_Mode;

typedef enum
{
	eGPIO_No_Pull = 0,
	eGPIO_Pull_Up,
	eGPIO_Pull_Down
}GPIO_Pull;

typedef struct
{
	GPIO_TypeDef * Port;
	uint16_t Pin;
	GPIO_State Current_State;
}GPIO;


void Init_GPIO(GPIO * gpio, GPIO_TypeDef * Port, uint16_t Pin);
void Change_GPIO_Configuration(GPIO * gpio, GPIO_Mode Mode, GPIO_Pull Pull);
GPIO_State Read_GPIO_State(GPIO * gpio);
void Set_GPIO_State_High(GPIO * gpio);
void Set_GPIO_State_Low(GPIO * gpio);



#ifdef __cplusplus
}
#endif



#endif /* GPIO_GPIO_H_ */
