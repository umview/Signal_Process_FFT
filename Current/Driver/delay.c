/**
  * @file    Gpio.c 
  * @author  WB R&D Team - openmcu666
  * @version V1.0
  * @date    2016.05.04
  * @brief   systick
  */
#include "delay.h"

static	uint8_t  sysclk = 72;                             //默认系统时钟为72MHz
static  uint8_t  fac_us=0;                                //us延时倍乘数
static  uint16_t fac_ms=0;                                //ms延时倍乘数 

/**
  * @brief  基准延时初始化，使用SysTick;
  *         SysTick时钟源由HCLK/8提供，当系统频率为72MHz时
  *         最小计数周期为1/9MHz,计满9次为1us，fac_us以9为单位        
  * @param  None
  * @retval None
  */
void Delay_Init(void)
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);     //选择系统定时器的时钟源
	fac_us = sysclk/8;		    
	fac_ms=(u16)fac_us*1000;
}

/**
  * @brief  毫秒级延时，SysTick->LOAD为24位寄存器,
  *         所以最大延时小于或等于1864ms
  * @param  nms(ms number) 毫秒数 不能超过1864
  * @retval None
  */
void Delay_ms(u16 nms)
{	 		  	  
	u32 temp;		   
	SysTick->LOAD=(u32)nms*fac_ms;                             //时间加载(SysTick->LOAD为24bit)
	SysTick->VAL =0x00;                                        //清空计数器 
	SysTick->CTRL=0x01 ;                                       //开始倒数   
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));                          //等待时间到达  
	SysTick->CTRL=0x00;                                         //关闭计数器	 
	SysTick->VAL =0x00;                                         //清空计数器	 	    
} 

/**
  * @brief  实现微秒级延时，最大延时1864ms
  *         这两个函数是通过寄存器配置的，没有对应的固件库函数，参考misc.h
  * @param  us(number us) 延时微秒数 最大1864135
  * @retval None
  */
void Delay_us(u32 nus)
{		
	u32 temp;	    	 
	SysTick->LOAD=nus*fac_us;                             //时间加载 fac_us=9,走9次就是1us	 		 
	SysTick->VAL=0x00;                                    //清空计数器 
	SysTick->CTRL=0x01 ;                                  //开始倒数  
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));                     //等待时间到达  
	SysTick->CTRL=0x00;                                    //关闭计数器
	SysTick->VAL =0x00;                                    //清空计数器	
}

/**
  * @brief  延时
  * @param  Time:0-65535
  * @retval None
  */
void Delay(uint32_t Time)
{
  uint32_t i;
    
  i = 0;
  while (Time--) 
	{
    for (i = 0; i < 8000; i++);
  }
}
