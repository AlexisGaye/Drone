#ifndef __REC_H
#define __REC_H



typedef struct Rec
{
	uint8_t now;
	uint8_t yes;
	int16_t ch[9];
}Rec;

extern Rec receiver;

void Receiver_Init(void);

#endif
