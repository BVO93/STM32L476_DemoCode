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
void GPIO_Init(void);

void Timer2Init(void);

void UART2_Init(void);
void printClockFreq(void);



// HANDLES
UART_HandleTypeDef huart2;
TIM_HandleTypeDef htimer6;
TIM_HandleTypeDef htimer2;

// UART
uint16_t lenOfData = 0;
char *userData = "Hello , ready ! \r\n";
char msg[100];

uint32_t pulse1_value = 25000;		// 500Hz
uint32_t pulse2_value = 125000;		// 1000Hz
uint32_t pulse3_value = 6250;		// 2000Hz
uint32_t pulse4_value = 3125;		// 4000Hz

uint32_t ccr_content;

int main() {

	HAL_Init();
	SystemClockConfig();
	GPIO_Init();
	UART2_Init();

	Timer2Init();

	// Print f of clock to make sure they are working
	printClockFreq();
	HAL_Delay(100);




	// ***** TIMER PART ****** //

	if(HAL_TIM_OC_Start_IT(&htimer2, TIM_CHANNEL_1) != HAL_OK){
		Error_handler();
	}

	if(HAL_TIM_OC_Start_IT(&htimer2, TIM_CHANNEL_2) != HAL_OK){
			Error_handler();
		}

	if(HAL_TIM_OC_Start_IT(&htimer2, TIM_CHANNEL_3) != HAL_OK){
			Error_handler();
		}

	if(HAL_TIM_OC_Start_IT(&htimer2, TIM_CHANNEL_4) != HAL_OK){
			Error_handler();
		}

	if( HAL_TIM_Base_Start(&htimer6) != HAL_OK){
		Error_handler();
	};


	memset(msg, 0, sizeof(msg));
	sprintf(msg, "Timer init done \n");
	lenOfData = strlen(msg);
	HAL_UART_Transmit(&huart2, (uint8_t*) msg, lenOfData, HAL_MAX_DELAY);

	while (1);

	return 0;
}



// ******** SYSTEM CLOCK INIT ********** //
void SystemClockConfig(void) {
	RCC_OscInitTypeDef osc_init;
	RCC_ClkInitTypeDef clk_init;

	memset(&osc_init, 0, sizeof(osc_init));
	osc_init.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	osc_init.HSIState = RCC_HSI_ON;
	//osc_init.HSICalibrationValue =  To calibrate the value of the clk in steps of 80 Khz.
	if (HAL_RCC_OscConfig(&osc_init) != HAL_OK) {
		Error_handler();
	}

	clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK |
	RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
	clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
	clk_init.APB1CLKDivider = RCC_HCLK_DIV1;
	clk_init.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&clk_init, FLASH_LATENCY_0) != HAL_OK) {
		Error_handler();
	}

	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

	return;
}

// ******** INITIALIZE UART ********** //
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

	return;
}

// ******** PRINT CLOCK FREQ ********** //
void printClockFreq(void){


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
	sprintf(msg, "PCLK2: %ld\r\n\n", HAL_RCC_GetPCLK2Freq());
	lenOfData = strlen(msg);
	HAL_UART_Transmit(&huart2, (uint8_t*) msg, lenOfData, HAL_MAX_DELAY);

	return;
}

// ******** GPIO INIT ********** //
void GPIO_Init(void) {
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitTypeDef ledgpio;
	ledgpio.Pin = GPIO_PIN_5;
	ledgpio.Mode = GPIO_MODE_OUTPUT_PP;
	ledgpio.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &ledgpio);

	/*
	GPIO_InitTypeDef pinD8;
	pinD8.Pin = GPIO_PIN_8;
	pinD8.Mode = GPIO_MODE_OUTPUT_PP;
	pinD8.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &pinD8);

	GPIO_InitTypeDef pinD9;
	pinD9.Pin = GPIO_PIN_9;
	pinD9.Mode = GPIO_MODE_OUTPUT_PP;
	pinD9.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &pinD9);
*/
	// Set all pins low
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 0);
	//HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, 0);
	//HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, 0);

	return;
}


void Timer2Init(void){

	TIM_OC_InitTypeDef tim2OC_init;

	htimer2.Instance = TIM2;
	htimer2.Init.Period = 0xFFFFFFFF;
	htimer2.Init.Prescaler = 1;

	if(HAL_TIM_OC_Init(&htimer2) != HAL_OK){
		Error_handler();
	}

	tim2OC_init.OCMode = TIM_OCMODE_TOGGLE;
	tim2OC_init.OCPolarity = TIM_OCNPOLARITY_HIGH;
	tim2OC_init.Pulse =  pulse1_value;
	if(HAL_TIM_OC_ConfigChannel(&htimer2, &tim2OC_init, TIM_CHANNEL_1) != HAL_OK){
		Error_handler();
	}

	tim2OC_init.Pulse =  pulse2_value;
	if(HAL_TIM_OC_ConfigChannel(&htimer2, &tim2OC_init, TIM_CHANNEL_2) != HAL_OK){
		Error_handler();
	}

	tim2OC_init.Pulse =  pulse3_value;
	if(HAL_TIM_OC_ConfigChannel(&htimer2, &tim2OC_init, TIM_CHANNEL_3) != HAL_OK){
		Error_handler();
	}

	tim2OC_init.Pulse =  pulse4_value;
	if(HAL_TIM_OC_ConfigChannel(&htimer2, &tim2OC_init, TIM_CHANNEL_4) != HAL_OK){
		Error_handler();
	}

}


void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim){

	if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
	{
		ccr_content = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
		__HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_1, ccr_content+pulse1_value);

	}

	if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
	{
		ccr_content = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);
		__HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_2, ccr_content+pulse2_value);

	}

	if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
	{
		ccr_content = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_3);
		__HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_3, ccr_content+pulse3_value);
	}

	if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4)
	{
		ccr_content = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_4);
		__HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_4, ccr_content+pulse4_value);
	}

}

void Error_handler(void) {

	// If in error set D7 high and write message
	//HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, 1);

	memset(msg, 0, sizeof(msg));
	sprintf(msg, "Something went wrong, stuck in error  !");
	lenOfData = strlen(msg);
	HAL_UART_Transmit(&huart2, (uint8_t*) msg, lenOfData, HAL_MAX_DELAY);
	HAL_Delay(10);

	while (1);
}
