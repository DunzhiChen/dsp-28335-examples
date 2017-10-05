#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File

#define	  LED4	GpioDataRegs.GPADAT.bit.GPIO1  //�Զ���4������LED��GPIO���ݼĴ���
#define	  LED3	GpioDataRegs.GPADAT.bit.GPIO2
#define	  LED1	GpioDataRegs.GPADAT.bit.GPIO3
#define	  LED2	GpioDataRegs.GPADAT.bit.GPIO4 

#define	  BUZZ_C	GpioDataRegs.GPASET.bit.GPIO0=1; //������������
#define	  BUZZ_O	GpioDataRegs.GPACLEAR.bit.GPIO0=1;//����������

interrupt void ISRExint1(void);
void configtestled(void);
void InitExInt(void);

void main(void)
{
   InitSysCtrl();
   DINT;

  InitPieCtrl();
// Disable CPU interrupts and clear all CPU interrupt flags:
   IER = 0x0000;
   IFR = 0x0000;

   InitPieVectTable();
   InitExInt();
	EALLOW;  // This is needed to write to EALLOW protected registers
   	PieVectTable.XINT1 = &ISRExint1;
   	EDIS;    // This is needed to disable write to EALLOW protected registers

   	PieCtrlRegs.PIECTRL.bit.ENPIE = 1;          // Enable the PIE block
	PieCtrlRegs.PIEIER1.bit.INTx4= 1; 
	 
   	IER |= M_INT1;                              // Enable CPU int1

    EINT;   // �ж�ʹ��
    ERTM;   // Enable Global realtime interrupt DBGM
    configtestled();
    LED1=1;       //��ʼ��LED��ƽ
	DELAY_US(10);
	LED2=1;
	DELAY_US(10);
	LED3=1;
	DELAY_US(10);
	LED4=1;
	DELAY_US(10);  
	BUZZ_C 
    while(1);
}
interrupt void ISRExint1(void){
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
	DELAY_US(1000);
	if(GpioDataRegs.GPADAT.bit.GPIO13 == 0){
        LED1=~LED1;
    	LED2=~LED2;
	    LED3=~LED3;
    	LED4=~LED4;
    	BUZZ_O
    	DELAY_US(5000);
    	BUZZ_C
    }	
}

void InitExInt(void){
     EALLOW;
     GpioCtrlRegs.GPAMUX1.bit.GPIO13 = 0;	 
	 GpioCtrlRegs.GPADIR.bit.GPIO13 = 0;
	 GpioCtrlRegs.GPAQSEL1.bit.GPIO13= 0;	 
     GpioIntRegs.GPIOXINT1SEL.bit.GPIOSEL = 13;//ѡ��GPIO13Ϊ�ⲿ����XINT3��������  	 
	 XIntruptRegs.XINT1CR.bit.POLARITY= 0;//�½��ش����ж�	
	 XIntruptRegs.XINT1CR.bit.ENABLE = 1; //ʹ��XINT3�ж�
	 EDIS;
} 

void configtestled(void)
{
   EALLOW;
   GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 0; // GPIO0����ΪGPIO����
   GpioCtrlRegs.GPADIR.bit.GPIO0 = 1;  // GPIO0����Ϊ���
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
