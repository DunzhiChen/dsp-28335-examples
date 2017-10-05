#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File

#define	  LED4	GpioDataRegs.GPADAT.bit.GPIO1  //自定义4个控制LED的GPIO数据寄存器
#define	  LED3	GpioDataRegs.GPADAT.bit.GPIO2
#define	  LED1	GpioDataRegs.GPADAT.bit.GPIO3
#define	  LED2	GpioDataRegs.GPADAT.bit.GPIO4 

#define	  BUZZ_C	GpioDataRegs.GPASET.bit.GPIO0=1; //蜂鸣器不鸣叫
#define	  BUZZ_O	GpioDataRegs.GPACLEAR.bit.GPIO0=1;//蜂鸣器鸣叫

void configtestled(void); //GPIO配置

void main(void)
{
   InitSysCtrl();
   DINT;
   InitPieCtrl();
// Disable CPU interrupts and clear all CPU interrupt flags:
   IER = 0x0000;
   IFR = 0x0000;
   InitPieVectTable();  
    configtestled();

    LED1=1;       //初始化LED电平
	DELAY_US(10);
	LED2=1;
	DELAY_US(10);
	LED3=1;
	DELAY_US(10);
	LED4=1;
	DELAY_US(10); 
	BUZZ_C
    while(1){
      if(GpioDataRegs.GPADAT.bit.GPIO13 == 0){ //如果检测到GPIO为低电平,即按键按下
	   DELAY_US(10000);   //延时消抖，防止电平抖动导致的误触发
	   if(GpioDataRegs.GPADAT.bit.GPIO13 == 0){ //如果还是低电平说明按键确实按下了
          LED1=~LED1;  //四个LED灯电平取反，实现LED闪烁效果
    	  LED2=~LED2;
	      LED3=~LED3;
    	  LED4=~LED4;
    	  BUZZ_O      //蜂鸣器鸣叫
    	  DELAY_US(50000);
    	  BUZZ_C
		  while(GpioDataRegs.GPADAT.bit.GPIO13 == 0);//等待按键松开
		}
      } 
	}
}

void configtestled(void)
{
   EALLOW;
   GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 0; // GPIO0复用为GPIO功能
   GpioCtrlRegs.GPADIR.bit.GPIO0 = 1;  // GPIO0设置为输出
   GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 0; // GPIO1 = GPIO1
   GpioCtrlRegs.GPADIR.bit.GPIO1 = 1;
   GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 0; // 
   GpioCtrlRegs.GPADIR.bit.GPIO2 = 1;
   GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 0; // 
   GpioCtrlRegs.GPADIR.bit.GPIO3 = 1;
   GpioCtrlRegs.GPAMUX1.bit.GPIO4 = 0; // 
   GpioCtrlRegs.GPADIR.bit.GPIO4 = 1; 
   EDIS; 
}

//===========================================================================
// No more.
//===========================================================================
