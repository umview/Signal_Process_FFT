#ifndef __HZ_H
#define __HZ_H
const unsigned char HZ[][32]={

	{0x02, 0x00, 0x02, 0x00, 0x02, 0x10, 0x7F, 0x78, 0x42, 0x10, 0x42, 0x10, 0x7F, 0x70, 0x42, 0x10,
	0x42, 0x10, 0x7F, 0x70, 0x42, 0x10, 0x02, 0x00, 0x02, 0x04, 0x02, 0x04, 0x01, 0x7C, 0x00, 0x00,}, //电0
	
	{0x00, 0x00, 0x40, 0x48, 0x37, 0x7C, 0x10, 0x40, 0x00, 0x10, 0x61, 0x08, 0x27, 0x7C, 0x08, 0x04,
	0x11, 0x50, 0x21, 0x50, 0x61, 0x50, 0x21, 0x50, 0x21, 0x52, 0x21, 0x52, 0x22, 0x4E, 0x24, 0x40 }, //流1
	
	{0x00, 0x08, 0x3F, 0x7C, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x08, 0x2F, 0x7C, 
	0x20, 0x00, 0x20, 0x20, 0x20, 0x18, 0x20, 0x08, 0x40, 0x00, 0x40, 0x04, 0x1F, 0x7E, 0x00, 0x00 }, //压2
	  
	{0x10, 0x04, 0x13, 0x7E, 0x54, 0x20, 0x5E, 0x44, 0x51, 0x7E, 0x55, 0x04, 0x7F, 0x24, 0x11, 0x24,
	0x55, 0x24, 0x55, 0x24, 0x55, 0x24, 0x15, 0x24, 0x08, 0x20, 0x10, 0x50, 0x20, 0x0C, 0x43, 0x04 }, //频3
	     
	{0x02, 0x00, 0x01, 0x08, 0x7F, 0x7C, 0x01, 0x00, 0x42, 0x44, 0x27, 0x08, 0x11, 0x10, 0x22, 0x48, 
	0x4F, 0x64, 0x01, 0x20, 0x01, 0x04, 0x7F, 0x7E, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00 }, //率4
	
	{0x08, 0x20, 0x08, 0x28, 0x7F, 0x7C, 0x08, 0x20, 0x0F, 0x60, 0x08, 0x20, 0x0F, 0x60, 0x08, 0x24, 
	0x7F, 0x7E, 0x04, 0x40, 0x09, 0x30, 0x31, 0x4E, 0x4F, 0x64, 0x01, 0x00, 0x01, 0x10, 0x3F, 0x78},  //基5
	
	{0x00, 0x40, 0x40, 0x40, 0x30, 0x40, 0x17, 0x7C, 0x04, 0x44, 0x04, 0x48, 0x44, 0x40, 0x0F, 0x78,
	0x14, 0x08, 0x25, 0x10, 0x64, 0x20, 0x24, 0x40, 0x28, 0x20, 0x29, 0x10, 0x32, 0x0E, 0x24, 0x04 },  //波6
	
	{0x00, 0x00, 0x40, 0x00, 0x30, 0x00, 0x11, 0x08, 0x01, 0x7C, 0x02, 0x08, 0x08, 0x50, 0x10, 0x40, 
	0x20, 0x40, 0x60, 0x40, 0x20, 0x20, 0x20, 0x20, 0x21, 0x10, 0x22, 0x08, 0x24, 0x0E, 0x28, 0x04 },  //次7
	
	{0x04, 0x20, 0x24, 0x24, 0x17, 0x28, 0x14, 0x30, 0x04, 0x22, 0x05, 0x22, 0x76, 0x5E, 0x10, 0x08,
	0x13, 0x7C, 0x12, 0x08, 0x12, 0x08, 0x13, 0x78, 0x16, 0x08, 0x1A, 0x08, 0x13, 0x78, 0x02, 0x08},   //谐8
	
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x3F, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x7F, 0x7E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   //二9
	
	{0x00, 0x00, 0x00, 0x08, 0x7F, 0x7C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x3F, 0x78, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x7F, 0x7E, 0x00, 0x00, 0x00, 0x00 },//三10
	
	{0x00, 0x00, 0x00, 0x04, 0x7F, 0x7E, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 
	0x44, 0x44, 0x44, 0x3C, 0x48, 0x04, 0x50, 0x04, 0x40, 0x04, 0x7F, 0x7C, 0x40, 0x04, 0x00, 0x00 },//四11
	
	{0x00, 0x00, 0x00, 0x08, 0x7F, 0x7C, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x10, 0x3F, 0x7C, 
	0x04, 0x10, 0x04, 0x10, 0x04, 0x10, 0x08, 0x10, 0x08, 0x10, 0x08, 0x14, 0x7F, 0x7E, 0x00, 0x00 },//五12
	
	{0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,0x00,0x00,
	0x00, 0x00, 0x00, 0x00, 0x18,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  //: 13
	
	{0x10, 0x08, 0x11, 0x7C, 0x14, 0x00, 0x7E, 0x78, 0x54, 0x08, 0x54, 0x08, 0x54, 0x78, 0x54, 0x04,
	0x55, 0x7E, 0x55, 0x24, 0x55, 0x24, 0x5D, 0x7C, 0x11, 0x24, 0x11, 0x24, 0x11, 0x7C, 0x11, 0x04 },//幅14
	
	{0x08, 0x40, 0x08, 0x48, 0x0F, 0x7C, 0x10, 0x40, 0x10, 0x48, 0x33, 0x7C, 0x52, 0x08, 0x13, 0x78, 
	0x12, 0x08, 0x13, 0x78, 0x12, 0x08, 0x13, 0x78, 0x12, 0x08, 0x12, 0x08, 0x1F, 0x7E, 0x10, 0x00} //值15
	
};
#endif
