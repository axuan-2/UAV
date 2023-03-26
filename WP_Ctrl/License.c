/* Copyright (c)  2019-2025 Wuhan Nameless Innovation Technology Co.,Ltd. All rights reserved.*/
/*----------------------------------------------------------------------------------------------------------------------/
                开源并不等于免费，先驱者的历史已经证明，在国内这个环境下，毫无收益的开源，单靠坊间个人爱好者，自发地参与项
								目完善的方式行不通，好的开源项目需要请专职人员做好售后服务、手把手教学、统计用户反馈需求、在实践中完成对产
								品的一次次完善与迭代升级。经过综合考虑，无名飞控开源代码中，程序仅保留公司正版激活功能代码，版本激活无实际
								功能，属于公司产品常规出厂操作，不影响客户学习，其余代码全部开放给客户学习，客户移植和二次开发代码请保留代
								码版权。
-----------------------------------------------------------------------------------------------------------------------/
*               本程序只供购买者学习使用，版权著作权属于无名科创团队，无名科创团队将飞控程序源码提供给购买者，
*               购买者要为无名科创团队提供保护，未经作者许可，不得将源代码提供给他人，不得将源代码放到网上供他人免费下载， 
*               更不能以此销售牟利，如发现上述行为，无名科创团队将诉之以法律解决！！！
-----------------------------------------------------------------------------------------------------------------------
*                                                 为什么选择无名创新？
*                                         感动人心价格厚道，最靠谱的开源飞控；
*                                         国内业界良心之作，最精致的售后服务；
*                                         追求极致用户体验，高效进阶学习之路；
*                                         萌新不再孤单求索，合理把握开源尺度；
*                                         响应国家扶贫号召，促进教育体制公平；
*                                         新时代奋斗最出彩，建人类命运共同体。 
-----------------------------------------------------------------------------------------------------------------------
*               生命不息、奋斗不止；前人栽树，后人乘凉！！！
*               开源不易，且学且珍惜，祝早日逆袭、进阶成功！！！
*               学习优秀者，简历可推荐到DJI、ZEROTECH、XAG、AEE、GDU、AUTEL、EWATT、HIGH GREAT等公司就业
*               求职简历请发送：15671678205@163.com，需备注求职意向单位、岗位、待遇等
*               无名科创开源飞控QQ群：2号群465082224、1号群540707961（人员已满）
*               CSDN博客：http://blog.csdn.net/u011992534
*               优酷ID：NamelessCotrun无名小哥
*               B站教学视频：https://space.bilibili.com/67803559/#/video
*               客户使用心得、改进意见征集贴：http://www.openedv.com/forum.php?mod=viewthread&tid=234214&extra=page=1
*               淘宝店铺：https://shop348646912.taobao.com/?spm=2013.1.1000126.2.5ce78a88ht1sO2
*               百度贴吧:无名科创开源飞控
*               公司官网:www.nameless.tech
*               无名创新国内首款TI开源飞控设计初衷、知乎专栏:https://zhuanlan.zhihu.com/p/54471146
*               修改日期:2021/03/20                  
*               版本：躺赢者PRO——CarryPilot_V3.0.4
*               版权所有，盗版必究。
*               Copyright(C) 2019-2025 武汉无名创新科技有限公司 
*               All rights reserved
-----------------------------------------------------------------------------------------------------------------------
*               重要提示：
*               正常淘宝咸鱼转手的飞控、赠送朋友、传给学弟的都可以进售后群学习交流，
*               不得直接在网上销售无名创新资料，无名创新代码有声明版权，他人不得将
*               资料代码传网上供他人下载，不得以谋利为目的销售资料代码，发现有此操
*               作者，公司会提前告知，请1天内及时处理，否则你的学校、单位、姓名、电
*               话、地址信息会被贴出在公司官网、官方微信公众平台、官方技术博客、知乎
*               专栏以及淘宝店铺首页予以公示公告，此种所作所为，会成为个人污点，影响
*               升学、找工作、社会声誉、很快就很在无人机界出名，后果很严重。
*               因此行为给公司造成重大损失者，会以法律途径解决，感谢您的合作，谢谢！！！
----------------------------------------------------------------------------------------------------------------------*/
#include "Headfile.h"
#include "PPM.h"
#include "License.h"
#include "stdlib.h"





