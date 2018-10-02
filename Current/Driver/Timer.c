/**
  * @file    Timer.c 
  * @author  WB R&D Team - openmcu666
  * @version V1.0
  * @date    2016.05.06
  * @brief   Timer Driver
  */

#include "Timer.h"
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

void TIM3_Int_Init(u16 arr,u16 psc)

{


	NVIC_InitTypeDef NVIC_InitStructure;

 

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //????

	

	//???TIM3???

	TIM_TimeBaseStructure.TIM_Period = arr; //???????????????????????????	

	TIM_TimeBaseStructure.TIM_Prescaler =psc; //??????TIMx???????????

	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 

	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM??????

	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //??????????TIMx???????

 

	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //?????TIM3??,??????

 

	//?????NVIC??

	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn; 

	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  

	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  
	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 

	NVIC_Init(&NVIC_InitStructure);  //???NVIC???
	TIM_Cmd(TIM3, ENABLE);  //??TIM3					 

}

//???3??????



//vu16 CCR1_Val = 40000;						      //初始化输出比较通道1计数周期变量
//vu16 CCR2_Val = 20000;						      //初始化输出比较通道2计数周期变量 
//vu16 CCR3_Val = 10000;				          //初始化输出比较通道3计数周期变量 
//vu16 CCR4_Val = 5000;						        //初始化输出比较通道4计数周期变量 

///**
//  * @brief  TIM2初始化配置
//  * @param  None
//  * @retval None
//  */   							
//void TIM_Init(void)
//{
//	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;   //定义 TIM_TimeBase 初始化结构体 TIM_TimeBaseStructure 

//	TIM_OCInitTypeDef  TIM_OCInitStructure;           //定义 TIM_OCInit 初始化结构体 TIM_OCInitStructure
//	
//	NVIC_InitTypeDef NVIC_InitStructure;
//	
//	RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM2, ENABLE );        //使能TIM2时钟
//	
//	TIM_TimeBaseStructure.TIM_Period = 65535;                      //计数重载值为65535
//	TIM_TimeBaseStructure.TIM_Prescaler = 0;                       //预分频0
//	TIM_TimeBaseStructure.TIM_ClockDivision = 0;                   //时钟分割0
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	   //向上计数
//	TIM_TimeBaseInit(TIM2 , &TIM_TimeBaseStructure);               //初始化TIM2

//  //预分频值为(7199 + 1 = 7200)且立即装入	
//	TIM_PrescalerConfig(TIM2 , 7199 , TIM_PSCReloadMode_Immediate);
//	
//	
//	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;             //输出比较时间模式
//	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	  //输出使能
//	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;       //TIM2输出极性高

//	TIM_OCInitStructure.TIM_Pulse = CCR1_Val;	                      //写值到捕获/比较寄存器CCR1
//	TIM_OC1Init(TIM2, &TIM_OCInitStructure);
//		
//	TIM_OCInitStructure.TIM_Pulse = CCR2_Val;	                      //写值到捕获/比较寄存器CCR2
//	TIM_OC2Init(TIM2, &TIM_OCInitStructure);
//	
//	TIM_OCInitStructure.TIM_Pulse = CCR3_Val;	                      //写值到捕获/比较寄存器CCR3
//	TIM_OC3Init(TIM2, &TIM_OCInitStructure);
//		
//	TIM_OCInitStructure.TIM_Pulse = CCR4_Val;	                      //写值到捕获/比较寄存器CCR4
//	TIM_OC4Init(TIM2, &TIM_OCInitStructure);


//	TIM_OC1PreloadConfig(TIM2 , TIM_OCPreload_Disable);             //禁止预装载寄存器 
//	TIM_OC2PreloadConfig(TIM2 , TIM_OCPreload_Disable);
//	TIM_OC3PreloadConfig(TIM2 , TIM_OCPreload_Disable);
//	TIM_OC4PreloadConfig(TIM2 , TIM_OCPreload_Disable);
//		
//	//使能 TIM 中断 
//	TIM_ITConfig(TIM2 , TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_CC3 | TIM_IT_CC4 , ENABLE);
//	
//	TIM_Cmd(TIM2 , ENABLE);	                                        //启动 TIM 计数 
//  
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);	//选择优先级分组0 
//	
//	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn; //开启 TIM2 中断, 0级先占优先级，0级后占优先级 
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
//}

