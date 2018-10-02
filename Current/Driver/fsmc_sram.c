/**
  * @file    fsmc_sram.c 
  * @author  WB R&D Team - openmcu666
  * @version V1.0
  * @date    2016.05.04
  * @brief   SRAM Driver
  */
#include "fsmc_sram.h"
#include "Timer.h"
//static u16 TxBuffer[BUFFER_SIZE];
//static u16 RxBuffer[BUFFER_SIZE];
u32 WriteReadStatus = 0, Index = 0;
/**
  * @brief  初始化FSMC及其IO
  * @param  None
  * @retval None
  */
void FSMC_SRAM_Init(void)
{
  FSMC_NORSRAMInitTypeDef        FSMC_NORSRAMInitStructure;
  FSMC_NORSRAMTimingInitTypeDef  p;
  GPIO_InitTypeDef               GPIO_InitStructure; 
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOG | RCC_APB2Periph_GPIOE |
                         RCC_APB2Periph_GPIOF, ENABLE);
	
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
	
/*-- GPIO Configuration ------------------------------------------------------*/
  /* SRAM Data lines configuration */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_8 | GPIO_Pin_9 |
                                GPIO_Pin_10 | GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOD, &GPIO_InitStructure); 
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 |
                                GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | 
                                GPIO_Pin_15;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  
  /* SRAM Address lines configuration */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | 
                                GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_12 | GPIO_Pin_13 | 
                                GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_Init(GPIOF, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | 
                                GPIO_Pin_4 | GPIO_Pin_5;
  GPIO_Init(GPIOG, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13; 
  GPIO_Init(GPIOD, &GPIO_InitStructure);
   
  /* NOE and NWE configuration */  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 |GPIO_Pin_5;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  
  /* NE3 configuration */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 
  GPIO_Init(GPIOG, &GPIO_InitStructure);
  
  /* NBL0, NBL1 configuration */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1; 
  GPIO_Init(GPIOE, &GPIO_InitStructure); 
  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	                 //推挽输出
	GPIO_Init(GPIOG, &GPIO_InitStructure);
  GPIO_SetBits(GPIOG,GPIO_Pin_9|GPIO_Pin_12);
		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	                 //复用推挽输出
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOE, GPIO_Pin_2);
	
/*-- FSMC Configuration ------------------------------------------------------*/
   p.FSMC_AddressSetupTime = 2;
   p.FSMC_AddressHoldTime = 2;
   p.FSMC_DataSetupTime = 1;
   p.FSMC_BusTurnAroundDuration = 1;
   p.FSMC_CLKDivision = 0;
   p.FSMC_DataLatency = 0;
	
  p.FSMC_AccessMode = FSMC_AccessMode_A;

  FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM3;
  FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
  FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM;
  FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
  FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
  FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
  FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
  FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
  FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &p;
  FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &p;

  FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure); 

  FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM3, ENABLE);   //Enable FSMC Bank1_SRAM Bank 
}

/**
  * @brief  向SRAM写入指定长度的数据
  * @param  pBuffer:指向发送缓冲;WriteAddr:写入地址;NumHalfwordToWrite:数据长度
  * @retval None
  */
void FSMC_SRAM_WriteBuffer(u16* pBuffer, u32 WriteAddr, u32 NumHalfwordToWrite)
{
  for(; NumHalfwordToWrite != 0; NumHalfwordToWrite--)     //while there is data to write 
  {
   
    *(u16 *) (Bank1_SRAM3_ADDR + WriteAddr) = *pBuffer++;  // Transfer data to the memory 
    
    WriteAddr += 2;                                        //Increment the address
  }   
}

/**
  * @brief  从SRAM读取指定长度的数据
  * @param  pBuffer:指向读取缓冲;ReadAddr:读取地址;NumHalfwordToRead:数据长度
  * @retval None
  */
void FSMC_SRAM_ReadBuffer(u16* pBuffer, u32 ReadAddr, u32 NumHalfwordToRead)
{
  for(; NumHalfwordToRead != 0; NumHalfwordToRead--)          //while there is data to read 
  {
    
    *pBuffer++ = *(vu16*) (Bank1_SRAM3_ADDR + ReadAddr);      //Read a half-word from the memory 

    ReadAddr += 2;                                            //Increment the address
  }  
}

/**
  * @brief  填充数据
  * @param  pBuffer:指向填充缓冲;BufferLenght:数据长度;Offset:开始的数值
  * @retval None
  */
void Fill_Buffer(u16 *pBuffer, u16 BufferLenght, u32 Offset)
{
  u16 IndexTmp = 0;

  for (IndexTmp = 0; IndexTmp < BufferLenght; IndexTmp++ )
  {
    pBuffer[IndexTmp] = IndexTmp + Offset;
  }
}

/**
  * @brief  SRAM读写测试
  * @param  None
  * @retval None
  */
//void SRAM_Test(void)
//{
//	vu32 time1=0,time2=0;
//	printf("\r\n***********************SRAM读写测试**********************\r\n");
//  Fill_Buffer(TxBuffer, BUFFER_SIZE, 0x3212);
//	TIM_Init();
//	
//	time1=get_runtime();
//  FSMC_SRAM_WriteBuffer(TxBuffer, WRITE_READ_ADDR, BUFFER_SIZE);//从32k开始写
//  time2=get_runtime();
//	printf("\r\n写1KB耗时%dus,写速度:%dKB/s\r\n",(time2-time1)*10,100000/(time2-time1)); //10us中断1次
//  time1=0;
//	time2=0;
//	time1=get_runtime();
//	FSMC_SRAM_ReadBuffer(RxBuffer, WRITE_READ_ADDR, BUFFER_SIZE);  
//  time2=get_runtime();
//  printf("\r\n读1KB耗时%dus,读速度:%dKB/s\r\n",(time2-time1)*10,100000/(time2-time1)); //10us中断1次
//	
//	Set_TIM2_IRQn(DISABLE);
//	
//	for(Index = 0x00; (Index < BUFFER_SIZE) && (WriteReadStatus == 0); Index++)//检查内容的正确性
//  {
//    if(RxBuffer[Index] != TxBuffer[Index])
//    {
//	    GPIO_ResetBits(LED_GPIO_PORT, LED3_GPIO_PIN);		 //如果读出来的和写进去的不相同则LD3亮

//      WriteReadStatus = Index + 1;
//    }
//  }	

//  if (WriteReadStatus == 0)	                            //pass
//  {
//    printf("\r\nSRAM读写测试通过\r\n");
//    GPIO_ResetBits(LED_GPIO_PORT, LED1_GPIO_PIN);		    //LD1亮的话说明写进去的和读出来的完全一样 
//  }
//  else                                                   //error
//  { 
//		printf("\r\nSRAM读写测试失败,请复位后再试一次\r\n");
//    GPIO_ResetBits(LED_GPIO_PORT, LED2_GPIO_PIN);     	  //如果读出来的和写进去的不相同则LD2亮
//   
//	}
//	
//}
