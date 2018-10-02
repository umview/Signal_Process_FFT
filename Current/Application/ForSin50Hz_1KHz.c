#include "ForSin50Hz_1KHz.h"
#include "Config.h"
#include "Function.h"
#include "WB_LCD.h"
extern float SDS[4];
//#include "HZ.h"
extern struct InfoStruct Info;
extern struct SinStruct Sin;
extern   TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
uint16_t j=0;
float sum=0.0f,X=0.0f,Y=0.0f;
void SearchFunFreqAmp(void){
		TIM3_Int_Init(((u32)1000000/InitSampleFreq)-1,36-1);//31//124 20.58//13
		while(Info.cnt<(LEN-1)){};
		cr4_fft_1024_stm32(Info.FArray, Info.buff, LEN);
		GetPowerMag();
//			for(j=0;j<LEN/2;j++){
//				SDS[0]=(int16_t)((Info.buff[j]>>16));
//				
//				Plot(SDS);
//			}
		Sin.max_index=FindMax((Info.lBufMagArray+1),LEN/2-1);
		Sin.fun_amp=(uint16_t)Info.lBufMagArray[Sin.max_index];
		Sin.fun_freq=Info.omega[Sin.max_index];
		//LCD_ShowNum(80, 20,Sin.fun_freq,6,RED, WHITE);
		/***********初次采样结束，调节采样频率，进行精密采样****/
		TIM_DeInit(TIM3);
		Info.SampleFreq=(uint16_t)(Sin.fun_freq*2*1.024f);
		Info.cnt=0;
		TIM3_Int_Init(((u32)1000000/Info.SampleFreq)-1,36-1);
		while(Info.cnt<(LEN-1)){};
		cr4_fft_1024_stm32(Info.FArray, Info.buff, LEN);
		GetPowerMag();
		Sin.max_index=FindMax((Info.lBufMagArray+1),LEN/2-1);
		Sin.fun_amp=(uint16_t)Info.lBufMagArray[Sin.max_index];
		Sin.fun_freq=Info.omega[Sin.max_index]+0.3;//static error
		if(Sin.fun_freq<49.4f){
//			Info.cnt=0;
//			TIM3_Int_Init(((u32)1000000/Info.SampleFreq)-1,36-1);
//			while(Info.cnt<(LEN-1)){};
//			cr4_fft_1024_stm32(Info.FArray, Info.buff, LEN);
//			GetPowerMag();
//			Sin.max_index=FindMax((Info.lBufMagArray+1),LEN/2-1);
//			Sin.fun_amp=(uint16_t)Info.lBufMagArray[Sin.max_index];
//			Sin.fun_freq=Info.omega[Sin.max_index]+0.3;//static error
		}

		LCD_DisplayStr(0,0,(u8*)"10*FunFreq",RED,WHITE);
		LCD_ShowNum(100, 0,Sin.fun_freq*10.0f,5,RED, WHITE);
		LCD_DisplayStr(0,20,(u8*)"SampleFreq",RED,WHITE);
		LCD_ShowNum(90, 20,Info.SampleFreq,6,RED, WHITE);
		Info.cnt=0;
		//TIM_Cmd(TIM3,ENABLE);
		/****************测频完成，开始测幅****************/
		TIM_DeInit(TIM3);
		Info.SampleFreq=(uint16_t)(Sin.fun_freq*COE*1.024f);
		Info.cnt=0;
		TIM3_Int_Init(((u32)1000000/Info.SampleFreq)-1,36-1);
		while(Info.cnt<(LEN-1)){};
		cr4_fft_1024_stm32(Info.FArray, Info.buff, LEN);
		GetPowerMag();
		Sin.max_index=FindMax((Info.lBufMagArray+1),LEN/2-1);
		/********计算频谱尖峰周围分量**************/

		/************计算实际幅值*******************/
		Sin.fun_amp =(uint16_t)(sqrt(getAmp(Sin.max_index,20)) / LEN * 65536 * 3.3f/4096.0f*2000.0f);//20
		if(Sin.fun_freq<200.0f)Sin.fun_amp+=555;
		else Sin.fun_amp+=350;
		Sin.current=(float)((Sin.fun_amp+100)/100*0.131);
		LCD_DisplayStr(0,40,(u8*)"SinAmp",RED,WHITE);
		LCD_ShowNum(90, 40,Sin.fun_amp,6,RED, WHITE);
		LCD_DisplayStr(0,60,(u8*)"Current",RED,WHITE);
		LCD_ShowNum(90, 60,Sin.current,6,RED, WHITE);
}
uint16_t FindMax(long *a,uint16_t n){
	uint16_t i=0;
	uint16_t rec=0;
	long max=*a;
	//uint16_t max_num=0;
	for(i=1;i<n;i++){
		if(*(a+i)>max){
			max=*(a+i);
			rec=i;
		}
	}
	return rec;
}
uint16_t FindRangeMax(long *a,uint16_t start,uint16_t stop){
	uint16_t i=start;
	uint16_t rec=start;
	long max=*(a+start);
	//uint16_t max_num=0;
	for(i=start;i<stop;i++){
		if(*(a+i)>max){
			max=*(a+i);
			rec=i;
		}
	}
	return rec;
}
int16_t ki=0;
float getAmp(uint16_t max_index,uint16_t deep){
		sum=0.0f;

		for(ki=0;ki<=deep;ki++){
			X = LEN * ((float)(((Info.FArray[max_index-ki] << 16) >> 16))/ 32768);
			Y = LEN * ((float)((Info.FArray[max_index-ki] >> 16)))/ 32768;
			sum+=X*X+Y*Y;
			X = LEN * ((float)(((Info.FArray[max_index+ki] << 16) >> 16))/ 32768);
			Y = LEN * ((float)((Info.FArray[max_index+ki] >> 16)))/ 32768;
			sum+=X*X+Y*Y;
		}
		return sum;
//        X = LEN * ((float)(((Info.FArray[max_index-10] << 16) >> 16))/ 32768);
//        Y = LEN * ((float)((Info.FArray[max_index-10] >> 16)))/ 32768;
//		sum+=X*X+Y*Y;
//        X = LEN * ((float)(((Info.FArray[max_index-9] << 16) >> 16))/ 32768);
//        Y = LEN * ((float)((Info.FArray[max_index-9] >> 16)))/ 32768;
//		sum+=X*X+Y*Y;
//        X = LEN * ((float)(((Info.FArray[max_index-8] << 16) >> 16))/ 32768);
//        Y = LEN * ((float)((Info.FArray[max_index-8] >> 16)))/ 32768;
//		sum+=X*X+Y*Y;
//        X = LEN * ((float)(((Info.FArray[max_index-7] << 16) >> 16))/ 32768);
//        Y = LEN * ((float)((Info.FArray[max_index-7] >> 16)))/ 32768;
//		sum+=X*X+Y*Y;
//        X = LEN * ((float)(((Info.FArray[max_index-6] << 16) >> 16))/ 32768);
//        Y = LEN * ((float)((Info.FArray[max_index-6] >> 16)))/ 32768;
//		sum+=X*X+Y*Y;
//        X = LEN * ((float)(((Info.FArray[max_index-5] << 16) >> 16))/ 32768);
//        Y = LEN * ((float)((Info.FArray[max_index-5] >> 16)))/ 32768;
//		sum+=X*X+Y*Y;
//        X = LEN * ((float)(((Info.FArray[max_index-4] << 16) >> 16))/ 32768);
//        Y = LEN * ((float)((Info.FArray[max_index-4] >> 16)))/ 32768;
//		sum+=X*X+Y*Y;
//        X = LEN * ((float)(((Info.FArray[max_index-3] << 16) >> 16))/ 32768);
//        Y = LEN * ((float)((Info.FArray[max_index-3] >> 16)))/ 32768;
//		sum+=X*X+Y*Y;
//        X = LEN * ((float)(((Info.FArray[max_index-2] << 16) >> 16))/ 32768);
//        Y = LEN * ((float)((Info.FArray[max_index-2] >> 16)))/ 32768;
//		sum+=X*X+Y*Y;
//        X = LEN * ((float)(((Info.FArray[max_index-1] << 16) >> 16))/ 32768);
//        Y = LEN * ((float)((Info.FArray[max_index-1] >> 16)))/ 32768;
//		sum+=X*X+Y*Y;
//        X = LEN * ((float)(((Info.FArray[max_index] << 16) >> 16))/ 32768);
//        Y = LEN * ((float)((Info.FArray[max_index] >> 16)))/ 32768;
//		sum+=X*X+Y*Y;
//        X = LEN * ((float)(((Info.FArray[max_index+3] << 16) >> 16))/ 32768);
//        Y = LEN * ((float)((Info.FArray[max_index+3] >> 16)))/ 32768;
//		sum+=X*X+Y*Y;
//        X = LEN * ((float)(((Info.FArray[max_index+2] << 16) >> 16))/ 32768);
//        Y = LEN * ((float)((Info.FArray[max_index+2] >> 16)))/ 32768;
//		sum+=X*X+Y*Y;
//        X = LEN * ((float)(((Info.FArray[max_index+1] << 16) >> 16))/ 32768);
//        Y = LEN * ((float)((Info.FArray[max_index+1] >> 16)))/ 32768;
//		sum+=X*X+Y*Y;
//        X = LEN * ((float)(((Info.FArray[max_index+7] << 16) >> 16))/ 32768);
//        Y = LEN * ((float)((Info.FArray[max_index+7] >> 16)))/ 32768;
//		sum+=X*X+Y*Y;
//        X = LEN * ((float)(((Info.FArray[max_index+6] << 16) >> 16))/ 32768);
//        Y = LEN * ((float)((Info.FArray[max_index+6] >> 16)))/ 32768;
//		sum+=X*X+Y*Y;
//        X = LEN * ((float)(((Info.FArray[max_index+5] << 16) >> 16))/ 32768);
//        Y = LEN * ((float)((Info.FArray[max_index+5] >> 16)))/ 32768;
//		sum+=X*X+Y*Y;
//        X = LEN * ((float)(((Info.FArray[max_index+4] << 16) >> 16))/ 32768);
//        Y = LEN * ((float)((Info.FArray[max_index+4] >> 16)))/ 32768;
//		sum+=X*X+Y*Y;
//        X = LEN * ((float)(((Info.FArray[max_index+10] << 16) >> 16))/ 32768);
//        Y = LEN * ((float)((Info.FArray[max_index+10] >> 16)))/ 32768;
//		sum+=X*X+Y*Y;
//        X = LEN * ((float)(((Info.FArray[max_index+9] << 16) >> 16))/ 32768);
//        Y = LEN * ((float)((Info.FArray[max_index+9] >> 16)))/ 32768;
//		sum+=X*X+Y*Y;
//        X = LEN * ((float)(((Info.FArray[max_index+8] << 16) >> 16))/ 32768);
//        Y = LEN * ((float)((Info.FArray[max_index+8] >> 16)))/ 32768;
//		sum+=X*X+Y*Y;
//		return sum;
}

//void SelectSort(int *a, int n) {
//    for (int i = 0; i < n; i++)
//    {
//        int key = i;    //    ????????????????
//        for (int j = i + 1; j < n; j++) {
//            if (a[j] < a[key]) {    
//                key = j;    //    ?????????
//            }
//        }
//            if (key != i)
//            {
//                int tmp = a[key]; a[key] = a[i]; a[i] = tmp;    //    ?????
//            }
//        
//    }
//}
