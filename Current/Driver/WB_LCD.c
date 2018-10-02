/**
  * @file    WB_LCD.c 
  * @author  WB R&D Team - openmcu666
  * @version V1.0
  * @date    2016.05.04
  * @brief   LCD Driver
	* @description 红牛2.8/3.2/3.5寸LCD屏的驱动(支持的驱动IC为SSD1298、ili9325/ili9328、SSD2119)
  */
#include "WB_LCD.h"
#include "font.h"
uint16_t DeviceCode;                                             //用于存取设备ID
#define  Font_Size 16                 		                       // 默认字体为16*8 
#define  XY    1										                       //文字支持横竖屏切换,竖屏:1;横屏:0

                                               
/*********************************************************
** Function name:       void Delay_10ms (u32 nCount)
** Descriptions:        LCD延时函数 10MS
** input parameters:    nCount
** output parameters:   无
** Returned value:      无
*********************************************************/
void Delay_10ms (u32 nCount)
{
	volatile int i;	 	
	for (i=0;i<nCount*800;i++);
}

/*********************************************************
** Function name:       void LCD_WR_REG(uint16_t LCD_Reg)
** Descriptions:        写寄存器
** input parameters:    LCD_Reg
** output parameters:   无
** Returned value:      无
*********************************************************/
void LCD_WR_REG(uint16_t LCD_Reg)
{
	LCD_REG= LCD_Reg;	 		//往指令寄存器写指令
}

/*********************************************************
** Function name:       void LCD_WR_DATA(uint16_t LCD_Data)	
** Descriptions:        写数据
** input parameters:    LCD_Data
** output parameters:   无
** Returned value:      无
*********************************************************/
void LCD_WR_DATA(uint16_t LCD_Data)	
{
	LCD_RAM= LCD_Data; 			//往数据寄存器写数据
} 

/*********************************************************************************
** Function name:     void LCD_WriteReg(uint16_t LCD_Reg ,uint16_t LCD_RegValue)
** Descriptions:      配置寄存器
** input parameters:  寄存器地址:LCD_Reg   数据:RegValue
** output parameters: 无
** Returned value:    无
**********************************************************************************/
void LCD_WriteReg(uint16_t LCD_Reg ,uint16_t LCD_RegValue)
{
	LCD_WR_REG(LCD_Reg);                    //写寄存器
  LCD_WR_DATA(LCD_RegValue);	            //写数据
			    
}

/*********************************************************
** Function name:       void LCD_WriteRAM_Prepare(void)
** Descriptions:        开始写GRAM
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************/
void LCD_WriteRAM_Prepare(void)
{
	LCD_WR_REG(0x22);
}


/*********************************************************
** Function name:       void LCD_WriteRAM(u16 RGB_Code)
** Descriptions:        LCD写GRAM
** input parameters:    颜色值
** output parameters:   无
** Returned value:      无
*********************************************************/
void LCD_WriteRAM(u16 RGB_Code)
{							    
	LCD_WR_DATA(RGB_Code);               //写十六位GRAM
}

/*********************************************************
** Function name:       uint16_t Read_LCDReg(uint16_t LCD_Reg)
** Descriptions:        读寄存器
** input parameters:    LCD_Reg:寄存器
** output parameters:   无
** Returned value:      返回寄存器值
*********************************************************/
uint16_t Read_LCDReg(uint16_t LCD_Reg)
{
	uint16_t temp;

	LCD_REG= LCD_Reg;
 
	temp= LCD_RAM; 
  
  return temp;		
}

/**********************************************************************************
** Function name:       void LCD_Configuration(void)
** Descriptions:        LCD相关引脚配置
** input parameters:  	无
** output parameters:   无
** Returned value:      无
************************************************************************************/
void LCD_IOConfig(void)
{
	 GPIO_InitTypeDef GPIO_InitStructure;


  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE |
                         RCC_APB2Periph_GPIOF | RCC_APB2Periph_GPIOG |
                         RCC_APB2Periph_AFIO, ENABLE);

  /* Set PD.00(D2), PD.01(D3), PD.04(NOE), PD.05(NWE), PD.08(D13), PD.09(D14),
     PD.10(D15), PD.14(D0), PD.15(D1) as alternate 
     function push pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 |
                                GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_14 | 
                                GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  /* Set PE.07(D4), PE.08(D5), PE.09(D6), PE.10(D7), PE.11(D8), PE.12(D9), PE.13(D10),
     PE.14(D11), PE.15(D12) as alternate function push pull */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | 
                                GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | 
                                GPIO_Pin_15;
  GPIO_Init(GPIOE, &GPIO_InitStructure);

  GPIO_WriteBit(GPIOE, GPIO_Pin_6, Bit_SET);
  /* Set PF.00(A0 (RS)) as alternate function push pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_Init(GPIOF, &GPIO_InitStructure);

  /* Set PG.12(NE4 (LCD/CS)) as alternate function push pull - CE3(LCD /CS) */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  GPIO_Init(GPIOG, &GPIO_InitStructure);
}

