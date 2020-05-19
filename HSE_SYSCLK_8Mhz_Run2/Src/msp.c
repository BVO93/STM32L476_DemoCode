/*
 * msp.c
 *
 *  Created on: 30 Apr 2020
 *      Author: bjorn
 */

#include "stm32l4xx_hal.h"
#include "stm32l4xx_hal_gpio.h"
#include "stm32l4xx_hal_gpio_ex.h"
#include "stm32l4xx_hal_cortex.h"

void HAL_MSPInit(void){

	// 1. Set up the priority grouping of the arm cortex mx processor
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

	//2. enable the required system exceptions of the arm cortex
	SCB->SHCSR |= 0x7 << 16;		// usage fault, mem fault , bus fault enabled

	//3. configure priority for the system exception
	// in enum at stm32lxx.h   THese corenspond with  the interupts we have set in point 2
	HAL_NVIC_SetPriority(MemoryManagement_IRQn, 0, 0);
	HAL_NVIC_SetPriority(BusFault_IRQn, 0, 0);
	HAL_NVIC_SetPriority(UsageFault_IRQn, 0, 0);

}


// Low level init of the UART connection
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	GPIO_InitTypeDef gpio_uart;

	// 1. Enable the clock for USART2 perhipheral and for the GPIO
	__HAL_RCC_USART2_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	//2. Pin muxing config
	gpio_uart.Pin = GPIO_PIN_2;			// uART2 TX
	gpio_uart.Mode = GPIO_MODE_AF_PP; 	// push pull
	gpio_uart.Pull = GPIO_PULLUP;
	gpio_uart.Speed = GPIO_SPEED_FREQ_LOW;
	gpio_uart.Alternate = GPIO_AF7_USART2;   // Goes wrong a lot
	HAL_GPIO_Init(GPIOA, &gpio_uart);

	//Only change pin, the rest is same
	gpio_uart.Pin = GPIO_PIN_3;   		// UART2 RX
	HAL_GPIO_Init(GPIOA, &gpio_uart);


	//3. setting up IRQ en set up priority (NVIC settings (processor dependant))
	// Check hal cortex.c
	HAL_NVIC_EnableIRQ(USART2_IRQn);
	HAL_NVIC_SetPriority(USART2_IRQn, 15, 0);


}
