/*
 * it.c
 *
 *  Created on: 30 Apr 2020
 *      Author: bjorn
 */

#include "it.h"
#include "main.h"


extern UART_HandleTypeDef huart2;

// make a global tick
// Global tick calls habndler, that all we need.
void SysTick_Handler(void)
{

	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}


// This handler is called when a byte is recieved
void USART2_IRQHandler(void){
	HAL_UART_IRQHandler(&huart2);


}