/**********************************************************************************
** Function name:       void FSMC_LCDInit(void)
** Descriptions:        FSMC时序配置
** input parameters:  	无
** output parameters:   无
** Returned value:      无
************************************************************************************/
void FSMC_LCDInit(void)
{
	FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;	   	//定义一个NOR初始化结构体
	FSMC_NORSRAMTimingInitTypeDef  w,r;						          //定义一个NOR时序初始化结构体

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);						//使能FSMC总线时钟

  w.FSMC_AddressSetupTime = 0x00;							                  //地址建立时间：0x1
  w.FSMC_AddressHoldTime = 0x00;							                  //地址保持时间：0x00
  w.FSMC_DataSetupTime = 0x01;							                    //数据建立时间：0x2
  w.FSMC_BusTurnAroundDuration = 0x00;
  w.FSMC_CLKDivision = 0x00;
  w.FSMC_DataLatency = 0x00;
  w.FSMC_AccessMode = FSMC_AccessMode_B;

	r.FSMC_AddressSetupTime = 0x00;							                   //地址建立时间：0x1
  r.FSMC_AddressHoldTime = 0x00;							                   //地址保持时间：0x00
  r.FSMC_DataSetupTime = 0x07;							                     //数据建立时间：0x2
  r.FSMC_BusTurnAroundDuration = 0x00;
  r.FSMC_CLKDivision = 0x00;
  r.FSMC_DataLatency = 0x0f;
  r.FSMC_AccessMode = FSMC_AccessMode_B;

  FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM4;	   	//扩展NOR BANK 的第1个子BANK
  FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;//不使用总线复用
																	                                            
  FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_NOR;//扩展类型为NOR FLASH
  FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;//扩展总线宽度为16位
																	                                          
  FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
																	
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
																	
  FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
  FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
  FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Enable;
  FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
  FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &r;	  	
  FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &w;	  
 
  FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure); 							//根据指定参数初始化结构体
  										
  FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM4, ENABLE); 							//使能FSMC_Bank1_NORSRAM4 内存

}
/**********************************************************************************
** Function name:       void LCD_Init(void)
** Descriptions:        LCD初始化
** input parameters:  	无
** output parameters:   无
** Returned value:      无
************************************************************************************/
void LCD_Init(void)
{
	LCD_IOConfig();
  FSMC_LCDInit();
	Delay_10ms(5);                     // delay 50 ms 
	Delay_10ms(5);                     // delay 50 ms 
	DeviceCode = Read_LCDReg(0x0000);       // 读驱动IC
	Delay_10ms(5);                     // delay 50 ms
	//printf("lcd id is 0x%x",DeviceCode);
	//DeviceCode=0x8999;
	if(DeviceCode==0x8999)	           // 对应的驱动IC为SSD1298
	{
		/*-----   Start Initial Sequence ------*/
		LCD_WriteReg(0x00, 0x0001); /*Start internal OSC */
		LCD_WriteReg(0x01, 0x3B3F); /*Driver output control */
		LCD_WriteReg(0x02, 0x0600); /* set 1 line inversion	*/
		/*-------- Power control setup --------*/
		LCD_WriteReg(0x0C, 0x0007); /* Adjust VCIX2 output voltage */
		LCD_WriteReg(0x0D, 0x0006); /* Set amplitude magnification of VLCD63 */
		LCD_WriteReg(0x0E, 0x3200); /* Set alternating amplitude of VCOM */
		LCD_WriteReg(0x1E, 0x00BB); /* Set VcomH voltage */
		LCD_WriteReg(0x03, 0x6A64); /* Step-up factor/cycle setting  */
		/*-------- RAM position control --------*/
		LCD_WriteReg(0x0F, 0x0000); /* Gate scan position start at G0 */
		LCD_WriteReg(0x44, 0xEF00); /* Horizontal RAM address position */
		LCD_WriteReg(0x45, 0x0000); /* Vertical RAM address start position*/
		LCD_WriteReg(0x46, 0x013F); /* Vertical RAM address end position */
		/* ------ Adjust the Gamma Curve -------*/
		LCD_WriteReg(0x30, 0x0000);
		LCD_WriteReg(0x31, 0x0706);
		LCD_WriteReg(0x32, 0x0206);
		LCD_WriteReg(0x33, 0x0300);
		LCD_WriteReg(0x34, 0x0002);
		LCD_WriteReg(0x35, 0x0000);
		LCD_WriteReg(0x36, 0x0707);
		LCD_WriteReg(0x37, 0x0200);
		LCD_WriteReg(0x3A, 0x0908);
		LCD_WriteReg(0x3B, 0x0F0D);
		/*--------- Special command -----------*/
		LCD_WriteReg(0x28, 0x0006); /* Enable test command	*/
		LCD_WriteReg(0x2F, 0x12EB); /* RAM speed tuning	 */
		LCD_WriteReg(0x26, 0x7000); /* Internal Bandgap strength */
		LCD_WriteReg(0x20, 0xB0E3); /* Internal Vcom strength */
		LCD_WriteReg(0x27, 0x0044); /* Internal Vcomh/VcomL timing */
		LCD_WriteReg(0x2E, 0x7E45); /* VCOM charge sharing time */ 
		/*--------- Turn On display ------------*/
		LCD_WriteReg(0x10, 0x0000); /* Sleep mode off */
		Delay_10ms(3);              /* Wait 30mS  */
		LCD_WriteReg(0x11, 0x6870); /* Entry mode setup. 262K type B, take care on the data bus with 16it only */
		LCD_WriteReg(0x07, 0x0033); /* Display ON	*/
	}		

  if(DeviceCode==0x9325||DeviceCode==0x9328)//ILI9325
	{				
		LCD_WriteReg(LCD_REG_0, 0x0001); /* Start internal OSC. */
    LCD_WriteReg(LCD_REG_1, 0x0100); /* Set SS and SM bit */
    LCD_WriteReg(LCD_REG_2, 0x0700); /* Set 1 line inversion */
    LCD_WriteReg(LCD_REG_3, 0x1018); /* Set GRAM write direction and BGR=1. */
    LCD_WriteReg(LCD_REG_4, 0x0000); /* Resize register */
    LCD_WriteReg(LCD_REG_8, 0x0202); /* Set the back porch and front porch */
    LCD_WriteReg(LCD_REG_9, 0x0000); /* Set non-display area refresh cycle ISC[3:0] */
    LCD_WriteReg(LCD_REG_10, 0x0000); /* FMARK function */
    LCD_WriteReg(LCD_REG_12, 0x0000); /* RGB interface setting */
    LCD_WriteReg(LCD_REG_13, 0x0000); /* Frame marker Position */
    LCD_WriteReg(LCD_REG_15, 0x0000); /* RGB interface polarity */

    /* Power On sequence -----------------------------------------------------*/
    LCD_WriteReg(LCD_REG_16, 0x0000); /* SAP, BT[3:0], AP, DSTB, SLP, STB */
    LCD_WriteReg(LCD_REG_17, 0x0000); /* DC1[2:0], DC0[2:0], VC[2:0] */
    LCD_WriteReg(LCD_REG_18, 0x0000); /* VREG1OUT voltage */
    LCD_WriteReg(LCD_REG_19, 0x0000); /* VDV[4:0] for VCOM amplitude */
    Delay_10ms(130);                     /* Dis-charge capacitor power voltage (200ms) */
    LCD_WriteReg(LCD_REG_16, 0x17B0); /* SAP, BT[3:0], AP, DSTB, SLP, STB */
    LCD_WriteReg(LCD_REG_17, 0x0137); /* DC1[2:0], DC0[2:0], VC[2:0] */
    Delay_10ms(50);                      /* Delay 50 ms */
    LCD_WriteReg(LCD_REG_18, 0x0139); /* VREG1OUT voltage */
    Delay_10ms(50);                     /* Delay 50 ms */
    LCD_WriteReg(LCD_REG_19, 0x1d00); /* VDV[4:0] for VCOM amplitude */
    LCD_WriteReg(LCD_REG_41, 0x0013); /* VCM[4:0] for VCOMH */
    Delay_10ms(50);                       /* Delay 50 ms */
    LCD_WriteReg(LCD_REG_32, 0x0000); /* GRAM horizontal Address */
    LCD_WriteReg(LCD_REG_33, 0x0000); /* GRAM Vertical Address */

    /* Adjust the Gamma Curve (ILI9325)---------------------------------------*/
    LCD_WriteReg(LCD_REG_48, 0x0007);
    LCD_WriteReg(LCD_REG_49, 0x0302);
    LCD_WriteReg(LCD_REG_50, 0x0105);
    LCD_WriteReg(LCD_REG_53, 0x0206);
    LCD_WriteReg(LCD_REG_54, 0x0808);
    LCD_WriteReg(LCD_REG_55, 0x0206);
    LCD_WriteReg(LCD_REG_56, 0x0504);
    LCD_WriteReg(LCD_REG_57, 0x0007);
    LCD_WriteReg(LCD_REG_60, 0x0105);
    LCD_WriteReg(LCD_REG_61, 0x0808);

    /* Set GRAM area ---------------------------------------------------------*/
    LCD_WriteReg(LCD_REG_80, 0x0000); /* Horizontal GRAM Start Address */
    LCD_WriteReg(LCD_REG_81, 0x00EF); /* Horizontal GRAM End Address */
    LCD_WriteReg(LCD_REG_82, 0x0000); /* Vertical GRAM Start Address */
    LCD_WriteReg(LCD_REG_83, 0x013F); /* Vertical GRAM End Address */
	
    LCD_WriteReg(LCD_REG_96,  0xA700); /* Gate Scan Line(GS=1, scan direction is G320~G1) */
    LCD_WriteReg(LCD_REG_97,  0x0001); /* NDL,VLE, REV */
    LCD_WriteReg(LCD_REG_106, 0x0000); /* set scrolling line */

    /* Partial Display Control -----------------------------------------------*/
    LCD_WriteReg(LCD_REG_128, 0x0000);
    LCD_WriteReg(LCD_REG_129, 0x0000);
    LCD_WriteReg(LCD_REG_130, 0x0000);
    LCD_WriteReg(LCD_REG_131, 0x0000);
    LCD_WriteReg(LCD_REG_132, 0x0000);
    LCD_WriteReg(LCD_REG_133, 0x0000);

    /* Panel Control ---------------------------------------------------------*/
    LCD_WriteReg(LCD_REG_144, 0x0010);
    LCD_WriteReg(LCD_REG_146, 0x0000);
    LCD_WriteReg(LCD_REG_147, 0x0003);
    LCD_WriteReg(LCD_REG_149, 0x0110);
    LCD_WriteReg(LCD_REG_151, 0x0000);
    LCD_WriteReg(LCD_REG_152, 0x0000);

    /* set GRAM write direction and BGR = 1 */
    /* I/D=00 (Horizontal : increment, Vertical : decrement) */
    /* AM=1 (address is updated in vertical writing direction) */
    //LCD_WriteReg(LCD_REG_3, 0x1018);  //横屏
    LCD_WriteReg(LCD_REG_3, 0x1030); 
    LCD_WriteReg(LCD_REG_7, 0x0133); /* 262K color and display ON */ 			
	}
	else if(DeviceCode==0x9919)  //SSD2119
	{
	  LCD_WriteReg(0x28,0x0006);
	  LCD_WriteReg(0x00,0x0001);
	  LCD_WriteReg(0x10,0x0000);
	  LCD_WriteReg(0x01,0x72ef);//
	  LCD_WriteReg(0x02,0x0600);
	  LCD_WriteReg(0x03,0x6a38);
	  LCD_WriteReg(0x11,0x6874);//74	
		//  RAM WRITE DATA MASK
		LCD_WriteReg(0x0f,0x0000); 
		//  RAM WRITE DATA MASK
		LCD_WriteReg(0x0b,0x5308);
		LCD_WriteReg(0x0c,0x0000); //3
		LCD_WriteReg(0x0d,0x000a);
		LCD_WriteReg(0x0e,0x2e00);  //0030
		LCD_WriteReg(0x1e,0x00be);
		LCD_WriteReg(0x25,0x8000);
		LCD_WriteReg(0x26,0x7800);
		LCD_WriteReg(0x27,0x0078);
		LCD_WriteReg(0x4e,0x0000);
		LCD_WriteReg(0x4f,0x0000);
		LCD_WriteReg(0x12,0x08d9);
		// -----------------Adjust the Gamma Curve----//
		LCD_WriteReg(0x30,0x0000);	 //0007	
	  LCD_WriteReg(0x31,0x0104);	   //0203 
		LCD_WriteReg(0x32,0x0100);		//0001
		LCD_WriteReg(0x33,0x0305);	  //0007
		LCD_WriteReg(0x34,0x0505);	  //0007
		LCD_WriteReg(0x35,0x0305);		 //0407	
		LCD_WriteReg(0x36,0x0707);		 //0407	
		LCD_WriteReg(0x37,0x0300);		  //0607 
		LCD_WriteReg(0x3a,0x1200);		 //0106	
		LCD_WriteReg(0x3b,0x0800);
		LCD_WriteReg(0x07,0x0033);
		  //LCD_WriteReg(0x11,0x6040|0x30);
	}
	Delay_10ms(5);	                       //延时50ms
	LCD_Clear(BLACK);
}

