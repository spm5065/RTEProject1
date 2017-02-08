#include "stm32l476xx.h"
#include "SysClock.h"
#include "LED.h"
#include "UART.h"

#include <string.h>
#include <stdio.h>

char RxComByte = 0;
uint8_t buffer[BufferSize];
char str[] = "An interrupt Happened!!!!";
char str2[] = "My body is ready!!!!";
char str3[] = "It is a 1!!!";

int g_pendingInterrupt = 0;

void setupGPIO(){
	//Input from PA0
	GPIOA->MODER &= !GPIO_MODER_MODER0;
	//Set Speed
	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR0;
	//Pulldown
	GPIOA->PUPDR &= !GPIO_PUPDR_PUPDR0_0;
	GPIOA->PUPDR |= GPIO_PUPDR_PUPDR0_1;
	
	//GPIOD->AFR[0] |= 0;
}

void setupInterrupt(){
	//Enable the system Clock
	RCC->APB2ENR |= (1 << 14);
	
	//Set PA0 to EXTI0
	SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PA;
	
	//Setup EXTI
	EXTI->IMR1 |= EXTI_IMR1_IM0;
	EXTI->RTSR1 |= EXTI_RTSR1_RT0;
	
	//Setup NVIC
	NVIC_SetPriority(EXTI0_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 1, 0));
	NVIC_EnableIRQ(EXTI0_IRQn);
	
}

void EXTI0_IRQHandler(void){
	
	g_pendingInterrupt = 1;
	EXTI->PR1 &= !EXTI_PR1_PIF0;
}

int main(void){
	char rxByte;
	int		a ;
	int		n ;
	int		i ;
	float b;
	
	System_Clock_Init(); // Switch System Clock = 80 MHz
	LED_Init();
	UART2_Init();
	setupGPIO();
	setupInterrupt();
	
	USART_Write(USART2, (uint8_t *)str2, strlen(str2));
	
	while (1){
//		n = sprintf((char *)buffer, "a = %d\t", a);
//		n += sprintf((char *)buffer + n, "b = %f\r\n", b);
//		USART_Write(USART2, buffer, n);		
//		a = a + 1;
//		b = (float)a/100;
//		// now spin for a while to slow it down
//		for (i = 0; i < 4000000; i++)
//			;
	
		if(g_pendingInterrupt){
			USART_Write(USART2, (uint8_t *)str, strlen(str));
			g_pendingInterrupt = 0;
		}
		
		if(GPIOA->IDR & GPIO_IDR_IDR_0){
			USART_Write(USART2, (uint8_t *)str3, strlen(str3));
		}
		
		
		
//			USART_Write(USART2, (uint8_t *)str, strlen(str));	
//			rxByte = USART_Read(USART2);
//			if (rxByte == 'N' || rxByte == 'n'){
//				Red_LED_Off();
//				USART_Write(USART2, (uint8_t *)"LED is Off\r\n\r\n", 16);
//			}
//			else if (rxByte == 'Y' || rxByte == 'y'){
//				Red_LED_On();
//				USART_Write(USART2, (uint8_t *)"LED is on\r\n\r\n", 15);
//			}
	}
}

