#include "stm32f4xx.h"
#include "Moter.h"
#include "Delay.h"
#include "Receiver.h"

int16_t motor[4];

void PWM_Init()
{
	RCC->AHB1ENR |= 1 << 2;
	RCC->APB1ENR |= 1 << 1;

	//PC6789
	GPIOC->MODER &= ~((3 << 12)|(3 << 14)|(3 << 16)|(3 << 18)); //return to 00
	GPIOC->MODER |= (1 << 13) | (1 << 15)|(1 << 17)|(1 << 19);//PC6/7/8/9 :AF
	GPIOC->OTYPER &= ~((1 << 6)|(1 << 7)|(1 << 8)|(1 << 9));//PP
	GPIOC->PUPDR |= (1 << 12) | (1 << 14)|(1 << 16)|(1 << 18);//up
	GPIOC->OSPEEDR |= ((3 << 12)|(3 << 14)|(3 << 16)|(3 << 18));//very High speed
	
	GPIOC->AFR[0] &= ~(0xFF << 24 );//Alternate function: PWM
	GPIOC->AFR[1] &= ~(0xFF);
	GPIOC->AFR[0] |= 2 << 24;
	GPIOC->AFR[0] |= 2 << 28;
  GPIOC->AFR[1] |= 2 ;
  GPIOC->AFR[1] |= 2 << 4;


	//PWMFreq = 84000000/84/10000
	//Reso = 1/ARR + 1 =>1/10000
  TIM3->ARR = 20000 - 1;
	TIM3->PSC |= 84 - 1;
	ChangeDutyCycle(7);
	
	TIM3->CR1 |= 1 << 7;//Auto-reload preload enable
	TIM3->CR1 &= ~( 1 << 4);//Counter used as upcounter
  TIM3->CCMR1 |= (6 << 4)|(6 << 12);//CH1 CH2 PWM mode 1
	TIM3->CCMR1 |= (1 << 3)|(1 << 11);//CH1 CH2 Output compare 1 preload enable
	TIM3->CCMR2 |= (6 << 4)|(6 << 12);//CH3 CH4 PWM mode 1
	TIM3->CCMR2 |= (1 << 3)|(1 << 11);//CH3 CH4 Output compare 1 preload enable
		
	TIM3->CCER |=1|(1 << 4)|(1 << 8)|(1 << 12);//Compare 1/2/3/4 output enable
	
	TIM3->CR1 |= 1;//counter enable

}


void ChangeDutyCycle(int Duty)
{
	TIM3->CCR1 = Duty * (TIM3 -> ARR) / 100;
	TIM3->CCR2 = Duty * (TIM3 -> ARR) / 100;
	TIM3->CCR3 = Duty * (TIM3 -> ARR) / 100;
	TIM3->CCR4 = Duty * (TIM3 -> ARR) / 100;
}

void ChangeAccelrator(int16_t motor[])
{
	motor[0] = (int16_t)(((float)(receiver.ch[3] - 0x03E8))/(0x07C4 - 0x03E8)*1000 +1000);
	TIM3->CCR1 = motor[0];
	TIM3->CCR2 = motor[0];
	TIM3->CCR3 = motor[0];
	TIM3->CCR4 = motor[0];
}


	

