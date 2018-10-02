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
/*****************************************************************************
**   Main Function  main()
******************************************************************************/
extern float SDS[4];

int main(void)
{  
	float Voltage=0.00; 
  RCC_Config();                                             //系统频率在56MHz
	LED_Init();
  USART1_Init();
	USART2_Init();
  Delay_Init();                                             //systick初始化
	ADC1_Init();
	while(1)
	{
#ifndef USE_DMA
		ADCValue = ADC_GetConversionValue(ADC1);	                //获取转换值
#endif
		Voltage= (3.3/4096)*ADCValue;			                      //将AD值转换为电压值
//		printf("\r\n The ADC_Value is %d \r\n",ADCValue);
//		printf("\r\n The Voltage_Value is %.2f V \r\n",Voltage);
//    Delay_ms(1000);
		SDS[0]=ADCValue;
//		SDS[1]=
//		SDS[2]=
//		SDS[3]=
			Plot(SDS);
			Delay_ms(5);
  }
}
