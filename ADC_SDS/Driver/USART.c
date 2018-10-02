/**
  * @file    USART.c 
  * @author  WB R&D Team - openmcu666
  * @version V1.0
  * @date    2016.05.04
  * @brief   USART Driver
  */

#include "USART.h"

uint16_t data_length;								// 数据长度
uint8_t rx_buffer[DATA_BUF_SIZE];		// 接收缓冲区
uint8_t tx_buffer[DATA_BUF_SIZE];		// 发送缓冲区
/**
  * @brief  USART1初始化配置 包括GPIO初始化 TX必须配置为复用输出
  * @param  None
  * @retval None
  */
void USART1_Init(void)
{	
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA  ,  ENABLE );

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                   //复用推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
    
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;              //浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate            = 115200  ;
	USART_InitStructure.USART_WordLength          = USART_WordLength_8b;           //8个数据位
	USART_InitStructure.USART_StopBits            = USART_StopBits_1;              //1个停止位
	USART_InitStructure.USART_Parity              = USART_Parity_No ;              //无奇偶校验
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//禁用硬件流控制
	USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;  //使能发送/接收
	USART_Init(USART1, &USART_InitStructure);                           
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);                                  //使能接收中断
	USART_Cmd(USART1, ENABLE);     
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);			                            //中断分组2
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; 		                          //USART1接收中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;                       //先占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		                          //次占优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
  
}

/**
  * @brief  USART1发送数据
  * @param  None
  * @retval None
  */
void USART1_SendByte(u16 Data)
{ 
   while (!(USART1->SR & USART_FLAG_TXE));
   USART1->DR = (Data & (uint16_t)0x01FF);	 
   
}

/**
  * @brief  USART1发送字符串
  * @param  *buf:发送缓冲区地址 len:需要发送的数据长度
  * @retval None
  */
void USART1_SendString(u8 *buf,uint16_t len)
{
	uint16_t t; 
  for(t=0;t<len;t++)		                                     //循环发送数据
	{
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET); //等待发送结束		
		USART_SendData(USART1,buf[t]);                           //发送数据
	}	 
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);   //等待发送结束	
}

/**
  * @brief  USART2初始化配置 包括GPIO初始化 TX必须配置为复用输出
  * @param  None
  * @retval None
  */
void USART2_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA  ,  ENABLE );
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART2 | RCC_APB1Periph_USART3, ENABLE );
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                       //复用推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;                 //浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate            = 115200  ;
	USART_InitStructure.USART_WordLength          = USART_WordLength_8b;  //8个数据位
	USART_InitStructure.USART_StopBits            = USART_StopBits_1;     //1个停止位
	USART_InitStructure.USART_Parity              = USART_Parity_No ;     //无奇偶校验
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//禁用硬件流控制
	USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;//使能发送/接收
	USART_Init(USART2, &USART_InitStructure);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART2, ENABLE);   

  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn; 		                          //USART2接收中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;                       //先占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		                          //次占优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

}

/**
  * @brief  USART2发送数据
  * @param  None
  * @retval None
  */
void USART2_SendByte(u16 Data)
{ 
   while (!(USART2->SR & USART_FLAG_TXE));
   USART2->DR = (Data & (uint16_t)0x01FF);	   
}

/**
  * @brief  USART2发送字符串
  * @param  None
  * @retval None
  */
void USART2_SendString(uint8_t *ch)
{
	while(*ch!=0)
	{		
		while(!USART_GetFlagStatus(USART2, USART_FLAG_TXE));
		USART_SendData(USART2, *ch);
		ch++;
	}   	
}

/**
  * @brief  USART1和USART2发送字符串
  * @param  None
  * @retval None
  */
void SendMessage(void)
{
	const char menu[] =
   "\n\r"
   "\n\r+******************** 红牛开发板  **********************+\n\r"
   "\n\r|                  USART 异步通信试验                   |\n\r"
   "\n\r|                 欢迎进入论坛发帖交流                  |\n\r"
   "\n\r|                 www.openmcu.com/bbs                   |\n\r"
   "\n\r|                     2016.05.04                        |\n\r"
   "\n\r|                技术支持群：259180202                  |\n\r"
   "\n\r+-------------------------------------------------------+\n\r";

   printf(menu);
   printf("\n\rHello World! USART1_Test\n\r");
	 USART2_SendString("\n\rUSART2_Test>>请输入a~c任意字母控制LED的亮灭\n\r");
}

#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
/*******************************************************************************
* Function Name  : PUTCHAR_PROTOTYPE
* Description    : 把USART输出定向到printf
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(USART1, (u8) ch);

  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
  {}

  return ch;
}

