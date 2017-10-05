#include"DSP2833x_Device.h"
#include"DSP2833x_Examples.h"

#define StepA_ON   GpioDataRegs.GPASET.bit.GPIO27=1; //自定义步进机的四相GPIO口
#define StepA_OFF  GpioDataRegs.GPACLEAR.bit.GPIO27=1;
#define StepB_ON   GpioDataRegs.GPASET.bit.GPIO26=1;
#define StepB_OFF  GpioDataRegs.GPACLEAR.bit.GPIO26=1;
#define StepC_ON   GpioDataRegs.GPASET.bit.GPIO25=1;
#define StepC_OFF  GpioDataRegs.GPACLEAR.bit.GPIO25=1;
#define StepD_ON   GpioDataRegs.GPASET.bit.GPIO24=1;
#define StepD_OFF  GpioDataRegs.GPACLEAR.bit.GPIO24=1;

//转速为6档位，可以设置
//比如，如果要设置为最低速度, 就把time宏定义为20000, 而屏蔽其他对time的定义
//转速计算：r = 60/(time*4096/1000000)，单位：转/min
//公式详解：电机型号为28BYJ-48 5DC,步距角5.625°,四相八拍, 定时器中断一次产生一个换相脉冲
//由于该电机是减速电机, 减速比为1/64, 即产生64个脉冲电机轴转动一个步距角,每个脉冲电机轴转动
//5.625°/64=0.087°  电机轴要转动一圈，需要64*64=4096个脉冲,定时器的定时周期就是脉冲的周期
//定时器的周期是time，单位是us
//#define time      20000 //最低转速 
//#define time      17000
//#define time      14000
//#define time      11000
//#define time      8000     
#define time      5000  //最高转速 3转/min

#define PowerDwMotor()   StopCpuTimer0();    //停止电机
#define PowerOnMotor()   StartCpuTimer0();   //启动电机

void ConfigtestGpio(void);         // 配置为普通IO口输出
interrupt void ISRTimer0(void);    // 定时器0

unsigned char sa=1; //步进机四相通电分配顺序

unsigned char direct_st=1;//步进机转动方向标志direct_st，1正向，0反向


void main(void)
{
  InitSysCtrl(); //初始化
  DINT;//
  InitPieCtrl();//
  IER=0x0000;//
  IFR=0x0000;//
  InitPieVectTable();//

  EALLOW;//
  PieVectTable.TINT0=&ISRTimer0;//中断函数地址赋给TINT0
  EDIS;//

  InitCpuTimers();//
  ConfigCpuTimer(&CpuTimer0,150,time); // 定时时间为time
  StartCpuTimer0();//
  // 使能CPU相应的中断允许

  // 用到的中断对应的 PIE 中使能相应的中断允许位
  PieCtrlRegs.PIECTRL.bit.ENPIE=1;
  PieCtrlRegs.PIEIER1.bit.INTx7 = 1;
  IER |= M_INT1;  // 第一组中断允许
  EINT;//
  ERTM;//

  ConfigtestGpio();
  StepA_ON;  //步进机IO初始化
  DELAY_US(10);
  StepB_ON;
  DELAY_US(10);
  StepC_ON;
  DELAY_US(10);
  StepD_ON;
  DELAY_US(10);
  PowerOnMotor();
  for(; ;)
  {
   }
}

interrupt void ISRTimer0(void)//定时中断函数
{
  PieCtrlRegs.PIEACK.all = PIEACK_GROUP1; //
  CpuTimer0Regs.TCR.bit.TIF=1; //    
  CpuTimer0Regs.TCR.bit.TRB=1;

   switch(sa)
   {
      case 1: StepA_OFF; StepB_ON;  StepC_ON;  StepD_ON; break;//分配顺序1
      case 2: StepA_OFF; StepB_OFF; StepC_ON;  StepD_ON; break;//分配顺序2
      case 3: StepA_ON;  StepB_OFF; StepC_ON;  StepD_ON; break;//分配顺序3
      case 4: StepA_ON;  StepB_OFF; StepC_OFF; StepD_ON; break;//分配顺序4
      case 5: StepA_ON;  StepB_ON;  StepC_OFF; StepD_ON; break;//分配顺序5
      case 6: StepA_ON;  StepB_ON;  StepC_OFF; StepD_OFF;break;//分配顺序6
      case 7: StepA_ON;  StepB_ON;  StepC_ON;  StepD_OFF;break;//分配顺序7
      case 8: StepA_OFF; StepB_ON;  StepC_ON;  StepD_OFF;break;//分配顺序8
   }

   switch(direct_st) //判断步进机转动方向
   {
      case 1: if(sa==8) //正向
                 sa=1; 
                else
                 sa++;  break;
      case 0: if(sa==1) //反向
                 sa=8; 
                else
                 sa--;  break;
   }
  
}

void ConfigtestGpio()
{
  EALLOW;

  GpioCtrlRegs.GPAMUX2.bit.GPIO26=0;
  GpioCtrlRegs.GPADIR.bit.GPIO26=1;

  GpioCtrlRegs.GPAMUX2.bit.GPIO24=0;
  GpioCtrlRegs.GPADIR.bit.GPIO24=1;

  GpioCtrlRegs.GPAMUX2.bit.GPIO25=0;
  GpioCtrlRegs.GPADIR.bit.GPIO25=1;

  GpioCtrlRegs.GPAMUX2.bit.GPIO27=0;
  GpioCtrlRegs.GPADIR.bit.GPIO27=1;
  EDIS;
}


//===========================================================================
// No more.
//===========================================================================
