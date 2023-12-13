#include "stm32f4xx.h"
#include "Delay.h"
#include "MPU.h"
#include "MyI2C.h"
#include "HMC5883L.h"
#include "Serial.h"
#include "Moter.h"
#include "Receiver.h"


int main(void)
{
	
	MPU6050_Init();
	HMC5883L_Init();
	Serial_Init();
	Receiver_Init();
	PWM_Init();
	
	uint32_t i = 0;
	while (1)
	{
		
		if(receiver.yes == 1)
		{
			ChangeAccelrator(motor);
		}
		if(i > 5000000)
		{
		MPU6050_GetData(&MPU_Data_Structure);
		HMC5883L_GetData(&HMC_Data_Structure);
		Serial_SendArray("REC:",4);
		Serial_SendNumber(receiver.ch[1],4);
		Serial_SendByte(' ');
		Serial_SendNumber(receiver.ch[2],4);
		Serial_SendByte(' ');
		Serial_SendNumber(receiver.ch[3],4);
		Serial_SendByte('\n');
		Serial_SendArray("MPU:",4);
		Serial_SendNumber(MPU_Data_Structure.AccX,4);
		Serial_SendByte(' ');
		Serial_SendNumber(MPU_Data_Structure.AccY,4);
		Serial_SendByte(' ');
		Serial_SendNumber(MPU_Data_Structure.AccZ,4);
		Serial_SendByte('\n');
		i = 0;
		}
		i++;
	}
}
