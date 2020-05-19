/*
 * main.c
 *
 *  Created on: 30 Apr 2020
 *      Author: bjorn
 */

#include <stdio.h>
#include "main.h"
#include "it.h"
#include "string.h"

#define TRUE 1;
#define FALSE 0;

void SystemClockConfig(void);
void UART2_Init(void);
void Error_handler(void);


UART_HandleTypeDef huart2;  		// Declare a handle , has to be global

char *userData = "Hello , ready to mingle ! \r\n";
char msg[100];

int main() {

	RCC_OscInitTypeDef osc_init;
	RCC_ClkInitTypeDef clk_init;

	HAL_Init();



	memset(&osc_init, 0, sizeof(osc_init));
	osc_init.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	osc_init.HSIState = RCC_HSI_ON;
	//osc_init.HSICalibrationValue =  To calibrate the value of the clk in steps of 80 Khz.
	if(HAL_RCC_OscConfig(&osc_init) != HAL_OK){
		Error_handler();
	}

	clk_init.ClockType= RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | \
			RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
	clk_init.AHBCLKDivider = RCC_SYSCLK_DIV2;
	clk_init.APB1CLKDivider = RCC_HCLK_DIV2;
	clk_init.APB2CLKDivider = RCC_HCLK_DIV2;

	if(HAL_RCC_ClockConfig(&clk_init,FLASH_LATENCY_0) != HAL_OK){
		Error_handler();
	}

	// Disable HSI

	//__HAL_RCC_HSI_DISABLE();

	// Setup systick config
	// Give tick every ms
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

	// LAst diveder before going to cortec clk. Default 1 possible 8
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);


	UART2_Init();


	uint16_t lenOfData = strlen(userData);
	HAL_UART_Transmit(&huart2, (uint8_t*) userData, lenOfData, HAL_MAX_DELAY);


	memset(msg, 0, sizeof(msg));
	sprintf(msg, "SYSCLK: %ld\r\n", HAL_RCC_GetSysClockFreq());
	lenOfData = strlen(msg);
	HAL_UART_Transmit(&huart2, (uint8_t*) msg, lenOfData, HAL_MAX_DELAY);


	memset(msg, 0, sizeof(msg));
	sprintf(msg, "PCLK1: %ld\r\n", HAL_RCC_GetPCLK1Freq());
	lenOfData = strlen(msg);
	HAL_UART_Transmit(&huart2, (uint8_t*) msg, lenOfData, HAL_MAX_DELAY);



	while (1);

	return 0;
}






void SystemClockConfig(void) {

}


// Get data out of STm32Hal -> INc -> Hal_uart.h
void UART2_Init(void) {
	huart2.Instance = USART2; // This is declared in stm32l476xx.h as the coorect adress
	huart2.Init.BaudRate = 115200;
	huart2.Init.WordLength = UART_WORDLENGTH_8B; 	// 8 bit word legth
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;

	if (HAL_UART_Init(&huart2) != HAL_OK) {	// Everything is set, now give adress of huart
		Error_handler();								// Check if went ok
	}

}

	void Error_handler(void) {
		while (1)
			;
	}