#include "stm32f4xx.h"
#include <stdio.h>
#include "Serial.h"

uint8_t Serial_RxData;
uint8_t Serial_RxFlag;


void Serial_Init()
{
	//GPIOB USART1 clock enable
	RCC->AHB1ENR |= 1 << 1;
	RCC->APB2ENR |= 1 << 4;
	
	//GPIOA Init  
	//PB6 TX   &&    PB7  RX
	GPIOB->MODER &= ~((3 << 12)|(3 << 14)); //return to 00
	GPIOB->MODER |= (1 << 13) | (1 << 15);//PB6/7 :AF
	//GPIOA->OTYPER &= ~(1 << 2);//PB6 :PP
	GPIOB->OSPEEDR |= ((1 << 13)|(1 << 15));// High speed
	GPIOB->PUPDR &= ~((3 << 12)|(3 << 14));
	GPIOB->PUPDR |= (1 << 12) | (1 << 14);//PB6/7: Pull-up
	
	GPIOB->AFR[0] &= ~(0xFF << 24 );//Alternate function: PWM
	GPIOB->AFR[0] |= 7 << 24;
	GPIOB->AFR[0] |= 7 << 28; 
	
	//USART Init
	USART1->CR1 &= ~(1 << 15);//oversampling by 16
	USART1->CR1	|= 1 << 12;//1start,8data,1 stop
	USART1->CR1 |= (1 << 3) |(1 << 2);//Transmitter and receiver enable
	USART1->CR2 &= ~(3 << 12);//1 stop bit
	USART1->CR3 &= ~(1 << 10);//hardware flow disable
	
	//baud rate 
	float USARTDIV;
  unsigned int  DIV_M;
  unsigned int  DIV_F;
  USARTDIV=84000000/16/9600; 
  DIV_M =(u32) USARTDIV;
  DIV_F = (USARTDIV- DIV_M)*16; 
	
  USART1->BRR = DIV_M<<4 | DIV_F;//9600 baud rate
	
	//NVIC Init
	SCB->AIRCR = 0x05FA0500;//NVIC Group 2
	NVIC->IP[37] = 0xf0;
	
	//NVIC
	USART1->CR1 |= 1 << 5;//RXNE interupt enable
	NVIC->ISER[1] |= 1 << 5;
	
	//USART enable
	USART1->CR1 |= 1 << 13;
}

void Serial_SendByte(uint8_t Byte)
{
	//USART1->CR1 |= 1 << 3; //TE enable
	while (!(USART1->SR & 0x80 )){};//wait for TXE trun to 1 (Transmit data regester empty)
	USART1->DR = Byte & 0x1FF;
	//while (!(USART1->SR & 0x80 )){};//
}

void Serial_SendArray(uint8_t *Array, uint16_t Length)
{
	uint16_t i;
	for (i = 0; i < Length; i ++)
	{
		Serial_SendByte(Array[i]);
	}
}

void Serial_SendString(char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i ++)
	{
		Serial_SendByte(String[i]);
	}
}

uint32_t Serial_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;
	while (Y --)
	{
		Result *= X;
	}
	return Result;
}

void Serial_SendNumber(uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i ++)
	{
		Serial_SendByte(Number / Serial_Pow(10, Length - i - 1) % 10 + '0');
	}
}

/*********************************
int fputc(int ch, FILE *f)
{
	Serial_SendByte(ch);
	return ch;
}

void Serial_Printf(char *format, ...)
{
	char String[100];
	va_list arg;
	va_start(arg, format);
	vsprintf(String, format, arg);
	va_end(arg);
	Serial_SendString(String);
}


uint8_t Serial_GetRxFlag(void)
{
	if (Serial_RxFlag == 1)
	{
		Serial_RxFlag = 0;
		return 1;
	}
	return 0;
}

uint8_t Serial_GetRxData(void)
{
	return Serial_RxData;
}
**************************************/

void USART1_IRQHandler(void)
{
	if (USART1->SR & 0x20 )//RXEN:1 
	{
		Serial_RxData 
		= USART1->DR & 0x1FF;
		Serial_RxFlag = 1;
		USART1->SR &= ~(1 << 5);//clear RXNE
		Serial_SendByte(Serial_RxData);
	}
}



