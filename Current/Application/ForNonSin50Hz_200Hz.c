#include "ForNonSin50Hz_200Hz.h"
#include "Config.h"
extern struct InfoStruct Info;
extern struct NonSinStruct NonSin;
#include "WB_LCD.h"
void NonSinSearcher(void){
		TIM3_Int_Init(((u32)(1000000/(2000*1.024f)))-1,36-1);//31//124 20.58//13
		while(Info.cnt<(LEN-1)){};
		cr4_fft_1024_stm32(Info.FArray, Info.buff, LEN);
		GetPowerMag();
//			for(j=0;j<LEN/2;j++){
//				SDS[0]=(int16_t)((Info.buff[j]>>16));
//				
//				Plot(SDS);
//			}
		//while(1);
		NonSin.max_index=FindMax((Info.lBufMagArray+1),LEN/2-1);
		NonSin.fun_amp=(uint16_t)Info.lBufMagArray[NonSin.max_index];
		NonSin.fun_freq=Info.omega[NonSin.max_index];
		//LCD_ShowNum(80, 20,Sin.fun_freq,6,RED, WHITE);
		/***********初次采样结束，调节采样频率，进行精密采样****/
		TIM_DeInit(TIM3);
		Info.SampleFreq=(uint16_t)(NonSin.fun_freq*2*1.024f);
		Info.cnt=0;
		TIM3_Int_Init(((u32)1000000/Info.SampleFreq)-1,36-1);
		while(Info.cnt<(LEN-1)){};
		cr4_fft_1024_stm32(Info.FArray, Info.buff, LEN);
		GetPowerMag();
		NonSin.max_index=FindMax((Info.lBufMagArray+1),LEN/2-1);
		NonSin.fun_amp=(uint16_t)Info.lBufMagArray[NonSin.max_index];
		NonSin.fun_freq=Info.omega[NonSin.max_index];
//		LCD_DisplayStr(0,0,(u8*)"10*FunFreq",RED,WHITE);
//		LCD_ShowNum(100, 0,NonSin.fun_freq*10.0f,5,RED, WHITE);
//		LCD_DisplayStr(0,20,(u8*)"SampleFreq",RED,WHITE);
//		LCD_ShowNum(90, 20,Info.SampleFreq,6,RED, WHITE);
		Info.cnt=0;
		NonSin.harmonic_1_freq=NonSin.fun_freq;
		NonSin.harmonic_2_freq=NonSin.harmonic_1_freq*2.0f;
		NonSin.harmonic_3_freq=NonSin.harmonic_1_freq*3.0f;
		NonSin.harmonic_4_freq=NonSin.harmonic_1_freq*4.0f;
		NonSin.harmonic_5_freq=NonSin.harmonic_1_freq*5.0f;
		/*****************n次谐波测频完成,开始测基波幅值*********/
		TIM_DeInit(TIM3);
		Info.SampleFreq=(uint16_t)(NonSin.fun_freq*COE*1.024f);
		Info.cnt=0;
		TIM3_Int_Init(((u32)1000000/Info.SampleFreq)-1,36-1);
		while(Info.cnt<(LEN-1)){};
		cr4_fft_1024_stm32(Info.FArray, Info.buff, LEN);
		GetPowerMag();
		NonSin.max_index=FindRangeMax((Info.lBufMagArray+1),(uint16_t)(0),(uint16_t)(NonSin.fun_freq*2));
		NonSin.fun_amp =(uint16_t)(sqrt(getAmp(NonSin.max_index,5)) / LEN * 65536 * 3.3f/4096.0f*2000.0f);
		NonSin.harmonic_1_amp=NonSin.fun_amp;
			
		LCD_DisplayStr(0,0,(u8*)"NonSinFreq1",RED,WHITE);
		LCD_ShowNum(80, 0,NonSin.fun_freq*10.0f,5,RED, WHITE);
		LCD_DisplayStr(130,0,(u8*)"Amp1",RED,WHITE);
		LCD_ShowNum(180, 0,NonSin.fun_amp,5,RED, WHITE);
		// 	while(1);
		/*****************开始测2次谐波幅值****************/
		TIM_DeInit(TIM3);
		Info.SampleFreq=(uint16_t)(NonSin.harmonic_2_freq*COE*1.024f);
		Info.cnt=0;
		TIM3_Int_Init(((u32)1000000/Info.SampleFreq)-1,36-1);
		while(Info.cnt<(LEN-1)){};
		cr4_fft_1024_stm32(Info.FArray, Info.buff, LEN);
		GetPowerMag();
		NonSin.max_index=FindRangeMax((Info.lBufMagArray+1),(uint16_t)(NonSin.fun_freq*1),(uint16_t)(NonSin.fun_freq*3));
		NonSin.harmonic_2_amp =(uint16_t)(sqrt(getAmp(NonSin.max_index,5)) / LEN * 65536 * 3.3f/4096.0f*2000.0f);
		LCD_DisplayStr(0,20,(u8*)"NonSinFreq1",RED,WHITE);
		LCD_ShowNum(80, 20,NonSin.harmonic_2_freq*10.0f,5,RED, WHITE);
		LCD_DisplayStr(130,20,(u8*)"Amp2",RED,WHITE);
		LCD_ShowNum(180,20,NonSin.harmonic_2_amp,5,RED, WHITE);		
		/*******************开始测3次谐波幅值************/
		TIM_DeInit(TIM3);
		Info.SampleFreq=(uint16_t)(NonSin.harmonic_3_freq*COE*1.024f);
		Info.cnt=0;
		TIM3_Int_Init(((u32)1000000/Info.SampleFreq)-1,36-1);
		while(Info.cnt<(LEN-1)){};
		cr4_fft_1024_stm32(Info.FArray, Info.buff, LEN);
		GetPowerMag();
		NonSin.max_index=FindRangeMax((Info.lBufMagArray+1),(uint16_t)(NonSin.fun_freq*2),(uint16_t)(NonSin.fun_freq*4));
		NonSin.harmonic_3_amp =(uint16_t)(sqrt(getAmp(NonSin.max_index,5)) / LEN * 65536 * 3.3f/4096.0f*2000.0f);
		LCD_DisplayStr(0,40,(u8*)"NonSinFreq1",RED,WHITE);
		LCD_ShowNum(80, 40,NonSin.harmonic_3_freq*10.0f,5,RED, WHITE);
		LCD_DisplayStr(130,40,(u8*)"Amp3",RED,WHITE);
		LCD_ShowNum(180,40,NonSin.harmonic_3_amp,5,RED, WHITE);
		/*****************开始测4次谐波幅值******************/
		TIM_DeInit(TIM3);
		Info.SampleFreq=(uint16_t)(NonSin.harmonic_4_freq*COE*1.024f);
		Info.cnt=0;
		TIM3_Int_Init(((u32)1000000/Info.SampleFreq)-1,36-1);
		while(Info.cnt<(LEN-1)){};
		cr4_fft_1024_stm32(Info.FArray, Info.buff, LEN);
		GetPowerMag();
		NonSin.max_index=FindRangeMax((Info.lBufMagArray+1),(uint16_t)(NonSin.fun_freq*3),(uint16_t)(NonSin.fun_freq*5));
		NonSin.harmonic_4_amp =(uint16_t)(sqrt(getAmp(NonSin.max_index,5)) / LEN * 65536 * 3.3f/4096.0f*2000.0f);
		LCD_DisplayStr(0,60,(u8*)"NonSinFreq1",RED,WHITE);
		LCD_ShowNum(80, 60,NonSin.harmonic_4_freq*10.0f,5,RED, WHITE);
		LCD_DisplayStr(130,60,(u8*)"Amp4",RED,WHITE);
		LCD_ShowNum(180,60,NonSin.harmonic_4_amp,5,RED, WHITE);
		/*******************开始测5次谐波幅值*************/
		TIM_DeInit(TIM3);
		Info.SampleFreq=(uint16_t)(NonSin.harmonic_5_freq*COE*1.024f);
		Info.cnt=0;
		TIM3_Int_Init(((u32)1000000/Info.SampleFreq)-1,36-1);
		while(Info.cnt<(LEN-1)){};
		cr4_fft_1024_stm32(Info.FArray, Info.buff, LEN);
		GetPowerMag();
		NonSin.max_index=FindRangeMax((Info.lBufMagArray+1),(uint16_t)(NonSin.fun_freq*4),(uint16_t)(NonSin.fun_freq*6));
		NonSin.harmonic_5_amp =(uint16_t)(sqrt(getAmp(NonSin.max_index,5)) / LEN * 65536 * 3.3f/4096.0f*2000.0f);
		LCD_DisplayStr(0,80,(u8*)"NonSinFreq1",RED,WHITE);
		LCD_ShowNum(80, 80,NonSin.harmonic_5_freq*10.0f,5,RED, WHITE);
		LCD_DisplayStr(130,80,(u8*)"Amp5",RED,WHITE);
		LCD_ShowNum(180,80,NonSin.harmonic_5_amp,5,RED, WHITE);
		/***********************幅值测量完成，显示输出！***************/
//		LCD_DisplayStr(0,0,(u8*)"NonSinFreq1",RED,WHITE);
//		LCD_ShowNum(80, 0,NonSin.fun_freq*10.0f,5,RED, WHITE);
//		LCD_DisplayStr(130,0,(u8*)"NonSinAmp1",RED,WHITE);
//		LCD_ShowNum(180, 0,NonSin.fun_amp,5,RED, WHITE);
//		LCD_DisplayStr(0,0,(u8*)"NonSinFreq2",RED,WHITE);
//		LCD_ShowNum(80, 0,NonSin.harmonic_2_freq,5,RED, WHITE);
//		LCD_DisplayStr(130,0,(u8*)"NonSinFreq3",RED,WHITE);
//		LCD_ShowNum(180, 0,NonSin.harmonic_3_freq,5,RED, WHITE);
//		LCD_DisplayStr(0,0,(u8*)"NonSinFreq4",RED,WHITE);
//		LCD_ShowNum(80, 0,NonSin.harmonic_4_freq,5,RED, WHITE);
//		LCD_DisplayStr(130,0,(u8*)"NonSinFreq5",RED,WHITE);
//		LCD_ShowNum(180, 0,NonSin.harmonic_5_freq,5,RED, WHITE);
}
