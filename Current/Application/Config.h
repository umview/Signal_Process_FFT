#ifndef _CONFIG_
#define _CONFIG_
#include "Gpio.h"
#include "delay.h"
#include "USART.h"
#include "ADC.h"
#include "SDS.h"
#include "fsmc_sram.h"
#include "Timer.h"
#include "stm32_dsp.h"
#include "math.h"
#include "ForSin50Hz_1KHz.h"
#include "ForNonSin50Hz_200Hz.h"
#include "Function.h"
//#include "WB_LCD.h"
//#include "HZ.h"
//#include "hebut.h"
/********************************************************************/
#define LEN 1024
#define InitSampleFreq 2500
#define SearchFreqWidth 300
#define COE 5
struct InfoStruct {
	long buff[LEN];
	uint16_t cnt;
	uint16_t SampleFreq;
	long FArray[LEN/2];           //傅里叶数据
	long lBufMagArray[LEN/2];     //各次谐波幅值
	float omega[LEN/2];                  //计算基波频率
	double Amplitude[LEN/2];      //计算幅值
};
struct SinStruct {
	uint16_t max_index;
	uint16_t fun_amp;
	float fun_freq;
	float current;
};
struct NonSinStruct {
	uint16_t max_index;
	uint16_t fun_amp;
	uint16_t harmonic_1_amp;
	uint16_t harmonic_2_amp;
	uint16_t harmonic_3_amp;
	uint16_t harmonic_4_amp;
	uint16_t harmonic_5_amp;
	float fun_freq;
	float harmonic_1_freq;
	float harmonic_2_freq;
	float harmonic_3_freq;
	float harmonic_4_freq;
	float harmonic_5_freq;
};
#endif
