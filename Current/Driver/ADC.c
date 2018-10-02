/**
  * @file    ADC.c 
  * @author  WB R&D Team - openmcu666
  * @version V1.0
  * @date    2016.05.04
  * @brief   ADC Driver
  */
	
#include "ADC.h"

vu16 ADCValue;                                           //全局变量,用于转换后的AD值

/**
  * @brief  配置系统时钟为56MHz
  * @param  None
  * @retval None
  */
void RCC_Config(void)
{
	ErrorStatus HSEStartUpStatus;                           //定义枚举类型变量HSEStartUpStatus
	RCC_DeInit();                                           //复位系统时钟
	RCC_HSEConfig(RCC_HSE_ON);                              //开启HSE
	HSEStartUpStatus = RCC_WaitForHSEStartUp();             //等待HSE起振
	if(HSEStartUpStatus == SUCCESS)
	{
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
		FLASH_SetLatency(FLASH_Latency_2);
		RCC_HCLKConfig(RCC_SYSCLK_Div1);                      //选择HCLK时钟源为SYSCLK1分频
		RCC_PCLK2Config(RCC_HCLK_Div1);                       //选择PCLK2的时钟源为HCLK1分频
		RCC_PCLK1Config(RCC_HCLK_Div2);                       //选择PCLK1的时钟源为HCLK2分频
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_7);	//选择PLL时钟源为HSE1分频，7倍频 56MHz
		RCC_PLLCmd(ENABLE);                                   //使能PLL
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);   //等待PLL输出稳定
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);            //系统时钟为PLL
		while(RCC_GetSYSCLKSource() != 0x08);                 //等待PLL为系统时钟源
	}

}

/**
  * @brief  ADC1初始化配置 (默认不使用DMA,在ADC.h中配置)
  * @param  None
  * @retval None
  */

void ADC1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
#ifdef USE_DMA                                                      //in ADC.h file
	DMA_InitTypeDef DMA_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
#endif
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO 
	                           | RCC_APB2Periph_ADC1 , ENABLE );
                           
  RCC_ADCCLKConfig(RCC_PCLK2_Div4);	                                  //ADC时钟为PCLK2的4分频 56/4
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;		                        //ADC1通道15引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	                      //模拟输入模式
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
#ifdef USE_DMA
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADCValue;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = 1;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
		
	DMA_Cmd(DMA1_Channel1, ENABLE);                                    //Enable DMA1 channel1 
	
#endif	

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	                //独立工作模式
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;					              //开启多通道扫描
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;					        //连续转换模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//软件触发启动
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	            //ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 1;	                            //进行规则转换的ADC通道为1
	ADC_Init(ADC1, &ADC_InitStructure);
	

	ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 1, ADC_SampleTime_55Cycles5);
#ifdef USE_DMA 	
	ADC_DMACmd(ADC1, ENABLE);                                           //Enable ADC1 DMA 
#endif
  ADC_Cmd(ADC1, ENABLE);	                                            //使能ADC1
	  
	ADC_ResetCalibration(ADC1);	                                        //使能ADC1复位校准寄存器

	while(ADC_GetResetCalibrationStatus(ADC1));	                        //等待复位校准寄存器接收
	
	ADC_StartCalibration(ADC1);			                                    //启动ADC1校准

	while(ADC_GetCalibrationStatus(ADC1));	                            //等待ADC1校准结束
	
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		                          //启动软件转换
}

/*
void ADC1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;

	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO 
	                           | RCC_APB2Periph_ADC1 , ENABLE );
                           
  RCC_ADCCLKConfig(RCC_PCLK2_Div6);	                                  //ADC时钟为PCLK2的4分频 56/4
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;		                        //ADC1通道15引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	                      //模拟输入模式
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	


	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	                //独立工作模式
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;					              //开启多通道扫描
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;					        //连续转换模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//软件触发启动
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	            //ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 1;	                            //进行规则转换的ADC通道为1
	ADC_Init(ADC1, &ADC_InitStructure);
	

	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_55Cycles5);

  ADC_Cmd(ADC1, ENABLE);	                                            //使能ADC1
	  
	ADC_ResetCalibration(ADC1);	                                        //使能ADC1复位校准寄存器

	while(ADC_GetResetCalibrationStatus(ADC1));	                        //等待复位校准寄存器接收
	
	ADC_StartCalibration(ADC1);			                                    //启动ADC1校准

	while(ADC_GetCalibrationStatus(ADC1));	                            //等待ADC1校准结束
	
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		                          //启动软件转换
}
*/
