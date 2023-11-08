#ifndef __MOTER_H
#define __MOTER_H

extern int16_t motor[4];

void PWM_Init();
void ChangeDutyCycle(int Duty);
void ChangeAccelrator(int16_t Motor[]);

#endif 
