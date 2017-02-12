#include "stm32l476xx.h"
#include "SysClock.h"
#include "LED.h"
#include "UART.h"

#include <string.h>
#include <stdio.h>

char RxComByte = 0;
uint8_t buffer[BufferSize];
char str[256];
char str2[] = "My body is ready!!!!";
char str3[] = "It is a 1!!!";

int g_pendingInterrupt = 0;
uint32_t clockT = 0;

void setupGPIO(){
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	
	//Input from PA0 Alternate function mode
	GPIOA->MODER &= ~GPIO_MODER_MODER0;
	GPIOA->MODER |= GPIO_MODER_MODER0_1;
	
	//Seturp Alternate Function Bologna
	GPIOA->AFR[0] &= ~GPIO_AFRL_AFRL0;
	GPIOA->AFR[0] |= 0x0001;
	
	// //Set Speed
	// GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR0;
	// //Pulldown
	// GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR0_0;
	// GPIOA->PUPDR |= GPIO_PUPDR_PUPDR0_1;

}

void setupInterrupt(){
	//Enable the system Clock
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN | RCC_APB2ENR_SYSCFGEN;
	
	//Set PA0 to EXTI0
	SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PA;
	
	//Setup EXTI
	EXTI->IMR1 |= EXTI_IMR1_IM0;
	EXTI->RTSR1 |= EXTI_RTSR1_RT0;
	
	//Setup NVIC
	NVIC_SetPriority(EXTI0_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 1, 0));
	NVIC_EnableIRQ(EXTI0_IRQn);
	
}

void setupTimer2(){
	//Enable clock for timer 2
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;
	
	//Timer Prescale 80MHz / 40(0x28) -> 2MHz
	TIM2->PSC = 0x28;
	
	//Generate a Flipping Event 
	TIM2->EGR |= TIM_EGR_UG;
	
	//Turn off input capture
	TIM2->CCER &= ~TIM_CCER_CC1E;
	
	//Enable Input Capture channels
	TIM2->CCMR1 &= TIM_CCMR1_CC1S;
	TIM2->CCMR1 |= TIM_CCMR1_CC1S_0;
	
	//Re-enable input capture
	TIM2->CCER |= TIM_CCER_CC1E;
	
	//Input Caputure Filter 7 Cycles
	//TIM2->CCMR1 |= TIM_CCMR1_IC1F_2 | TIM_CCMR1_IC1F_1 | TIM_CCMR1_IC1F_0;
	
}

void EXTI0_IRQHandler(void){
	
	g_pendingInterrupt = 1;
	clockT = TIM2->CCR1;
	EXTI->PR1 |= EXTI_PR1_PIF0;
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
	setupTimer2();
	
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
			printf(str,  "An interrupt Happened!!!!\r\n Time is : %d", clockT);
			USART_Write(USART2, (uint8_t *)str, strlen(str));
			g_pendingInterrupt = 0;
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

