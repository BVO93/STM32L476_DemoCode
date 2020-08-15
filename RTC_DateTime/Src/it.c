/*
 * it.c
 *
 *  Created on: 30 Apr 2020
 *      Author: bjorn
 */

#include "main.h"


// make a global tick
// Global tick calls habndler, that all we need.
void SysTick_Handler(void)
{

	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}


void EXTI15_10_IRQHandler(void)
{

	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);
}








