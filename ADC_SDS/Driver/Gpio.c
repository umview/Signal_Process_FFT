/**
  * @file    Gpio.c 
  * @author  WB R&D Team - openmcu666
  * @version V1.0
  * @date    2016.05.04
  * @brief   GPIO Driver
  */
#include "Gpio.h"

/**
  * @brief  初始化LED管脚
  * @param  None
  * @retval None
  */
void LED_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_APB2PeriphClockCmd(LED_GPIO_CLK, ENABLE);

	GPIO_InitStructure.GPIO_Pin = LED1_GPIO_PIN | LED2_GPIO_PIN | LED3_GPIO_PIN
                                	            | LED4_GPIO_PIN | LED5_GPIO_PIN;
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(LED_GPIO_PORT, &GPIO_InitStructure);
	GPIO_SetBits(LED_GPIO_PORT,LED1_GPIO_PIN|LED2_GPIO_PIN|LED3_GPIO_PIN|LED4_GPIO_PIN|LED5_GPIO_PIN);
}

/**
  * @brief  翻转端口电平
  * @param  选择相应LED 1-LED1 2-LED2
  * @retval None
  */
void LED_Toggle(uint8_t led)
{
	switch(led)
	{	
		case 1: LED_GPIO_PORT->ODR ^= LED1_GPIO_PIN;
			break;
		case 2: LED_GPIO_PORT->ODR ^= LED2_GPIO_PIN;
			break;
		case 3: LED_GPIO_PORT->ODR ^= LED3_GPIO_PIN;
			break;
		case 4: LED_GPIO_PORT->ODR ^= LED4_GPIO_PIN;
			break;
		case 5: LED_GPIO_PORT->ODR ^= LED5_GPIO_PIN;
			break;
	}
}

/**
  * @brief  初始化蜂鸣器控制管脚PD3
  * @param  None
  * @retval None
  */
void BEEP_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
	
  RCC_APB2PeriphClockCmd(BEEP_GPIO_CLK, ENABLE);
	
  GPIO_InitStructure.GPIO_Pin = BEEP_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(BEEP_GPIO_PORT, &GPIO_InitStructure);
	
  GPIO_ResetBits(BEEP_GPIO_PORT,BEEP_GPIO_PIN);

}

/**
  * @brief  初始化按键检测引脚
  * @param  None
  * @retval None
  */
void Key_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_APB2PeriphClockCmd(KEY2_3_GPIO_CLK | KEY4_GPIO_CLK | KEY5_GPIO_CLK , ENABLE);

 	GPIO_InitStructure.GPIO_Pin = KEY2_GPIO_PIN | KEY3_GPIO_PIN;	  //PA0 PA8设置为输入
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(KEY2_3_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = KEY4_GPIO_PIN;				              //PC13设置为输入
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(KEY4_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = KEY5_GPIO_PIN;					              //PD3设置为输入
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(KEY5_GPIO_PORT, &GPIO_InitStructure); 
	
}
