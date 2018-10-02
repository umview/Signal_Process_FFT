#include "Function.h"
extern struct InfoStruct Info;
extern float SDS[4];
void GetPowerMag()
{
	signed short lX,lY;
   float X,Y,Mag;
   int16_t i;
  for(i=0; i<LEN/2; i++)
     {
         lX  = (Info.FArray[i] << 16) >> 16;
         lY  = (Info.FArray[i] >> 16);
         X = LEN * ((float)lX)/ 32768;
         Y = LEN * ((float)lY)/ 32768;
         Mag = sqrt(X * X + Y * Y) / LEN;
		 Info.omega[i]=(float)(1.0f*i*(float)(Info.SampleFreq)/(LEN/2.0f));
         if(i == 0)            
					 Info.lBufMagArray[i] = (unsigned long)(Mag * 32768);
         else
           Info.lBufMagArray[i] = (unsigned long)(Mag* 65536);
			Info.Amplitude[i]=(int16_t)(Info.lBufMagArray[i]*3.3f/4096.0f*2000.0f);
				 /***********************************/
			SDS[0]=(int16_t)(X*X*100.0f);
//			SDS[1]=(int16_t)(Y*Y*100.0f);
//			//SDS[2]=(int16_t)(Info.lBufMagArray[i]);
//			SDS[2]=(int16_t)Info.Amplitude[i];

//			SDS[3]=(int16_t)(Info.omega[i]*10.0f);
//			Plot(SDS);

    }

}
