#ifndef _SIN_
#define _SIN_
#include "Gpio.h"
#include "delay.h"
#include "USART.h"
#include "ADC.h"
#include "SDS.h"
#include "fsmc_sram.h"
#include "Timer.h"
#include "stm32_dsp.h"
#include "math.h"
#include "Config.h"
/*******************************************/
void SearchFunFreqAmp(void);
float getAmp(uint16_t max_index,uint16_t deep);
void SelectSort(int *a, int n);
uint16_t FindMax(long *a,uint16_t n);
uint16_t FindRangeMax(long *a,uint16_t start,uint16_t stop);
#endif
