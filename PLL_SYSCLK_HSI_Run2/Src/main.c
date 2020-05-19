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

void SystemClockConfig(uint8_t clock_freq);
void UART2_Init(void);
void Error_handler(void);


UART_HandleTypeDef huart2;  		// Declare a handle , has to be global

char *userData = "Hello , ready to mingle ! \r\n";
char msg[100];

int main() {


	HAL_Init();

	SystemClockConfig(SYS_CLOCK_FREQ_20_MHz);

	UART2_Init();

	uint16_t lenOfData = strlen(userData);
	HAL_UART_Transmit(&huart2, (uint8_t*) userData, lenOfData, HAL_MAX_DELAY);



	memset(msg, 0, sizeof(msg));
	sprintf(msg, "SYSCLK: %ld\r\n", HAL_RCC_GetSysClockFreq());
	lenOfData = strlen(msg);
	HAL_UART_Transmit(&huart2, (uint8_t*) msg, lenOfData, HAL_MAX_DELAY);

	memset(msg, 0, sizeof(msg));
	sprintf(msg, "HCLK: %ld\r\n", HAL_RCC_GetHCLKFreq());
	lenOfData = strlen(msg);
	HAL_UART_Transmit(&huart2, (uint8_t*) msg, lenOfData, HAL_MAX_DELAY);

	memset(msg, 0, sizeof(msg));
	sprintf(msg, "PCLK1: %ld\r\n", HAL_RCC_GetPCLK1Freq());
	lenOfData = strlen(msg);
	HAL_UART_Transmit(&huart2, (uint8_t*) msg, lenOfData, HAL_MAX_DELAY);

	memset(msg, 0, sizeof(msg));
	sprintf(msg, "PCLK2: %ld\r\n", HAL_RCC_GetPCLK2Freq());
	lenOfData = strlen(msg);
	HAL_UART_Transmit(&huart2, (uint8_t*) msg, lenOfData, HAL_MAX_DELAY);

	while (1);

	return 0;
}






void SystemClockConfig(uint8_t clock_freq) {

	RCC_OscInitTypeDef osc_init;
	RCC_ClkInitTypeDef clk_init;

	uint32_t Flatency = 0;

	osc_init.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	osc_init.HSIState = RCC_HSI_ON;
	osc_init.MSICalibrationValue = 16;



	osc_init.PLL.PLLState = RCC_PLL_ON;
	osc_init.PLL.PLLSource = RCC_PLLSOURCE_HSI;

	switch(clock_freq)
	{

	case SYS_CLOCK_FREQ_50_MHz:
	{
		osc_init.PLL.PLLM = 4;
		osc_init.PLL.PLLN = 25;
		osc_init.PLL.PLLP = 7;
		osc_init.PLL.PLLQ = 2;
		osc_init.PLL.PLLR = 2;

		clk_init.ClockType= RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | \
				RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
		clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
		clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
		clk_init.APB1CLKDivider = RCC_HCLK_DIV2;
		clk_init.APB2CLKDivider = RCC_HCLK_DIV2;

		Flatency = FLASH_ACR_LATENCY_1WS;

		break;
	}
	case SYS_CLOCK_FREQ_80_MHz:
	{
		osc_init.PLL.PLLM = 1;
		osc_init.PLL.PLLN = 10;
		osc_init.PLL.PLLP = 7;
		osc_init.PLL.PLLQ = 2;
		osc_init.PLL.PLLR = 2;

		clk_init.ClockType= RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | \
						RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
		clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
		clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
		clk_init.APB1CLKDivider = RCC_HCLK_DIV2;
		clk_init.APB2CLKDivider = RCC_HCLK_DIV2;

		Flatency = FLASH_ACR_LATENCY_2WS;

		break;

	}
	case SYS_CLOCK_FREQ_20_MHz:
	{
		osc_init.PLL.PLLM = 4;
		osc_init.PLL.PLLN = 20;
		osc_init.PLL.PLLP = 7;
		osc_init.PLL.PLLQ = 2;
		osc_init.PLL.PLLR = 4;

		clk_init.ClockType= RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | \
						RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
		clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
		clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
		clk_init.APB1CLKDivider = RCC_HCLK_DIV2;
		clk_init.APB2CLKDivider = RCC_HCLK_DIV2;

		Flatency = FLASH_ACR_LATENCY_1WS;

		break;
	}

	default:
		return;

	}

	if(HAL_RCC_OscConfig(&osc_init) != HAL_OK){
		Error_handler();
	}


	if(HAL_RCC_ClockConfig(&clk_init, Flatency) != HAL_OK){
		Error_handler();
	}

	// set up systick
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

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
