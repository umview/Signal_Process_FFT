#ifndef _ADC_H_
#define _ADC_H_
#include "stm32f10x.h"



#define ADC1_DR_Address    ((u32)0x4001244C)
//#define USE_DMA                            //È¥µô×¢ÊÍ,Ê¹ÓÃDMA 

extern  vu16 ADCValue;
void RCC_Config(void);
void ADC1_Init(void);
#endif
