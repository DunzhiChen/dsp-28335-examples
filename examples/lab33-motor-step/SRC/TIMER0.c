#include"DSP2833x_Device.h"
#include"DSP2833x_Examples.h"

#define StepA_ON   GpioDataRegs.GPASET.bit.GPIO27=1; //�Զ��岽����������GPIO��
#define StepA_OFF  GpioDataRegs.GPACLEAR.bit.GPIO27=1;
#define StepB_ON   GpioDataRegs.GPASET.bit.GPIO26=1;
#define StepB_OFF  GpioDataRegs.GPACLEAR.bit.GPIO26=1;
#define StepC_ON   GpioDataRegs.GPASET.bit.GPIO25=1;
#define StepC_OFF  GpioDataRegs.GPACLEAR.bit.GPIO25=1;
#define StepD_ON   GpioDataRegs.GPASET.bit.GPIO24=1;
#define StepD_OFF  GpioDataRegs.GPACLEAR.bit.GPIO24=1;

//ת��Ϊ6��λ����������
//���磬���Ҫ����Ϊ����ٶ�, �Ͱ�time�궨��Ϊ20000, ������������time�Ķ���
//ת�ټ��㣺r = 60/(time*4096/1000000)����λ��ת/min
//��ʽ��⣺����ͺ�Ϊ28BYJ-48 5DC,�����5.625��,�������, ��ʱ���ж�һ�β���һ����������
//���ڸõ���Ǽ��ٵ��, ���ٱ�Ϊ1/64, ������64����������ת��һ�������,ÿ����������ת��
//5.625��/64=0.087��  �����Ҫת��һȦ����Ҫ64*64=4096������,��ʱ���Ķ�ʱ���ھ������������
//��ʱ����������time����λ��us
//#define time      20000 //���ת�� 
//#define time      17000
//#define time      14000
//#define time      11000
//#define time      8000     
#define time      5000  //���ת�� 3ת/min

#define PowerDwMotor()   StopCpuTimer0();    //ֹͣ���
#define PowerOnMotor()   StartCpuTimer0();   //�������

void ConfigtestGpio(void);         // ����Ϊ��ͨIO�����
interrupt void ISRTimer0(void);    // ��ʱ��0

unsigned char sa=1; //����������ͨ�����˳��

unsigned char direct_st=1;//������ת�������־direct_st��1����0����


void main(void)
{
  InitSysCtrl(); //��ʼ��
  DINT;//
  InitPieCtrl();//
  IER=0x0000;//
  IFR=0x0000;//
  InitPieVectTable();//

  EALLOW;//
  PieVectTable.TINT0=&ISRTimer0;//�жϺ�����ַ����TINT0
  EDIS;//

  InitCpuTimers();//
  ConfigCpuTimer(&CpuTimer0,150,time); // ��ʱʱ��Ϊtime
  StartCpuTimer0();//
  // ʹ��CPU��Ӧ���ж�����

  // �õ����ж϶�Ӧ�� PIE ��ʹ����Ӧ���ж�����λ
  PieCtrlRegs.PIECTRL.bit.ENPIE=1;
  PieCtrlRegs.PIEIER1.bit.INTx7 = 1;
  IER |= M_INT1;  // ��һ���ж�����
  EINT;//
  ERTM;//

  ConfigtestGpio();
  StepA_ON;  //������IO��ʼ��
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

interrupt void ISRTimer0(void)//��ʱ�жϺ���
{
  PieCtrlRegs.PIEACK.all = PIEACK_GROUP1; //
  CpuTimer0Regs.TCR.bit.TIF=1; //    
  CpuTimer0Regs.TCR.bit.TRB=1;

   switch(sa)
   {
      case 1: StepA_OFF; StepB_ON;  StepC_ON;  StepD_ON; break;//����˳��1
      case 2: StepA_OFF; StepB_OFF; StepC_ON;  StepD_ON; break;//����˳��2
      case 3: StepA_ON;  StepB_OFF; StepC_ON;  StepD_ON; break;//����˳��3
      case 4: StepA_ON;  StepB_OFF; StepC_OFF; StepD_ON; break;//����˳��4
      case 5: StepA_ON;  StepB_ON;  StepC_OFF; StepD_ON; break;//����˳��5
      case 6: StepA_ON;  StepB_ON;  StepC_OFF; StepD_OFF;break;//����˳��6
      case 7: StepA_ON;  StepB_ON;  StepC_ON;  StepD_OFF;break;//����˳��7
      case 8: StepA_OFF; StepB_ON;  StepC_ON;  StepD_OFF;break;//����˳��8
   }

   switch(direct_st) //�жϲ�����ת������
   {
      case 1: if(sa==8) //����
                 sa=1; 
                else
                 sa++;  break;
      case 0: if(sa==1) //����
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