/************************************************************************
** Function name:       void LCD_SetCursor(u16 Xpos, u16 Ypos)
** Descriptions:        设置光标位置
** input parameters:    x轴坐标：uint8_t Xpos ;  y轴坐标：uint16_t Ypos
** output parameters:   无
** Returned value:      无
**************************************************************************/
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{	
	if(DeviceCode == 0x8999||DeviceCode==0x9919)
 	{
 		LCD_WriteReg(0x4E, Xpos);
 		LCD_WriteReg(0x4F, Ypos);
  }
	else 
 	{
		LCD_WriteReg(0x20, Xpos);
		LCD_WriteReg(0x21, Ypos);
	}
}


/***********************************************************
** Function name:       void LCD_Clear(uint16_t Color)
** Descriptions:        刷屏函数
** input parameters:    颜色值
** output parameters:   无
** Returned value:      无
************************************************************/
void LCD_Clear(uint16_t Color)
{
	uint32_t index=0;      
	LCD_SetCursor(0x00,0x0000);          //设置光标位置 
	LCD_WriteRAM_Prepare();              //开始写入GRAM	  
	for(index=0;index<76800;index++)
	{
		LCD_WR_DATA(Color);	
	}
}

/****************************************************************************
* 名    称：u16 LCD_GetPoint(u16 x,u16 y)
* 功    能：获取指定座标的颜色值
* 入口参数：x      行座标
*           y      列座标
* 出口参数：当前座标颜色值
* 说    明：
* 调用方法：CurrentColor = LCD_GetPoint(10,10);
* 备注：此函数有待验证，此处屏蔽
****************************************************************************/
//u16 LCD_GetPoint(u16 x,u16 y)
//{
//  LCD_SetCursor(x,y);
//  if(DeviceCode=0X8999)
//    return (LCD_ReadRAM());
//  else
//    return (LCD_BGRtoRGB(LCD_ReadRAM()));
//}


