/*******************************************************************************
  * Company: Wang Electronic Technology Co., Ltd.
  ******************************************************************************
  * 文件名称：main.c
  * 功能说明：红牛板-ADC-模数转换实验
  * 版    本：V1.0
  * 作    者：openmcu666	
  * 日    期: 2016-05-04
********************************************************************************
  * 文件名称：
  * 功能说明：
  * 版    本：
  * 更新作者:	
  * 日    期：
  * 更新原因：
********************************************************************************/
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
#include "Config.h"
#include "WB_LCD.h"
#include "HZ.h"
//#include "..\Libraries\STM32F10x_DSP_Lib\stm32_dsp.h"
/*****************************************************************************
**   Main Function  main()
******************************************************************************/
extern float SDS[4];

//extern const unsigned char gImage_hebut[153600];

struct SinStruct Sin;
struct NonSinStruct NonSin;
struct InfoStruct Info;

void LOOP(void);
//void cr4_fft_1024_stm32(void *pssOUT, void *pssIN, uint16_t Nbin);
//void hebut()
//{
//	//LCD_DrawPicture(0,0,240,320,(u8*)gImage_hebut);
//	LCD_Fill(0,0, 70, 20, WHITE);
//	LCD_Fill(70,0, 80, 15, WHITE);

//}
void shownumber()
{
		WriteOneHz (10, 5, (u8 *)HZ[0], WHITE, BLACK);
 		WriteOneHz (26, 5, (u8 *)HZ[1], WHITE, BLACK);    //电流
		WriteOneHz (42, 5, (u8 *)HZ[13], WHITE, BLACK); 
		//LCD_ShowNum(58,5,u32 num,u8 len, WHITE, BLACK);

	   
	    WriteOneHz (10, 25, (u8 *)HZ[0], WHITE, BLACK);
	   	WriteOneHz (26, 25, (u8 *)HZ[2], WHITE, BLACK);   //电压
	   	WriteOneHz (42, 25, (u8 *)HZ[13], WHITE, BLACK); 
	   //LCD_ShowNum(58,25,u32 num,u8 len, WHITE, BLACK);
	   
	   	WriteOneHz (10, 45, (u8 *)HZ[5], WHITE, BLACK);
	   	WriteOneHz (26, 45, (u8 *)HZ[6], WHITE, BLACK);
	   	WriteOneHz (42, 45, (u8 *)HZ[3], WHITE, BLACK);
	   	WriteOneHz (58, 45, (u8 *)HZ[4], WHITE, BLACK);    //基波频率
		WriteOneHz (74, 45, (u8 *)HZ[13], WHITE, BLACK); 
		//LCD_ShowNum(90,45,u32 num,u8 len, WHITE, BLACK);	   
	   
	   	WriteOneHz (10, 65, (u8 *)HZ[9], WHITE, BLACK);
	   	WriteOneHz (26, 65, (u8 *)HZ[7], WHITE, BLACK);
	    WriteOneHz (42, 65, (u8 *)HZ[8], WHITE, BLACK);
	   	WriteOneHz (58, 65, (u8 *)HZ[6], WHITE, BLACK);         //二
	    WriteOneHz (74, 65, (u8 *)HZ[3], WHITE, BLACK);  
	   	WriteOneHz (90, 65, (u8 *)HZ[4], WHITE, BLACK);  
			WriteOneHz (106, 65, (u8 *)HZ[13], WHITE, BLACK);  
		//LCD_ShowNum(122,65,u32 num,u8 len, WHITE, BLACK);
		
	   	WriteOneHz (10, 85, (u8 *)HZ[10], WHITE, BLACK);
	   	WriteOneHz (26, 85, (u8 *)HZ[7], WHITE, BLACK);
	   	WriteOneHz (42, 85, (u8 *)HZ[8], WHITE, BLACK);
	   	WriteOneHz (58, 85, (u8 *)HZ[6], WHITE, BLACK);  
		WriteOneHz (74, 85, (u8 *)HZ[3], WHITE, BLACK);  		//三
	    WriteOneHz (90, 85, (u8 *)HZ[4], WHITE, BLACK); 
	    WriteOneHz (106, 85, (u8 *)HZ[13], WHITE, BLACK);
		//LCD_ShowNum(122,85,u32 num,u8 len, WHITE, BLACK);		

	   
	   	WriteOneHz (10, 105, (u8 *)HZ[11], WHITE, BLACK);
	   	WriteOneHz (26, 105, (u8 *)HZ[7], WHITE, BLACK);
	   	WriteOneHz (42, 105, (u8 *)HZ[8], WHITE, BLACK);
	   	WriteOneHz (58, 105, (u8 *)HZ[6], WHITE, BLACK);        //四
		WriteOneHz (74, 105, (u8 *)HZ[3], WHITE, BLACK); 
		WriteOneHz (90, 105, (u8 *)HZ[4], WHITE, BLACK);
		WriteOneHz (106, 105, (u8 *)HZ[13], WHITE, BLACK);
		//LCD_ShowNum(122,105,u32 num,u8 len, WHITE, BLACK);
		
	   	WriteOneHz (10, 125, (u8 *)HZ[12], WHITE, BLACK);
	   	WriteOneHz (26, 125, (u8 *)HZ[7], WHITE, BLACK);
	   	WriteOneHz (42, 125, (u8 *)HZ[8], WHITE, BLACK);
	   	WriteOneHz (58, 125, (u8 *)HZ[6], WHITE, BLACK);        //五
		WriteOneHz (74, 125, (u8 *)HZ[3], WHITE, BLACK); 
		WriteOneHz (90, 125, (u8 *)HZ[4], WHITE, BLACK);
		WriteOneHz (106, 125, (u8 *)HZ[13], WHITE, BLACK); 
		//LCD_ShowNum(122,125,u32 num,u8 len, WHITE, BLACK);
	
		WriteOneHz (10, 145, (u8 *)HZ[5], WHITE, BLACK);
	   	WriteOneHz (26, 145, (u8 *)HZ[6], WHITE, BLACK);
	   	WriteOneHz (42, 145, (u8 *)HZ[14], WHITE, BLACK);
	   	WriteOneHz (58, 145, (u8 *)HZ[15], WHITE, BLACK);       
		WriteOneHz (74, 145, (u8 *)HZ[13], WHITE, BLACK); 		//基波幅值
		//LCD_ShowNum(90,145,u32 num,u8 len, WHITE, BLACK);

		WriteOneHz (10, 165, (u8 *)HZ[9], WHITE, BLACK);
	   	WriteOneHz (26, 165, (u8 *)HZ[7], WHITE, BLACK);
	    WriteOneHz (42, 165, (u8 *)HZ[8], WHITE, BLACK);
	   	WriteOneHz (58, 165, (u8 *)HZ[6], WHITE, BLACK);        
	    WriteOneHz (74, 165, (u8 *)HZ[14], WHITE, BLACK);  
	   	WriteOneHz (90, 165, (u8 *)HZ[15], WHITE, BLACK);  
		WriteOneHz (106, 165, (u8 *)HZ[13], WHITE, BLACK);  //二次谐波幅值
		//LCD_ShowNum(122,185,u32 num,u8 len, WHITE, BLACK);
		
		
			WriteOneHz (10, 185, (u8 *)HZ[10], WHITE, BLACK);
	   	WriteOneHz (26, 185, (u8 *)HZ[7], WHITE, BLACK);
	    WriteOneHz (42, 185, (u8 *)HZ[8], WHITE, BLACK);
	   	WriteOneHz (58, 185, (u8 *)HZ[6], WHITE, BLACK);        
	    WriteOneHz (74, 185, (u8 *)HZ[14], WHITE, BLACK);  
	   	WriteOneHz (90, 185, (u8 *)HZ[15], WHITE, BLACK);  
			WriteOneHz (106, 185, (u8 *)HZ[13], WHITE, BLACK); //三
		//LCD_ShowNum(122,185,u32 num,u8 len, WHITE, BLACK);
	
		WriteOneHz (10, 205, (u8 *)HZ[11], WHITE, BLACK);
	   	WriteOneHz (26, 205, (u8 *)HZ[7], WHITE, BLACK);
	    WriteOneHz (42, 205, (u8 *)HZ[8], WHITE, BLACK);
	   	WriteOneHz (58, 205, (u8 *)HZ[6], WHITE, BLACK);        
	    WriteOneHz (74, 205, (u8 *)HZ[14], WHITE, BLACK);  
	   	WriteOneHz (90, 205, (u8 *)HZ[15], WHITE, BLACK);  
		WriteOneHz (106, 205, (u8 *)HZ[13], WHITE, BLACK);	//四
		//LCD_ShowNum(122,205,u32 num,u8 len, WHITE, BLACK);
	
		WriteOneHz (10, 225, (u8 *)HZ[12], WHITE, BLACK);
	   	WriteOneHz (26, 225, (u8 *)HZ[7], WHITE, BLACK);
	    WriteOneHz (42, 225, (u8 *)HZ[8], WHITE, BLACK);
	   	WriteOneHz (58, 225, (u8 *)HZ[6], WHITE, BLACK);        
	    WriteOneHz (74, 225, (u8 *)HZ[14], WHITE, BLACK);  
	   	WriteOneHz (90, 225, (u8 *)HZ[15], WHITE, BLACK);  
		WriteOneHz (106, 225, (u8 *)HZ[13], WHITE, BLACK);	//五
		//LCD_ShowNum(122,225,u32 num,u8 len, WHITE, BLACK);
	
}
	int8_t time=1;
	uint16_t k=0;
