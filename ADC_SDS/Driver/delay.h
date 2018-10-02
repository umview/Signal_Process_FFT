#ifndef __DELAY_H
#define __DELAY_H

#include "stm32f10x.h"

void Delay_Init(void);
void Delay_ms(u16 nms);
void Delay_us(u32 nus);
void Delay(uint32_t Time);

#endif