/***********************************************************************
** Function name:       void LCD_DrawPoint(uint16_t xsta, uint16_t ysta)
** Descriptions:        打点
** input parameters:    xsta X起始坐标 0~239;ysta	Y起始坐标 0~319
** output parameters:   无
** Returned value:      无
***********************************************************************/
void LCD_DrawPoint(uint16_t xsta, uint16_t ysta)
{
	LCD_SetCursor(xsta,ysta);           //设置光标位置  
	LCD_WR_REG(0x22);                   //开始写入GRAM 
	LCD_WR_DATA(POINT_COLOR); 
}

/*************************************************************************************************************
** Function name:       void LCD_XYRAM(uint16_t xstart ,uint16_t ystart ,uint16_t xend ,uint16_t yend)
** Descriptions:        设置显存区域,用完以后要恢复起点(0,0)和终点(239,319)(ili9325/ili9328调用)      
** input parameters:    xsta 起始X坐标
		                    ysta 起始Y坐标
		                    xend 结束X坐标
		                    yend 结束Y坐标
** output parameters:   无
** Returned value:      无
**************************************************************************************************************/
void LCD_XYRAM(uint16_t xsta ,uint16_t ysta ,uint16_t xend ,uint16_t yend)
{
	LCD_WriteReg(0x0050, xsta); // 设置横坐标GRAM起始地址
	LCD_WriteReg(0x0051, xend-1); // 设置横坐标GRAM结束地址
	LCD_WriteReg(0x0052, ysta); // 设置纵坐标GRAM起始地址
	LCD_WriteReg(0x0053, yend-1); // 设置纵坐标GRAM结束地址
}

/*******************************************************************************************************
** Function name:       void LCD_WindowMax (unsigned int x,unsigned int y,unsigned int x_end,unsigned int y_end) 
** Descriptions:        设置窗口最大化240*320
** input parameters:    xsta X起始坐标 0~239;ysta	Y起始坐标 0~319
** output parameters:   无
** Returned value:      无
******************************************************************************************************/
void LCD_WindowMax (unsigned int x,unsigned int y,unsigned int x_end,unsigned int y_end) 
{
	if(DeviceCode==0x8999||DeviceCode==0x9919)
		{
	    LCD_WriteReg(0x44,x|((x_end-1)<<8));
	    LCD_WriteReg(0x45,y);
	    LCD_WriteReg(0x46,y_end-1);
		}
		else
		{
		  LCD_WriteReg(0x50, x);	   	       //水平方向GRAM起始地址
  	  LCD_WriteReg(0x51, x+x_end); 	     //水平方向GRAM结束地址 
  	  LCD_WriteReg(0x52, y);		  	     //垂直方向GRAM起始地址
  	  LCD_WriteReg(0x53, y+y_end);       //垂直方向GRAM结束地址
  	  //LCD_SetCursor(x, y);			       //设置光标位置
		}
}