void System_Start_Init(void)
{
  SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ |SYSCTL_OSC_MAIN);
  FPUEnable();
  FPULazyStackingEnable();
  initTime();
  EEPROM_Init();
	//全局中断使能
	IntMasterEnable();
	IntPriorityGroupingSet(3);	
//	EEPROMMassErase();
}	



void Resume_Factory_Setting(void)
{
	EEPROMMassErase();
}



void Schedule_init(void)
{
  SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);//定时器1使能				
  TimerConfigure(TIMER1_BASE,TIMER_CFG_PERIODIC_UP);//32位周期定时器
  TimerLoadSet(TIMER1_BASE,TIMER_A,SysCtlClockGet()/1000);//设定装载值,（80M/1000）*1/80M=1ms				
  IntEnable(INT_TIMER1A);//定时器1中断使能				
  TimerIntEnable(TIMER1_BASE,TIMER_TIMA_TIMEOUT); //中断输出, 设置模式;
  TimerIntRegister(TIMER1_BASE,TIMER_A,TIMER1A_Handler);//中断函数注册
//  IntMasterEnable();			
  TimerEnable(TIMER1_BASE,TIMER_A); //定时器使能开始计数	
  //IntPriorityGroupingSet(0);	
  IntPrioritySet(INT_TIMER1A,USER_INT5);
}


void TIMER1A_Handler(void)//中断函数
{
	Temperature_Ctrl();	
  Simulation_PWM_Output(Total_Controller.IMU_Temperature_Control.Control_OutPut);
  TimerIntClear(TIMER1_BASE,TIMER_TIMA_TIMEOUT);  	
}

/***********************************************************
@函数名：ESC_HardWave_Init
@入口参数：无
@出口参数：无
@功能描述：电调校准，随即执行电调模式下相关资源初始化，简单外
芯片资源、必备外设初始化
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void ESC_HardWave_Init()//只初始化校准电调的必要资源
{
  OLED_Init_Fast();//OLED显示屏初始化
  PPM_Init();//PPM解析初始化
  Init_PWM();//PWM初始化
  delay_ms(100);
  WriteFlashParameter(ESC_CALIBRATION_FLAG,0,&Table_Parameter);//写零避免下次上电再次进入
  while(1)
  {
    ESC_Calibration();
    LCD_clear_L(0,0);LCD_clear_L(0,1);LCD_P8x16Str(0,0,"Please Move Thr");
    LCD_clear_L(0,2);LCD_clear_L(0,3);LCD_P8x16Str(0,2,"Down When ESC");
    LCD_clear_L(0,4);LCD_clear_L(0,5);LCD_P8x16Str(0,4,"Beep Beep");
    LCD_P6x8Str(80,4,"Thr:");
    write_6_8_number(80,5,Receiver_PPM_Databuf[2]);
    LCD_clear_L(0,6);LCD_P6x8Str(0,6,"Repower When Set Up");
  }
}



/***********************************************************
@函数名：WP_Init
@入口参数：无
@出口参数：无
功能描述：对系统芯片资源、先执行开机电调校准自检函数，若需要
电调校准，随即执行电调模式下相关资源初始化，否则执行常规硬件
资源初始化。
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void WP_Init(void)
{
  System_Start_Init();//系统启动初始化
	Simulation_PWM_Init();
  ReadFlashParameterOne(ESC_CALIBRATION_FLAG,&ESC_Calibration_Flag);
  if(ESC_Calibration_Flag==1)
  {
    ESC_HardWave_Init();//只初始化校准电调的必要资源 
  }
  else
  {	
		ConfigureUART1();//串口1初始化，无名创新地面站
		ConfigureUART0();//串口0初始化，山外地面站
		ADC_Init();//ADC初始化，检测外部电池电压
		OLED_Init();//OLED显示屏初始化
		Init_I2C();	//硬件I2C初始化
		spl0601_init();
		//Lcd_Init();
		//ST7789_LCD_Clear(LCD_WHITE);
		//BACK_COLOR=LCD_WHITE;
  }
}


