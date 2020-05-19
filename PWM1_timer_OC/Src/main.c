/*
 * main.c
 *
 *  Created on: 02-Jun-2018
 *      Author: kiran
 */

#include <string.h>
#include "stm32l4xx_hal.h"
#include "main.h"


void GPIO_Init(void);
void Error_handler(void);
void TIMER2_Init(void);
void UART2_Init(void);
void SystemClock_Config_HSI(uint8_t clock_freq);

TIM_HandleTypeDef htimer2;
UART_HandleTypeDef huart2;


int main(void)
{

	HAL_Init();


	SystemClock_Config_HSI(SYS_CLOCK_FREQ_50_MHZ);

	GPIO_Init();

	UART2_Init();

	TIMER2_Init();

	if ( HAL_TIM_PWM_Start(&htimer2,TIM_CHANNEL_1) != HAL_OK)
	{
		Error_handler();
	}

	if ( HAL_TIM_PWM_Start(&htimer2,TIM_CHANNEL_2) != HAL_OK)
	{
		Error_handler();
	}

	if ( HAL_TIM_PWM_Start(&htimer2,TIM_CHANNEL_3) != HAL_OK)
	{
		Error_handler();
	}

	if ( HAL_TIM_PWM_Start(&htimer2,TIM_CHANNEL_4) != HAL_OK)
	{
		Error_handler();
	}

	while(1);

	return 0;
}



/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config_HSI(uint8_t clock_freq)
{

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

	case SYS_CLOCK_FREQ_50_MHZ:
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
	case SYS_CLOCK_FREQ_80_MHZ:
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
	case SYS_CLOCK_FREQ_20_MHZ:
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
	/* SysTick_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);

}



void GPIO_Init(void)
{
    __HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitTypeDef ledgpio;
	ledgpio.Pin = GPIO_PIN_5;
	ledgpio.Mode = GPIO_MODE_OUTPUT_PP;
	ledgpio.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA,&ledgpio);
}



void UART2_Init(void)
{
	huart2.Instance = USART2;
	huart2.Init.BaudRate = 115200;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	if ( HAL_UART_Init(&huart2) != HAL_OK )
	{
		//There is a problem
		Error_handler();
	}


}


 void TIMER2_Init(void)
{

	 TIM_OC_InitTypeDef tim2PWM_Config;
	 htimer2.Instance = TIM2;
	 htimer2.Init.Period = 10-1;
	 htimer2.Init.Prescaler = 4999;
	 if ( HAL_TIM_PWM_Init(&htimer2) != HAL_OK)
	 {
		 Error_handler();
	 }

	 memset(&tim2PWM_Config,0,sizeof(tim2PWM_Config));

	 tim2PWM_Config.OCMode = TIM_OCMODE_PWM1;
	 tim2PWM_Config.OCPolarity = TIM_OCPOLARITY_HIGH;
   	 tim2PWM_Config.Pulse =  (htimer2.Init.Period * 25 ) /100;

	 if( HAL_TIM_PWM_ConfigChannel(&htimer2,&tim2PWM_Config,TIM_CHANNEL_1) != HAL_OK)
	 {
		 Error_handler();
	 }

	 tim2PWM_Config.Pulse =  (htimer2.Init.Period * 45 ) /100;
	 if( HAL_TIM_PWM_ConfigChannel(&htimer2,&tim2PWM_Config,TIM_CHANNEL_2) != HAL_OK)
	 {
		 Error_handler();
	 }

	 tim2PWM_Config.Pulse =  (htimer2.Init.Period * 75 ) /100;
	 if( HAL_TIM_PWM_ConfigChannel(&htimer2,&tim2PWM_Config,TIM_CHANNEL_3) != HAL_OK)
	 {
		 Error_handler();
	 }

	 tim2PWM_Config.Pulse =  (htimer2.Init.Period * 95 ) /100;
	 if( HAL_TIM_PWM_ConfigChannel(&htimer2,&tim2PWM_Config,TIM_CHANNEL_4) != HAL_OK)
	 {
		 Error_handler();
	 }

}


void Error_handler(void)
{
	while(1);
}

