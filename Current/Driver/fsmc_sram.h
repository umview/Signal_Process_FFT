
#ifndef __FSMC_SRAM_H
#define __FSMC_SRAM_H

#include "stm32f10x.h"
#include "Gpio.h"
#include "Timer.h"
#include "USART.h"

#define Bank1_SRAM3_ADDR    ((u32)0x68000000)
#define BUFFER_SIZE         0x400
#define WRITE_READ_ADDR     0x8000

void FSMC_SRAM_Init(void);
void FSMC_SRAM_WriteBuffer(u16* pBuffer, u32 WriteAddr, u32 NumHalfwordToWrite);
void FSMC_SRAM_ReadBuffer(u16* pBuffer, u32 ReadAddr, u32 NumHalfwordToRead);
void SRAM_Test(void);
#endif 


