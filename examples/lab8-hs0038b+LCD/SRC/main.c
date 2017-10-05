#include"DSP2833x_Device.h"
#include "DSP2833x_Examples.h"

#define	  DATA 	  GpioDataRegs.GPBDAT.bit.GPIO51 
#define	  LCD_DATA_BUS	(*((volatile  Uint16 *)0x4000))
#define	  EN 	GpioDataRegs.GPBDAT.bit.GPIO54
#define	  RW 	GpioDataRegs.GPBDAT.bit.GPIO56
#define	  RS 	GpioDataRegs.GPBDAT.bit.GPIO57 

void LCD_DATA(uchar d){
    LCD_DATA_BUS = d;
} 

void delay(Uint16 t)
{
   Uint16 i,j;
   while(t--)
    {
      for(i=0;i<500;i++);
    }
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

void LCDInit(void)
{
  delay(500);
  Write_order(0x30);// 功能设定：基本指令动作
  delay(5);
  Write_order(0x0c);//显示状态：开
  delay(5);
  Write_order(0x01);//清屏
  delay(6);
  Write_order(0x02);//地址归位：AC=0x00
  delay(4);
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

    GpioCtrlRegs.GPBMUX2.bit.GPIO51 = 0;
    GpioCtrlRegs.GPBDIR.bit.GPIO51 = 0;  
    EDIS;
}

void display(char *hz)
{
   while(*hz!='\0')
    {
       Write_data(*hz);
       hz++;
       delay(2);
    }
} 

//函数功能：显示十进制数据（最大9999 9999 9）
//输入参数：   v：要显示的十进制数据
//           add: 显示起始地址
//           Len：十进制数据长度（最长9位）
//输出参数：无
void DisDec(Uint32 v, Uint16 add, unsigned char Len){
  Uint32 GUI_Pow10[] = {
  1 , 10, 100, 1000, 10000,
  100000, 1000000, 10000000, 100000000, 1000000000 
  }; 
  char c[10], g;
  unsigned char s=0;
  if (Len > 9) {
    Len = 9;
  }
  if(v == 0) Len = 1;
  while(Len--){
    //d = GUI_Pow10[Len];
    g = (char) (v / GUI_Pow10[Len]);
    v -= g * GUI_Pow10[Len];
    c[s++] = g + '0';     
  }
  c[s] = '\0';
  g=0;
  while(g < s-1){
    if(c[g] == '0') c[g]=' ';
	else break;
	g++;
  } 
    Write_order(add);
    delay(5);
    display(c);
}   

Uint32 Read_SIG(void){
   static Uint16 count=0;
   static Uint32 Last=0;
   Uint32 u=0;
   Uint16 j=0;
   Uint16 i=0;
   Uint16 TimeOut=0;
   while(!DATA){
     DELAY_US(5000);
	 if(DATA) break;
	 TimeOut=2;
	 while((!DATA) && TimeOut++); //等待变高，4.5ms高
	 if(TimeOut == 1) break;
	 DELAY_US(2530);
	 if(!DATA){
	     if(count >= 3)	{count=0;return Last;}
	     else  count++;	
	   } 
	 for(;i<32;i++){
	     TimeOut=2;
	     while(DATA && TimeOut++); //等待变低， 0.56ms低
		 if(TimeOut == 1) return 0;
		 TimeOut=2;
	     while((!DATA) && TimeOut++); //等待变高
		 if(TimeOut == 1) return 0;
	     DELAY_US(840); //延时0.84ms
	     u <<= 1;
	     if(DATA) u++;
	 }
	 i = u>>16;
     j=~(i>>8) & 0x00FF;
	 i &= 0x00FF;
if(i!= j) return 0; 
	 j = u;
     i=~(j>>8) & 0x00FF;
	 j &= 0x00FF;
if(i!= j) return 0; 
   Last = u; 
   count=0;
   return u; 
   }
   return 0;
}

void main(void)
{    
#define Start 0x85
    Uint32 o=0;
//	Uint16 a[10],i=0;
	unsigned char ReceivedChar;
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
   
   
    display("遥控键值: ");

 //   scib_xmit(0xaa);
 //   msg = "\r\nYou will enter a character, and the DSP will echo it back! \n\0";
 //   scib_msg(msg);    
    while(1){
//	DATA=1;
//	DELAY_US(1000);
//	DATA=0;DELAY_US(1000);
      do{
         o = Read_SIG();
	  }while( o == 0);
	 // Write_order(0x80);
     // a[i] = (o >> 8)&0x00FF;
	 // i++;
	 // if(i>=10) i=0;
     // display();
	 ReceivedChar= o & 0x00FF; 
	// scib_xmit(ReceivedChar);
	 ReceivedChar= (o >> 8) & 0x00FF; 
//	 scib_xmit(ReceivedChar);
	 //DisDec(ReceivedChar, 0x85, 3);
	 switch(ReceivedChar){
        case 104: Write_order(Start);delay(5);display(" 0  ");break;
		case 48 : Write_order(Start);delay(5);display(" 1  ");break;
		case 24 : Write_order(Start);delay(5);display(" 2  ");break;
		case 122: Write_order(Start);delay(5);display(" 3  ");break;
		case 16 : Write_order(Start);delay(5);display(" 4  ");break;
		case 56 : Write_order(Start);delay(5);display(" 5  ");break;
		case 90 : Write_order(Start);delay(5);display(" 6  ");break;
		case 66 : Write_order(Start);delay(5);display(" 7  ");break;
		case 74 : Write_order(Start);delay(5);display(" 8  ");break;
		case 82 : Write_order(Start);delay(5);display(" 9  ");break;
		case 152: Write_order(Start);delay(5);display("100+");break;
		case 176: Write_order(Start);delay(5);display("200+");break;
        case 162: Write_order(Start);delay(5);display("CH- ");break;
		case 98 : Write_order(Start);delay(5);display("CH  ");break;
		case 226: Write_order(Start);delay(5);display("CH+ ");break;
		case 34 : Write_order(Start);delay(5);display("左 ");break;
		case 2  : Write_order(Start);delay(5);display("右 ");break;
		case 194: Write_order(Start);delay(5);display("停 ");break;
		case 224: Write_order(Start);delay(5);display(" -  ");break;
		case 168: Write_order(Start);delay(5);display(" +  ");break;
		case 144: Write_order(Start);delay(5);display("EQ  ");break;
        default: break;
	 }
	}
}

//
//===========================================================================
// No more.
//===========================================================================
