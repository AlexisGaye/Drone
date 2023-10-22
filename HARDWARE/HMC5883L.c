#include "stm32f4xx.h"                  // Device header
#include "MyI2C.h"
#include "HMC5883L.h"
#include "HMC5883LAdress.h"
#include <math.h>

#define HMC5883L_ADDRESS_W		0x3C //write adress
#define HMC5883L_ADDRESS_R		0x3D //read adress
void HMC5883L_WriteReg(uint8_t RegAddress, uint8_t Data)
{
	MyI2C_Start();
	MyI2C_SendByte(HMC5883L_ADDRESS_W);
	MyI2C_ReceiveAck();
	MyI2C_SendByte(RegAddress);
	MyI2C_ReceiveAck();
	MyI2C_SendByte(Data);
	MyI2C_ReceiveAck();
	MyI2C_Stop();
}

uint8_t HMC5883L_ReadReg(uint8_t RegAddress)
{
	uint8_t Data;
	
	MyI2C_Start();
	MyI2C_SendByte(HMC5883L_ADDRESS_W);
	MyI2C_ReceiveAck();
	MyI2C_SendByte(RegAddress);
	MyI2C_ReceiveAck();
	
	MyI2C_Start();
	MyI2C_SendByte(HMC5883L_ADDRESS_R);
	MyI2C_ReceiveAck();
	Data = MyI2C_ReceiveByte();
	MyI2C_SendAck(1);
	MyI2C_Stop();
	
	return Data;
}

int16_t HMC5883L_ReadTwoReg(uint8_t RegAddress)
{
	int16_t Data;
	
	MyI2C_Start();
	MyI2C_SendByte(HMC5883L_ADDRESS_W);
	MyI2C_ReceiveAck();
	MyI2C_SendByte(RegAddress);
	MyI2C_ReceiveAck();
	
	MyI2C_Start();
	MyI2C_SendByte(HMC5883L_ADDRESS_R);
	MyI2C_ReceiveAck();
	Data = MyI2C_ReceiveByte();
  MyI2C_SendAck(0);	
	Data = (MyI2C_ReceiveByte())|(Data << 8);
	MyI2C_SendAck(1);
	MyI2C_Stop();
	
	return Data;
}


void HMC5883L_Init()
{
	MyI2C_Init();
	
	HMC5883L_WriteReg(CONFIGA,0x70);
	HMC5883L_WriteReg(CONFIGB,0x20);
	HMC5883L_WriteReg(MODE,0x00);//continuely measure
}

void HMC5883L_GetData(HMC_Data *HMC_Data_Structure)
{
	
	HMC_Data_Structure->GSX = HMC5883L_ReadTwoReg(DATAX_M);
	HMC_Data_Structure->GSZ = HMC5883L_ReadTwoReg(DATAY_M);
	HMC_Data_Structure->GSY = HMC5883L_ReadTwoReg(DATAZ_M);
	/*****************************************
	DataH = HMC5883L_ReadReg(DATAX_M); 
	DataL = HMC5883L_ReadReg(DATAX_L); 
	HMC_Data_Structure->GSX = (DataH << 8) | DataL; 
	
	DataH = HMC5883L_ReadReg(DATAY_M);
	DataL = HMC5883L_ReadReg(DATAY_L);
	HMC_Data_Structure->GSY = (DataH << 8) | DataL;
	
	DataH = HMC5883L_ReadReg(DATAZ_M);
	DataL = HMC5883L_ReadReg(DATAZ_L);
	HMC_Data_Structure->GSZ = (DataH << 8) | DataL;
	******************************************/
}

double HMC5883L_Magangle(HMC_Data HMC_Data_Structure)
{
	double GaX = HMC_Data_Structure.GSX;
	double GaY = HMC_Data_Structure.GSY;
	double Magangle;
				if((GaX > 0)&&(GaY > 0)) Magangle = atan(GaY/GaX)*57;
			else if((GaX > 0)&&(GaY < 0)) Magangle = 360+atan(GaY/GaX)*57;
			else if((GaX == 0)&&(GaY > 0)) Magangle = 90;
			else if((GaX == 0)&&(GaY < 0)) Magangle = 270;
			else if(GaX < 0) Magangle = 180+atan(GaY/GaX)*57;
	return Magangle;
}

