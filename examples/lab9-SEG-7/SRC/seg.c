#include"DSP2833x_Device.h"
#include "DSP2833x_Examples.h"
 
#define	  SBIT3 	GpioDataRegs.GPBDAT.bit.GPIO59
#define	  SBIT2 	GpioDataRegs.GPBDAT.bit.GPIO58
#define	  SBIT1 	GpioDataRegs.GPBDAT.bit.GPIO60
#define	  SBIT0 	GpioDataRegs.GPBDAT.bit.GPIO61
#define	  uchar 	unsigned char

unsigned char const table[]={//共阴极0~f数码管编码
0xfc,0x60,0xda,0xf2,     //0~3
0x66,0xb6,0xbe,0xe0,    //4~7
0xfe,0xf6
}; 

void delay(Uint16 t);
void configio(void);


void delay(Uint16 t)
{
   Uint16 i;
   while(t--)
    {
      for(i=0;i<125;i++);
    }
}

void SEG_DATA(uchar d){
   Uint32 b=0;
   b=d;
   b<<=8;
   GpioDataRegs.GPCDAT.all = b;
}

void configio(void)
{
    EALLOW;
     GpioCtrlRegs.GPCMUX1.bit.GPIO72 = 0;  // XD7
     GpioCtrlRegs.GPCMUX1.bit.GPIO73 = 0;  // XD6
     GpioCtrlRegs.GPCMUX1.bit.GPIO74 = 0;  // XD5
     GpioCtrlRegs.GPCMUX1.bit.GPIO75 = 0;  // XD4
     GpioCtrlRegs.GPCMUX1.bit.GPIO76 = 0;  // XD3
     GpioCtrlRegs.GPCMUX1.bit.GPIO77 = 0;  // XD2
     GpioCtrlRegs.GPCMUX1.bit.GPIO78 = 0;  // XD1
     GpioCtrlRegs.GPCMUX1.bit.GPIO79 = 0;  // XD0 
	GpioCtrlRegs.GPCDIR.all |= 0x0000FF00;
	GpioCtrlRegs.GPCDIR.bit.GPIO64 = 1;
//	GpioCtrlRegs.GPAPUD.all |= 0xFFFFFFFF;
	GpioCtrlRegs.GPBMUX2.bit.GPIO58 = 0;
    GpioCtrlRegs.GPBDIR.bit.GPIO58 = 1; 
    GpioCtrlRegs.GPBMUX2.bit.GPIO59 = 0;
    GpioCtrlRegs.GPBDIR.bit.GPIO59 = 1; 
    GpioCtrlRegs.GPBMUX2.bit.GPIO60 = 0;
    GpioCtrlRegs.GPBDIR.bit.GPIO60 = 1; 
    GpioCtrlRegs.GPBMUX2.bit.GPIO61= 0;
    GpioCtrlRegs.GPBDIR.bit.GPIO61 = 1;
    EDIS;
}

void main(void)
{
    InitSysCtrl();
    DINT;
    InitPieCtrl();
    IER=0x0000;
    IFR=0x0000;
  
    configio();
    SBIT1=1;
    SBIT0=1;
    SBIT2=1;
    SBIT3=1;

    SEG_DATA(0);	   
    while(1)
    {	  
         
      SEG_DATA(table[0]);
      SBIT0=0;DELAY_US(1);
      SBIT2=1;DELAY_US(1);
      SBIT1=1;DELAY_US(1);     
      SBIT3=1;
	  DELAY_US(100);
	  SBIT0=1;
	  SEG_DATA(0);
      
      SEG_DATA(table[1]);
      SBIT1=0;DELAY_US(1);
      SBIT2=1;DELAY_US(1);
      SBIT3=1;DELAY_US(1);
      SBIT0=1;
	  DELAY_US(100);
	  SBIT1=1;
	  SEG_DATA(0);
      
      SEG_DATA(table[2]);
      SBIT2=0;DELAY_US(1);
      SBIT1=1; DELAY_US(1);    
      SBIT3=1;DELAY_US(1);
      SBIT0=1;
	  DELAY_US(100);
	  SBIT2=1;
	  SEG_DATA(0);
     
      SEG_DATA(table[3]);
	  SBIT3=0;DELAY_US(1);
      SBIT1=1;DELAY_US(1);
      SBIT2=1;DELAY_US(1);      
      SBIT0=1; 
	  DELAY_US(100);
	  SBIT3=1;
	  SEG_DATA(0);

	}
}

//
//===========================================================================
// No more.
//===========================================================================