int main(void)
{  
  //RCC_Config();                                             //系统频率在56MHz
	LED_Init();
	Info.SampleFreq=(uint16_t)(2500*1.024f);
	USART1_Init();
	USART2_Init();
	Key_Init();
	FSMC_SRAM_Init(); 
	Delay_Init();                                             //systick初始化

	LCD_Init();
	ADC1_Init();
	LCD_Fill(0,0, 240, 320, WHITE);
	SearchFunFreqAmp();
	//NonSinSearcher();

	while(1){
//		//LCD_ShowNum(180, 0,(uint32_t)(ADC_GetConversionValue(ADC1)),6,RED, WHITE);
//		SDS[0]=ADC_GetConversionValue(ADC1);
//		//Plot(SDS);
//		Delay_ms(5);
	SearchFunFreqAmp();
	LCD_ShowNum(0, 120,k++,6,RED, WHITE);
	}
	//LCD_ShowNum(0, 0, 20, 8, u16 PenColor, u16 BackColor);
	//hebut();

	//TIM3_Int_Init(((u32)1000000/InitSampleFreq)-1,36-1);//31//124 20.58//13
	//124 20.58 1Khz 0%
	//496 5.17 50Hz 0.8%
	//62 40.89
	//25 99.01 5khz 1.8%
	//2000 1.29
	//2500 1.03
	//10000 257.2
	//850 3.02
//	for(;;)
//	{
//		 if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13)==0)
//		 {	
//			Delay(10);
//			time*=-1;
//		 }
//		 if(time==1){
//			//LCD_Clear(BLACK);
//			//hebut();
//		 }
//		 if(time==-1){
//			//LCD_Clear(BLACK);
//			shownumber();
//		 }
//			
//			
//		
//	}
}
void TIM3_IRQHandler(void)   //TIM3??
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //??TIM3???????
	{
			Info.buff[Info.cnt]=(int16_t)(ADC_GetConversionValue(ADC1))<<16;
		LED_GPIO_PORT->ODR ^= LED1_GPIO_PIN;//turn PF6
		if(Info.cnt>=LEN-1){
			TIM_Cmd(TIM3, DISABLE);  //??TIM3		
		}else Info.cnt++;
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  //??TIM3??????
	}
} 


uint16_t i=0;
void LOOP(){
	if(Info.cnt>=LEN-1){

	cr4_fft_1024_stm32(Info.FArray, Info.buff, LEN);
		GetPowerMag();
		
//		for(i=0;i<LEN;i++){
//			SDS[0]=(int16_t)(Info.buff[i]);
//			Plot(SDS);
//		}
		Info.cnt=0;
		TIM_Cmd(TIM3,ENABLE);
	}
}


