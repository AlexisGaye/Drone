#include "stm32f4xx.h"
#include "Delay.h"
#include "MPU.h"
#include "MyI2C.h"
#include "HMC5883L.h"
#include "Serial.h"
#include "Moter.h"
#include "Receiver.h"

HMC_Data HMC_Data_Structrue;
MPU_Data MPU_Data_Structrue;
double Magangle;
int main(void)
{
	
	MPU6050_Init();
	HMC5883L_Init();
	Serial_Init();
	PWM_Init();
	Receiver_Init();
	while (1)
	{
		//HMC5883L_GetData(&HMC_Data_Structrue);
		//MPU6050_GetData(&MPU_Data_Structrue);
		//Magangle = HMC5883L_Magangle(HMC_Data_Structrue);
		//uint8_t ab = 2;
		//char a[7] = "HELLO0";
		//Serial_SendByte('F');
		//Delay_ms(1000);
		//Serial_SendArray( a , 6);
		
		
		
	}
}
