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

void UART2_Init(void);
void printClockFreq(void);

void TIMER2_init(void);
void LSE_configuration(void);

// HANDLES
UART_HandleTypeDef huart2;
TIM_HandleTypeDef htimer2;

// UART
uint16_t lenOfData = 0;
char *userData = "Hello , ready ! \r\n";
char msg[100];

int count = 0;

uint32_t input_capture[2] = { 0 };
uint8_t count2 = 1;
u_int8_t is_capture_done = FALSE;

uint32_t capture_difference = 0;

int main() {

	HAL_Init();
	SystemClockConfig();
	GPIO_Init();
	UART2_Init();

	double timer2_cnt_freq = 0;
	double timer2_cnt_res = 0;
	double user_signal_time_period = 0;
	double user_signal_freq = 0;

	// Print f of clock to make sure they are working
	printClockFreq();

	// Timer init

	TIMER2_init();

	// We need to give timer 2 an input to measure the f on, so use LSE
	LSE_configuration();

	HAL_TIM_IC_Start_IT(&htimer2,TIM_CHANNEL_1);


	memset(msg, 0, sizeof(msg));
	sprintf(msg, "Timer init done \n");
	lenOfData = strlen(msg);
	HAL_UART_Transmit(&huart2, (uint8_t*) msg, lenOfData, HAL_MAX_DELAY);

	while (1) {
		if (is_capture_done) {
			if (input_capture[1] > input_capture[0]) {
				capture_difference = input_capture[1] - input_capture[0];

			} else {
				capture_difference = (0XFFFFFFFF - input_capture[0])
						+ input_capture[1];
			}

			timer2_cnt_freq = (HAL_RCC_GetPCLK1Freq() * 2
					/ (htimer2.Init.Prescaler + 1 ));
			timer2_cnt_res = 1 / timer2_cnt_freq;
			user_signal_time_period = capture_difference * timer2_cnt_res;
			user_signal_freq = 1 / user_signal_time_period;

			memset(msg, 0, sizeof(msg));
			sprintf(msg, "User freq: %lf\r\n", user_signal_freq);
			lenOfData = strlen(msg);
			HAL_UART_Transmit(&huart2, (uint8_t*) msg, lenOfData,
					HAL_MAX_DELAY);

			is_capture_done = FALSE;

		}
	}

	while (1)
		;

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
void printClockFreq(void) {

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
	__HAL_RCC_GPIOA_CLK_ENABLE()
	;
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
	 */

	GPIO_InitTypeDef pinD9;
	pinD9.Pin = GPIO_PIN_9;
	pinD9.Mode = GPIO_MODE_OUTPUT_PP;
	pinD9.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &pinD9);

	// Set all pins low
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 0);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, 0);
//	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, 0);

	return;
}

// Init of the timer 2

void TIMER2_init(void) {

	TIM_IC_InitTypeDef timer2IC_COnfig;

	htimer2.Instance = TIM2;
	htimer2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htimer2.Init.Period = 0xFFFFFFFF; 	// 32 bit counter
	htimer2.Init.Prescaler = 1;

	if (HAL_TIM_IC_Init(&htimer2) != HAL_OK) {
		Error_handler();
	}

	timer2IC_COnfig.ICFilter = 0;
	timer2IC_COnfig.ICPolarity = TIM_ICPOLARITY_RISING;
	timer2IC_COnfig.ICPrescaler = TIM_ICPSC_DIV1;
	timer2IC_COnfig.ICSelection = TIM_ICSELECTION_DIRECTTI;

	if (HAL_TIM_IC_ConfigChannel(&htimer2, &timer2IC_COnfig, TIM_CHANNEL_1)
			!= HAL_OK) {

		Error_handler();
	};

	return;
}

void LSE_configuration(void) {

	RCC_OscInitTypeDef osc_init;

	osc_init.OscillatorType = RCC_OSCILLATORTYPE_LSE;
	osc_init.LSEState = RCC_LSE_ON;

	if (HAL_RCC_OscConfig(&osc_init) != HAL_OK) {
		Error_handler();
	}

	HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_LSE, RCC_MCODIV_1);
}

// Capture the value of the timer at the moment the capture is been taken

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {

	if (!is_capture_done) {

		if (count2 == 1) {
			input_capture[0] = __HAL_TIM_GET_COMPARE(htim, TIM_CHANNEL_1);
			count2++;
		} else if (count2 == 2) {
			input_capture[1] = __HAL_TIM_GET_COMPARE(htim, TIM_CHANNEL_1);
			count2 = 1;
			is_capture_done = TRUE;
		}
	}

}

void Error_handler(void) {

	// If in error set D7 high and write message
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, 1);

	memset(msg, 0, sizeof(msg));
	sprintf(msg, "Something went wrong, stuck in error  !");
	lenOfData = strlen(msg);
	HAL_UART_Transmit(&huart2, (uint8_t*) msg, lenOfData, HAL_MAX_DELAY);
	HAL_Delay(10);

	while (1)
		;
}