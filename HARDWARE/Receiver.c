#include "stm32f4xx.h"
#include "Receiver.h"

Rec receiver = { 0 };

void Receiver_Init()
{
	//PA8
	RCC->AHB1ENR |= 1;
	RCC->APB2ENR |= 1;

	GPIOA->MODER &= ~(3 << 16); //return to 00
	GPIOA->MODER |= 1 << 17;//PA8 :AF
	GPIOA->PUPDR |= 1 << 16;//up     73
	GPIOA->OSPEEDR |= 3 << 16;//very High speed
	
	GPIOA->AFR[1] &= ~0xF;
  GPIOA->AFR[1] |= 1 ;
	
	TIM1->ARR = 65535;
	TIM1->PSC = 84 - 1 ;
	
	TIM1->CR1 &= ~( 1 << 4);//Counter used as upcounter
	TIM1->CCMR1 |= 1;//CC1 channel is configured as input, IC1 is mapped on TI1
	TIM1->CCMR1 &= ~(3 << 2);// no prescaler, capture is done each time an edge is detected on the capture input
	TIM1->CCMR1 &= ~(0xF << 4);//No filter, sampling is done at fDTS

	TIM1->DIER |= 1 << 1;// Capture 1 interrupt enable
	
	//NVIC
  //NVIC Group 2 (Set in Serial.c)
	NVIC->IP[27] = 0xa0;
	NVIC->ISER[0] |= 1 << 27;
	
	TIM1->CR1 |= 1;//Counter enable
	TIM1->CCER |= 1;// Capture enabled
}

void TIM1_CC_IRQHandler(void)
{
	if (TIM1->SR & 0x2 )// Capture/Compare 1 interrupt flag 
	{
		TIM1->CNT = 0;
		if(TIM1->CCR1 >= 4000)
			{
				receiver.yes = 1;
				receiver.now = 0;
			}	
		if(receiver.yes == 1)
			{
			  receiver.ch[receiver.now++] = TIM1->CCR1;
				if(receiver.now == 9)
				{
					receiver.now = 0;
					receiver.yes = 0;
				}
		  }
	}
}
