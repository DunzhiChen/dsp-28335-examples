#include"DSP2833x_Device.h"
#include"DSP2833x_Examples.h"

#define DCA_ON     GpioDataRegs.GPASET.bit.GPIO20=1  //自定义直流电机的两相GPIO口
#define DCA_OFF    GpioDataRegs.GPACLEAR.bit.GPIO20=1
#define DCB_ON     GpioDataRegs.GPASET.bit.GPIO21=1
#define DCB_OFF    GpioDataRegs.GPACLEAR.bit.GPIO21=1

void ConfigtestGpio(void);         // 配置为普通IO口输出

unsigned char direct_dc=0;//直流电机转动方向标志，1正向，0反向

void main(void)
{
  InitSysCtrl(); //初始化
  DINT;//
  InitPieCtrl();//
  IER=0x0000;//
  IFR=0x0000;//
  InitPieVectTable();//
  EINT;//
  ERTM;//
  ConfigtestGpio();
  DCA_ON;
  DELAY_US(10);
  DCB_OFF;
  DELAY_US(10);
  switch(direct_dc)
  {
    case 1: DCA_ON;DCB_OFF;break;
	case 0: DCA_OFF;DCB_ON;break;
  } 
 
  for(; ;)
  {
   }
}

void ConfigtestGpio()
{
  EALLOW;
  GpioCtrlRegs.GPAMUX2.bit.GPIO20=0;
  GpioCtrlRegs.GPADIR.bit.GPIO20=1;
  
  GpioCtrlRegs.GPAMUX2.bit.GPIO21=0;
  GpioCtrlRegs.GPADIR.bit.GPIO21=1;

  EDIS;
}


//===========================================================================
// No more.
//===========================================================================