/*************************************************************************************************************
** Function name:       void LCD_Fill(uint8_t xsta, uint16_t ysta, uint8_t xend, uint16_t yend, uint16_t colour)
** Descriptions:        在指定矩形区域填充指定颜色，区域大小(xend-xsta)*(yend-ysta)
** input parameters:    xsta 起始X坐标
		                    ysta 起始Y坐标
		                    xend 结束X坐标
		                    yend 结束Y坐标
		                    color 待填充颜色
** output parameters:   无
** Returned value:      无
**************************************************************************************************************/
void LCD_Fill(uint8_t xsta, uint16_t ysta, uint8_t xend, uint16_t yend, uint16_t colour)
{                    	
	u32 n;
  if(DeviceCode==0x8999||DeviceCode==0x9919)	           // 对应的驱动IC为SSD1298
	{	
	  LCD_WindowMax (xsta, ysta, xend, yend); //设置窗口	
	}
	else
	{
		LCD_XYRAM(xsta ,ysta ,xend ,yend);     // 设置GRAM坐标
	}
	LCD_SetCursor(xsta,ysta);                 //设置光标位置 
	LCD_WriteRAM_Prepare();                   //开始写入GRAM*6 	   	   
	n=(u32)(yend-ysta+1)*(xend-xsta+1);    
	while(n--){LCD_WR_DATA(colour);}          //显示所填充的颜色
	 
	LCD_WindowMax (0, 0, 240, 320);      	    //恢复窗口
}  


/****************************************************************************************************
** Function name:       void LCD_DrawLine(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend)
** Descriptions:        画线
** input parameters:    xsta X起始坐标
	   	                  ysta Y起始坐标
		                    xend X终点坐标
		                    yend Y终点坐标	
** output parameters:   无
** Returned value:      无
****************************************************************************************************/
void LCD_DrawLine(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend)
{
    u16 x, y, t;
	if((xsta==xend)&&(ysta==yend))LCD_DrawPoint(xsta, ysta);
	else if(abs(yend-ysta)>abs(xend-xsta))                    //斜率大于1 
	{
		if(ysta>yend) 
		{
			t=ysta;
			ysta=yend;
			yend=t; 
			t=xsta;
			xsta=xend;
			xend=t; 
		}
		for(y=ysta;y<yend;y++)                                   //以y轴为基准 
		{
			x=(u32)(y-ysta)*(xend-xsta)/(yend-ysta)+xsta;
			LCD_DrawPoint(x, y);  
		}
	}
	else                                                       //斜率小于等于1 
	{
		if(xsta>xend)
		{
			t=ysta;
			ysta=yend;
			yend=t;
			t=xsta;
			xsta=xend;
			xend=t;
		}   
		for(x=xsta;x<=xend;x++)                                   //以x轴为基准
		{
			y =(u32)(x-xsta)*(yend-ysta)/(xend-xsta)+ysta;
			LCD_DrawPoint(x,y); 
		}
	} 
} 

/****************************************************************************************************
** Function name:       void LCD_DrawHVLine(uint8_t xsta, uint16_t ysta, uint16_t Length,uint16_t Color,uint8_t Direction)
** Descriptions:        绘制水平或垂直的线条
** input parameters:    xsta X起始坐标
	   	                  ysta Y起始坐标
		                    Length 线条长度
		                    Color 线条颜色
                        Direction: Horizontal 水平; Vertical 垂直
** output parameters:   无
** Returned value:      无
****************************************************************************************************/
void LCD_DrawHVLine(uint8_t xsta, uint16_t ysta, uint16_t Length,uint16_t Color,uint8_t Direction)
{
  uint32_t i = 0;
  
  LCD_SetCursor(xsta, ysta);

  if(Direction == Horizontal)
  {
		 //LCD_WindowMax(ysta,xsta,ysta+Length,xsta-1);	
    LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
    for(i = 0; i < Length; i++)
    {
      LCD_WriteRAM(Color);
    }
  }
  else
  {
    for(i = 0; i < Length; i++)
    {
			
      LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
      LCD_WriteRAM(Color);
      xsta++;
      LCD_SetCursor(xsta, ysta);
    }
  }
}
/*****************************************************************************
** Function name:       void LCD_DrawCircle(uint16_t x0, uint16_t y0, uint8_t r)
** Descriptions:        画圆
** input parameters:    x0 中心点横坐标
	                      y0 中心点纵坐标
		                    r  半径
** output parameters:   无
** Returned value:      无
******************************************************************************/
void LCD_DrawCircle(uint16_t x0, uint16_t y0, uint8_t r)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);                                        //判断下个点位置的标志
	while(a<=b)
	{
		LCD_DrawPoint(x0-b,y0-a);                         //3           
		LCD_DrawPoint(x0+b,y0-a);                         //0           
		LCD_DrawPoint(x0-a,y0+b);                         //1       
		LCD_DrawPoint(x0-b,y0-a);                         //7           
		LCD_DrawPoint(x0-a,y0-b);                         //2             
		LCD_DrawPoint(x0+b,y0+a);                         //4               
		LCD_DrawPoint(x0+a,y0-b);                         //5
		LCD_DrawPoint(x0+a,y0+b);                         //6 
		LCD_DrawPoint(x0-b,y0+a);             
		a++;

		/*使用Bresenham算法画圆*/     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 
		LCD_DrawPoint(x0+a,y0+b);
	}
} 


