#ifndef __HMC5883L_H_
#define __HMC5883L_H_

#include "stdint.h"

typedef struct HMC_Data
{
	int16_t GSX;
	int16_t GSY;
	int16_t GSZ;
}HMC_Data;

void HMC5883L_WriteReg(uint8_t RegAddress, uint8_t Data);
uint8_t HMC5883L_ReadReg(uint8_t RegAddress);
void HMC5883L_Init();
void HMC5883L_GetData(HMC_Data *HMC_Data_Structure);
double HMC5883L_Magangle(HMC_Data HMC_Data_Structure);

#endif
