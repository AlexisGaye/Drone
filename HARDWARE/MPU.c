#include "stm32f4xx.h"                  // Device header
#include "MyI2C.h"
#include "MPU.h"
#include "MPU6050Adress.h"

#define MPU6050_ADDRESS		0xD0

void MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data)
{
	MyI2C_Start();
	MyI2C_SendByte(MPU6050_ADDRESS);
	MyI2C_ReceiveAck();
	MyI2C_SendByte(RegAddress);
	MyI2C_ReceiveAck();
	MyI2C_SendByte(Data);
	MyI2C_ReceiveAck();
	MyI2C_Stop();
}

uint8_t MPU6050_ReadReg(uint8_t RegAddress)
{
	uint8_t Data;
	
	MyI2C_Start();
	MyI2C_SendByte(MPU6050_ADDRESS);
	MyI2C_ReceiveAck();
	MyI2C_SendByte(RegAddress);
	MyI2C_ReceiveAck();
	
	MyI2C_Start();
	MyI2C_SendByte(MPU6050_ADDRESS | 0x01);
	MyI2C_ReceiveAck();
	Data = MyI2C_ReceiveByte();
	MyI2C_SendAck(1);
	MyI2C_Stop();
	
	return Data;
}


int16_t MPU6050_ReadTwoReg(uint8_t RegAddress)
{
	int16_t Data;
	
	MyI2C_Start();
	MyI2C_SendByte(MPU6050_ADDRESS);
	MyI2C_ReceiveAck();
	MyI2C_SendByte(RegAddress);
	MyI2C_ReceiveAck();
	
	MyI2C_Start();
	MyI2C_SendByte(MPU6050_ADDRESS | 0x01);
	MyI2C_ReceiveAck();
	Data = MyI2C_ReceiveByte();
  MyI2C_SendAck(0);	
	Data = (MyI2C_ReceiveByte())|(Data << 8);
	MyI2C_SendAck(1);
	MyI2C_Stop();
	
	return Data;
}

void MPU6050_ReadAllReg(uint8_t RegAddress, int16_t* data,uint8_t count)
{
	int16_t Data;
	uint8_t i = 0;
	
	MyI2C_Start();
	MyI2C_SendByte(MPU6050_ADDRESS);
	MyI2C_ReceiveAck();
	MyI2C_SendByte(RegAddress);
	MyI2C_ReceiveAck();
	
	MyI2C_Start();
	MyI2C_SendByte(MPU6050_ADDRESS | 0x01);
	MyI2C_ReceiveAck();
	while(i < count)
	{
	  Data = MyI2C_ReceiveByte();
    MyI2C_SendAck(0);	
	  Data = (MyI2C_ReceiveByte())|(Data << 8);
		data[i] = Data;
		i++;
		if(i == (count - 1))
		{
	    MyI2C_SendAck(1);
		}
		else
		{
		  MyI2C_SendAck(0);
		}
		
	}
	MyI2C_Stop();
}


void MPU6050_Init(void)
{
	MyI2C_Init();
	MPU6050_WriteReg(MPU6050_PWR_MGMT_1, 0x01);
	MPU6050_WriteReg(MPU6050_PWR_MGMT_2, 0x00);
	MPU6050_WriteReg(MPU6050_SMPLRT_DIV, 0x09);
	MPU6050_WriteReg(MPU6050_CONFIG, 0x06);
	MPU6050_WriteReg(MPU6050_GYRO_CONFIG, 0x18);
	MPU6050_WriteReg(MPU6050_ACCEL_CONFIG, 0x18);
	
	//config MPU6050's regester to hang the iic to the main bus
	MPU6050_WriteReg(MPU6050_USER_CTRL,0x00);
	MPU6050_WriteReg(MPU6050_INT_PIN_CFG,0x02);
}

uint8_t MPU6050_GetID(void)
{
	return MPU6050_ReadReg(MPU6050_WHO_AM_I);
}

void MPU6050_GetData(MPU_Data *MPU_Data_Structrue)
{
	int16_t ACCEL_Data[3];
	int16_t GYRO_Data[3];
	
	MPU6050_ReadAllReg(MPU6050_ACCEL_XOUT_H, ACCEL_Data,3);
	MPU6050_ReadAllReg(MPU6050_GYRO_XOUT_H, GYRO_Data,3);
	
	MPU_Data_Structrue->AccX = ACCEL_Data[0];
	MPU_Data_Structrue->AccY = ACCEL_Data[1];
	MPU_Data_Structrue->AccZ = ACCEL_Data[2];
	MPU_Data_Structrue->GyroX = GYRO_Data[0];
	MPU_Data_Structrue->GyroY = GYRO_Data[1];
	MPU_Data_Structrue->GyroZ = GYRO_Data[2];
	
	
	/*****************************************************************
	MPU_Data_Structrue->AccX = MPU6050_ReadTwoReg(MPU6050_ACCEL_XOUT_H);
	MPU_Data_Structrue->AccY = MPU6050_ReadTwoReg(MPU6050_ACCEL_YOUT_H);;
	MPU_Data_Structrue->AccZ = MPU6050_ReadTwoReg(MPU6050_ACCEL_ZOUT_H);
	MPU_Data_Structrue->GyroX = MPU6050_ReadTwoReg(MPU6050_GYRO_XOUT_H);
	MPU_Data_Structrue->GyroY = MPU6050_ReadTwoReg(MPU6050_GYRO_YOUT_H);
	MPU_Data_Structrue->GyroZ = MPU6050_ReadTwoReg(MPU6050_GYRO_ZOUT_H);
	*******************************************************************/
	
	/*******************************************
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_L);
	MPU_Data_Structrue->AccX = (DataH << 8) | DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_L);
	MPU_Data_Structrue->AccY = (DataH << 8) | DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_L);
	MPU_Data_Structrue->AccZ = (DataH << 8) | DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_GYRO_XOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_GYRO_XOUT_L);
	MPU_Data_Structrue->GyroX = (DataH << 8) | DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_GYRO_YOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_GYRO_YOUT_L);
	MPU_Data_Structrue->GyroY = (DataH << 8) | DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_L);
	MPU_Data_Structrue->GyroZ = (DataH << 8) | DataL;
	***********************************************/
}