/*****************************************************************************
** Function name:       void Draw_Circle(uint16_t x0, uint16_t y0, uint8_t r)
** Descriptions:        画矩形
** input parameters:    xsta X起始坐标
	                      ysta Y起始坐标
		                    xend X结束坐标
		                    yend Y结束坐标
** output parameters:   无
** Returned value:      无
******************************************************************************/
void LCD_DrawRectangle(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend)
{
	LCD_DrawLine(xsta,ysta,xend,ysta);
	LCD_DrawLine(xsta,ysta,xsta,yend);
	LCD_DrawLine(xsta,yend,xend,yend);
	LCD_DrawLine(xend,ysta,xend,yend);
} 

void LCD_DrawPixel(uint16_t xsta, uint16_t ysta, uint16_t color)
{
	LCD_SetCursor(xsta, ysta);                                          //设置光标位置                  */	             
	LCD_WriteRAM_Prepare();
	LCD_WR_DATA(color);
}
/***************************************************************************************************
** Function name:       void LCD_ShowChar(u8 x, u16 y, u8 num, u8 size, u16 PenColor, u16 BackColor)
** Descriptions:        显示一个字符
** input parameters:    x，y      起始坐标（x:0~234 y:0~308）
		                    num       字符ASCII码值
		                    size      字符大小，使用默认8*16
		                    PenColor  字体颜色
		                    BackColor 字体背景颜色
** output parameters:   无
** Returned value:      无
****************************************************************************************************/
void LCD_ShowChar(u8 x, u16 y, u8 num, u8 size, u16 PenColor, u16 BackColor)
{       
  u8 temp;
  u8 pos,t;
  if(x>MAX_CHAR_POSX||y>MAX_CHAR_POSY)return;		 
	if(DeviceCode == 0x8999)                       //驱动IC为SSD1298
	{
    if(XY)                                       //竖屏显示
 	   {
 		   LCD_WindowMax(x,y,x+size/2,y+size);	       //设置窗口
 		   LCD_WriteReg(0x11, 0x6870);
 		   LCD_SetCursor(x, y); 
 	  }
   else                                          //横屏显示
 	  {
 		  LCD_WindowMax(x,y-size/2,x+size,y);	       //设置窗口		
			LCD_WriteReg(0x11,0x6858);
 		  LCD_SetCursor(x, y-1);                     //设置光标位置
 	  }  
	}
	else if(DeviceCode==0x9919)                       //驱动IC为SSD2119
	{
    if(XY)                                          //竖屏显示
 	   {
			  LCD_WindowMax(x,y,x+size/2,y+size);	          //设置窗口
				LCD_WriteReg(0x01, 0x32EF);                    //输出控制寄存器01
				LCD_WriteReg(0x11, 0x6038);                    //设置GRAM自增方向
 		    LCD_SetCursor(y, x); 
 	  }
    else                                          //横屏显示
 	  {     
			LCD_WindowMax(x,y,x+size,y+size/2);        //设置窗口		
			LCD_SetCursor(y, x);                       //设置光标位置
 	  }  
	}
  else if(DeviceCode == 0x9325 || DeviceCode == 0x9328) 
	{ 
		if(XY)
		{
		  LCD_WindowMax(x,y,(size/2-1),size-1);            //设置窗口	驱动IC为ILI9325/9328
			LCD_WriteReg(LCD_REG_3, 0x1030);
	    LCD_SetCursor(x, y);                             //设置光标位置 
		}
		else
		{
		  LCD_WindowMax(x,y+(size/2),size-1,(size/2)-1);    
		  LCD_WriteReg(LCD_REG_3, 0x1098);//0x1068
	    LCD_SetCursor(x, y);   
		}
	}
	LCD_WriteRAM_Prepare();                        //开始写入GRAM   
	num=num-' ';                                   //得到偏移后的值 
	for(pos=0;pos<size;pos++)
	{
		if(size==12)
			temp=asc2_1206[num][pos];                  //调用1206字体
		else 
			temp=asc2_1608[num][pos];		               //调用1608字体
		for(t=0;t<size/2;t++)
	    {                 
	      if(temp&0x01)			                       //从低位开始
			   {
				   LCD_WR_DATA(PenColor);                //画字体颜色 一个点
			   }
			 else 
				  LCD_WR_DATA(BackColor);	               //画背景颜色 一个点      
	        temp>>=1; 
	    }
	}			
	LCD_WindowMax(0x0000,0x0000,240,320);	         //恢复窗体大小
	
} 
/**********************************************************************************
** Function name:       unsigned int mypow(unsigned int m,unsigned int n)
** Descriptions:        m的n次方 用于取出数值每一位
** input parameters:  	m进制：unsigned int m
**											n次方：unsigned int n						
** output parameters:   m^n
** Returned value:      无
************************************************************************************/
u32 mypow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}			 
 
/*********************************************************************************************
** Function name:       void LCD_ShowNum(u8 x,u8 y,u32 num,u8 len, u16 PenColor, u16 BackColor)
** Descriptions:        显示数字(0~4294967295);
** input parameters:  	x起始：     u8 x
  											y起始：     u8 y
  											数值:       num
    										有效长度：  len		
                        字体颜色:   PenColor
                        背景颜色：  BackColor
** output parameters:   无
** Returned value:      无
************************************************************************************************/
void LCD_ShowNum(u8 x,u8 y,u32 num,u8 len, u16 PenColor, u16 BackColor)
{    
	u8 size = 16;                                          // 这里使用默认的16*8       	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+(size/2)*t,y,' ',size, PenColor, BackColor);
				LCD_ShowChar(x+(size/2)*t,y,'0',size, PenColor, BackColor);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size, PenColor, BackColor); 
	}
} 
  

