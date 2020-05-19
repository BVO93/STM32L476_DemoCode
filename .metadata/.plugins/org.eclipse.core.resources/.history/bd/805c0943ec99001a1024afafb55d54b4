/*
 * main.c
 *
 *  Created on: 30 Apr 2020
 *      Author: bjorn
 */

#include<string.h>
#include "stm32l4xx_hal.h"
#include "main.h"

void SystemClockConfig(void);
void Error_handler(void);
void TIMER6Init(void);
void GPIO_Init(void);

TIM_HandleTypeDef htimer6;

int main() {

	HAL_Init();
	SystemClockConfig();
	GPIO_Init();

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, 1);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, 0);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, 0);

	TIMER6Init();

	// Start timer
	// Implement the int in it.c
	HAL_TIM_Base_Start_IT(&htimer6);

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, 1);


	while (1);

	return 0;
}

void TIMER6Init(void) {

	htimer6.Instance = TIM6;
	htimer6.Init.Prescaler = 60;
	htimer6.Init.Period = 640 - 1;

	if (HAL_TIM_Base_Init(&htimer6) != HAL_OK) {
		Error_handler();
	}

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, 1);

	return;
}

void SystemClockConfig(void) {

	return;
}

void GPIO_Init(void) {
	__HAL_RCC_GPIOA_CLK_ENABLE()
	;
	GPIO_InitTypeDef ledgpio;
	ledgpio.Pin = GPIO_PIN_5;
	ledgpio.Mode = GPIO_MODE_OUTPUT_PP;
	ledgpio.Pull = GPIO_NOPULL;

	HAL_GPIO_Init(GPIOA, &ledgpio);

	return;
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);

}


void Error_handler(void) {

	while (1)
		;
}
