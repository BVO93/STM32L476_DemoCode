/*
 * it.c
 *
 *  Created on: 30 Apr 2020
 *      Author: bjorn
 */

#include "main.h"

extern TIM_HandleTypeDef htimer6;;

// make a global tick
// Global tick calls habndler, that all we need.
void SysTick_Handler(void)
{

	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}


void TIM6_DAC_IRQHandler(void){
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, 1);
	HAL_TIM_IRQHandler(&htimer6);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, 0);
}