/*********************************************************************************************
** Function name:       void LCD_ShowCharString(uint16_t x, uint16_t y, const uint8_t *p, uint16_t PenColor, uint16_t BackColor)
** Descriptions:        显示字符(此函数不能单独调用)
** input parameters:  	x，y      起始坐标
	                      p         指向字符串起始地址
		                    PenColor  字符颜色
		                    BackColor 背景颜色
** output parameters:   无
** Returned value:      无
************************************************************************************************/
void LCD_ShowCharString(uint16_t x, uint16_t y, const uint8_t *p, uint16_t PenColor, uint16_t BackColor)
{   
	uint8_t size = 16;                                    //字符大小默认16*8
	 
    if(x>MAX_CHAR_POSX){x=0;y+=size;}			              //超出X轴字体最小单位，换行
    if(y>MAX_CHAR_POSY){y=x=0;LCD_Clear(WHITE);}	      //超出Y轴字体最小单位，回到原点，并且清屏
    LCD_ShowChar(x, 320-y, *p, size, PenColor, BackColor);	//0表示非叠加方式
 
}


/**********************************************************************************
** Function name:       u16 findHzIndex(u8 *hz) 
** Descriptions:        索引汉字存储的内存地址
** input parameters:  	hz				
** output parameters:   无
** Returned value:      无
************************************************************************************/
u16 findHzIndex(u8 *hz)                               // 在自定义汉字库在查找所要显示 
                                                      // 的汉字的位置 
{
	u16 i=0;
	FNT_GB16 *ptGb16 = (FNT_GB16 *)GBHZ_16;		          //ptGb16指向GBHZ_16
	while(ptGb16[i].Index[0] > 0x80)
	{
	    if ((*hz == ptGb16[i].Index[0]) && (*(hz+1) == ptGb16[i].Index[1])) //汉字用两位来表示地址码
		{
	        return i;
	    }
	    i++;
	    if(i > (sizeof((FNT_GB16 *)GBHZ_16) / sizeof(FNT_GB16) - 1))  //搜索下标约束 
	    {
		    break;
	    }
	}
	return 0;
}

	
/************************************************************************************************
** Function name:       void WriteOneHz(u16 x0, u16 y0, u8 *pucMsk, u16 PenColor, u16 BackColor)
** Descriptions:        显示一个汉字(此函数不能单独作为汉字字符显示)
** input parameters:  	x0,y0      起始坐标
		                    *pucMsk    指向字符串地址
		                    PenColor	 字符颜色
		                    BackColor  背景颜色			
** output parameters:   无
** Returned value:      无
************************************************************************************************/
void WriteOneHz(u16 x0, u16 y0, u8 *pucMsk, u16 PenColor, u16 BackColor)
{
   u16 i,j;
   u16 mod[16];                                      //当前字模 16*16 
   u16 *pusMsk;                                      //当前字库地址  
   u16 y;

	u16 size = 16;                                      //汉字默认大小16*16

    pusMsk = (u16 *)pucMsk;

    for(i=0; i<16; i++)                                //保存当前汉字点阵式字模 
    {
        mod[i] = *pusMsk;                              //取得当前字模，半字对齐访问   
        mod[i] = ((mod[i] & 0xff00) >> 8) | ((mod[i] & 0x00ff) << 8);//字模交换高低字节
		    pusMsk = pusMsk+1;
    }
    y = x0;
		if(DeviceCode == 0x8999)                       //驱动IC为SSD1298 
		{		  
			if(XY)                                       //竖屏显示
 	    {
 		    LCD_WindowMax(x0,y0,x0+size,y0+size);	       //设置窗口
 		    LCD_WriteReg(0x11, 0x6870);
 		    LCD_SetCursor(x0, y0); 
 	    }
     else                                          //横屏显示
 	   {
 		  LCD_WindowMax(x0,y0-size,x0+size,y0);	       //设置窗口	
 		  LCD_WriteReg(0x11, 0x6858);                    //
 		  LCD_SetCursor(x0, y0-1);                     //设置光标位置
 	   }  
		}
		else if(DeviceCode==0x9919)                     //驱动IC为SSD2119 
		{		  
			if(XY)                                       //竖屏显示
 	    {

		    LCD_WindowMax(x0,y0,x0+size,y0+size);	       //设置窗口
				LCD_WriteReg(0x01, 0x32EF);                    //输出控制寄存器01
				LCD_WriteReg(0x11, 0x6038);                    //设置GRAM自增方向
 		    LCD_SetCursor(y0, x0); 
 	    }
     else                                          //横屏显示
 	   {
			 LCD_WindowMax(x0,y0,x0+size,y0+size);	
			 LCD_SetCursor(y0, x0);                     //设置光标位置
 	   }  
		}
		
	  else if(DeviceCode == 0x9325 || DeviceCode == 0x9328)   //驱动IC为ILI9325/9328
		{ 
      LCD_WindowMax(x0,y0,(size-1),size-1); 	
       if(XY)	LCD_WriteReg(LCD_REG_3, 0x1030);
       else 	LCD_WriteReg(LCD_REG_3, 0x1098);	
      LCD_SetCursor(x0, y0); 			
	  }
                               
	  LCD_WriteRAM_Prepare();                              //开始写入GRAM  
    for(i=0; i<16; i++)                                   //16行 
    {                                              
      for(j=0; j<16; j++)                                 //16列  
       {
		    if((mod[i] << j) & 0x8000)                     //显示第i行 共16个点 
          { 
			      LCD_WriteRAM(PenColor);
          } 
			  else 
			    {
            LCD_WriteRAM(BackColor);                   //用读方式跳过写空白点的像素
			    } 
       }
        y++;
    }
	LCD_WindowMax(0x0000,0x0000,240,320);  	             //恢复窗体大小
}

