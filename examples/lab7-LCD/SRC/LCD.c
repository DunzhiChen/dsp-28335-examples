#include"DSP2833x_Device.h"
#include "DSP2833x_Examples.h"
#define	  LCD_DATA_BUS	(*((volatile  Uint16 *)0x4000))
#define	  EN 	GpioDataRegs.GPBDAT.bit.GPIO54
#define	  RW 	GpioDataRegs.GPBDAT.bit.GPIO56
#define	  RS 	GpioDataRegs.GPBDAT.bit.GPIO57
#define	  uchar 	unsigned char
extern uchar menu1[]={"选择电机定时设置"};
extern uchar menu2[]={"开步进电机A"};
extern uchar menu3[]={"开步进电机B"};
extern uchar menu4[]={"开步进电机C"};
void LCDInit(void);
void Write_order(Uint16 order);
void Write_data(Uint16 data);
void delay(Uint16 t);
void configio(void);
void InitXintf(void);
void display(uchar *hz);

void LCD_DATA(uchar d){
    LCD_DATA_BUS = d;
} 
void LCDInit(void)
{
  DELAY_US(5000);
  Write_order(0x30);// 功能设定：基本指令动作
  DELAY_US(50);
  Write_order(0x0c);//显示状态：开
  DELAY_US(50);
  Write_order(0x01);//清屏
  DELAY_US(60);
  Write_order(0x02);//地址归位：AC=0x00
  DELAY_US(40);
}

void Write_order(Uint16 order)
{
   RS=0;delay(1);
   RW=0;delay(1);
   EN=1;delay(1);
   LCD_DATA(order);
   //delay(1);
   EN=0;delay(1);
}

void Write_data(Uint16 data)
{
   RS=1;delay(1);
   RW=0;delay(1);
   EN=1;delay(1);
   LCD_DATA(data);
   //delay(1);
   EN=0;delay(1);
}

void delay(Uint16 t)
{
   Uint16 i;
   while(t--)
    {
      for(i=0;i<500;i++);
    }
}

void configio(void)
{
    EALLOW;
    GpioCtrlRegs.GPBMUX2.bit.GPIO54 = 0;
    GpioCtrlRegs.GPBDIR.bit.GPIO54 = 1; 
    GpioCtrlRegs.GPBMUX2.bit.GPIO56 = 0;
    GpioCtrlRegs.GPBDIR.bit.GPIO56 = 1; 
    GpioCtrlRegs.GPBMUX2.bit.GPIO57= 0;
    GpioCtrlRegs.GPBDIR.bit.GPIO57 = 1; 
    EDIS;
}

void display(uchar *hz)
{
   while(*hz!='\0')
    {
       Write_data(*hz);
       hz++;
       DELAY_US(250);
    }
}

void main(void)
{
    InitSysCtrl();
    InitXintf();
    InitXintf16Gpio();
    DINT;
    InitPieCtrl();
    IER=0x0000;
    IFR=0x0000;
    
    configio();
    RS=0;
    delay(5);
    RW=0;
    delay(5);
    EN=0;
    LCDInit();
    delay(5);
    Write_order(0x01);
    delay(50);
    Write_order(0x80);
    delay(5);
    display(menu1);
   
    Write_order(0x91);
    delay(5);
    display(menu2);
    
    Write_order(0x89);
    delay(5);
    display(menu3);

    Write_order(0x99);
    delay(5);
    display(menu4);

    while(1);
}

//
//===========================================================================
// No more.
//===========================================================================
