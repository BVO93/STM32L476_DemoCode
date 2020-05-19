/*
 * main.c
 *
 *  Created on: 30 Apr 2020
 *      Author: bjorn
 */

#include "main.h"
#include "it.h"
#include "string.h"

#define TRUE 1;
#define FALSE 0;

void SystemClockConfig(void);
void UART2_Init(void);
void Error_handler(void);
uint8_t convertToUpper(uint8_t data);

UART_HandleTypeDef huart2;  		// Declare a handle , has to be global

char *userData = "Hello , ready ! \r\n";
uint8_t dataBuffer[1000];
uint32_t count = 0;
uint8_t rcvdData;
uint8_t receptionComplete = FALSE;

int main() {

	HAL_Init();
	SystemClockConfig();
	UART2_Init();

	uint16_t lenOfData = strlen(userData);
	HAL_UART_Transmit(&huart2, (uint8_t*) userData, lenOfData, HAL_MAX_DELAY);


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

uint8_t convertToUpper(uint8_t data) {

	if (data >= 'a' && data <= 'z') {
		data = data - ('a' - 'A');
	}

	return data;
}

// This is the callback to the user.
// Find it in the hal_uart, if we want callback we need to search this
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {

	if (rcvdData == '\r') {
		receptionComplete = TRUE;
		HAL_UART_Transmit(huart, dataBuffer, count, HAL_MAX_DELAY);
		} else {

			dataBuffer[count++] = rcvdData;

		}

	}

	void Error_handler(void) {
		while (1)
			;
	}