/**********************************************************************************
** Function name:       void LCD_ShowHzString(u16 x0, u16 y0, u8 *pcStr, u16 PenColor, u16 BackColor)
** Descriptions:        显示汉字字符串(不能单独调用)
** input parameters:  	x0，y0    起始坐标
		                    pcStr     指向字符串地址
		                    PenColor  字体颜色
		                    BackColor 字体背景				
** output parameters:   无
** Returned value:      无
************************************************************************************/
void LCD_ShowHzString(u16 x0, u16 y0, u8 *pcStr, u16 PenColor, u16 BackColor)
{

	u16 usIndex;
	u8 size = 16; 
	FNT_GB16 *ptGb16 = 0;    
    ptGb16 = (FNT_GB16 *)GBHZ_16; 

	if(x0>MAX_HZ_POSX){x0=0;y0+=size;}			            //超出X轴字体最小单位，换行
   // if(y0>MAX_HZ_POSY){y0=x0=0;LCD_Clear(WHITE);}	    //超出Y轴字体最小单位，回到原点，并且清屏

	usIndex = findHzIndex(pcStr);
	WriteOneHz(x0, y0, (u8 *)&(ptGb16[usIndex].Msk[0]),  PenColor, BackColor); //显示一个汉字
}

/**********************************************************************************
** Function name:       void LCD_DisplayStr(unsigned int x, unsigned int y, unsigned char *s,u16 PenColor, u16 BackColor)
** Descriptions:        显示字符串(可以和汉字混合显示,支持横竖屏切换)
** input parameters:  	x，y    起始坐标
		                    *s     指向字符串地址
		                    PenColor  字体颜色
		                    BackColor 字体背景				
** output parameters:   无
** Returned value:      无
************************************************************************************/
void LCD_DisplayStr(unsigned int x, unsigned int y, unsigned char *s,u16 PenColor, u16 BackColor) 
{
	if(XY==0)
	{
		u16 change=0;
	  change=x;
	  x=y;
	  y=change;
	}
  while (*s) 
	{  	
		if(*s>0x80)       //显示汉字
		{
			if(XY)
		  {	
				LCD_ShowHzString(x, y, s, PenColor, BackColor);		
			  x += 16;
			}
			else	
			{
         switch(DeviceCode)
				 {
				   case 0x8999:
						LCD_ShowHzString(x, 319-y, s, PenColor, BackColor);
					 break;
					  case 0x9919:
						LCD_ShowHzString(x, y, s, PenColor, BackColor);
					 break;
					 default:
						 LCD_ShowHzString(x, 303-y, s, PenColor, BackColor);//319-16
					 break;
				 }
			  y += 16;
			}
		  s += 2;
		}
  	else                                                 //非汉字
		{		
		 if(XY)                                               //竖屏显示
		  {
		    LCD_ShowChar(x, y, *s++, 16, PenColor, BackColor);
			  x+=Font_Size/2;
			 if(x>=232)
			 {
				x=0;
				y+=Font_Size;
				if(y>=304)y=0;
			 }
		}
		else                                                  //横屏显示
		{
		  switch(DeviceCode)
			{
				case 0x8999:
			  LCD_ShowChar(x, 319-y, *s++, 16, PenColor, BackColor);
				break;
				case 0x9919:
			  LCD_ShowChar(x, y, *s++, 16, PenColor, BackColor);
				break;
				default:
					LCD_ShowChar(x, 303-y, *s++, 16, PenColor, BackColor);//319-16
				break;
			}
			y+=Font_Size/2;
			if(y>=312)
			{
				y=0;
				x+=Font_Size;
				if(x>=224)y=0;
			}
		}
  }
  }
}

/**********************************************************************************
** Function name:       u16 LCD_RGBtoBGR(u16 Color)
** Descriptions:        RRRRRGGGGGGBBBBB 改为 BBBBBGGGGGGRRRRR 格式
** input parameters:  	Color      RGB 颜色值		
** output parameters:   BGR 颜色值
** Returned value:      无
************************************************************************************/
u16 LCD_RGBtoBGR(u16 Color)
{						   
  u16  r, g, b, bgr;

  b = (Color>>0)  & 0x1f;	                    //提取B 
  g = (Color>>5)  & 0x3f;	                    //中间六位
  r = (Color>>11) & 0x1f;	                    //提取R  
  
  bgr =  (b<<11) + (g<<5) + (r<<0);

  return( bgr );
}


/**********************************************************************************
** Function name:       void LCD_DrawPicture(u16 StartX,u16 StartY,u16 Xend,u16 Yend,u8 *pic)
** Descriptions:        图片取模格式为水平扫描，16位颜色模式
** input parameters:  	StartX     行起始座标
                        StartY     列起始座标
                        EndX       行结束座标
                        EndY       列结束座标
                        pic        图片头指针	
** output parameters:   无
** Returned value:      无
************************************************************************************/
void LCD_DrawPicture(u16 StartX,u16 StartY,u16 Xend,u16 Yend,u8 *pic)
{
	static	u16 i=0,j=0;
	u16 *bitmap = (u16 *)pic;
	if(DeviceCode==0x8999||DeviceCode==0x9919)
	 {
	   LCD_WindowMax(StartX, StartY, Xend, Yend);	//设置图片显示窗口大小
	 }
	 else
	 {
	   LCD_XYRAM(StartX ,StartY ,Xend ,Yend);     // 设置GRAM坐标
	 }
	
	LCD_SetCursor(StartX,StartY);
	LCD_WriteRAM_Prepare();
	for(j=0; j<Yend-StartY; j++)
	{
		for(i=0; i<Xend-StartX; i++) LCD_WriteRAM(*bitmap++); 	
	}
	
	LCD_WindowMax(0, 0, 240, 320);              //恢复窗口
}

