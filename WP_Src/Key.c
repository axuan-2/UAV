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
#include "Key.h"

rc_buttton _button;


/***********************************************************
@函数名：Key_Init
@入口参数：无
@出口参数：无
功能描述：按键初始化
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void Key_Init(void)
{
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
  //SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
  // Unlock PF0 so we can change it to a GPIO input
  // Once we have enabled (unlocked) the commit register then re-lock it
  // to prevent further changes.  PF0 is muxed with NMI thus a special case.
  HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
  HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;
  HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;
  GPIODirModeSet(GPIO_PORTF_BASE, GPIO_PIN_0,GPIO_DIR_MODE_IN);//SW1
  GPIOPadConfigSet(GPIO_PORTF_BASE,GPIO_PIN_0,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
  GPIODirModeSet(GPIO_PORTF_BASE, GPIO_PIN_4,GPIO_DIR_MODE_IN);//SW2
  GPIOPadConfigSet(GPIO_PORTF_BASE,GPIO_PIN_4,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
	
	
	_button.state[UP].port=GPIO_PORTF_BASE;
	_button.state[UP].pin=GPIO_PIN_0;
	_button.state[UP].value=1;
	_button.state[UP].last_value=1;

	_button.state[DOWN].port=GPIO_PORTF_BASE;
	_button.state[DOWN].pin=GPIO_PIN_4;
	_button.state[DOWN].value=1;
	_button.state[DOWN].last_value=1;	
	
	ADC_Button_Init();
}


void Read_Button_State_One(button_state *button)
{
  button->value=GPIOPinRead(button->port,button->pin);
	if(button->value==0)
	{
		if(button->last_value!=0)//首次按下
		{
			button->press_time=millis();//记录按下的时间点
			button->in_time=millis();//记录按下的时间点
			button->in_press_cnt=0;
		}
		else
		{
			if(millis()-button->in_time>IN_PRESS_LIMIT)//持续按下
			{
				button->in_time=millis();
				button->press=IN_PRESS;
				//rc_beep.time=1;
				//Bling_Set(&Light_3,200,50,0.1,0,GPIO_PORT_PJ,GPIO_PIN5,0);//红色
				if(button->press==IN_PRESS)  button->in_press_cnt++;
			}
		}
	}
  else
	{
		if(button->last_value==0)//按下后释放
		{
			button->release_time=millis();//记录释放时的时间
			
			if(button->release_time-button->press_time>LONG_PRESS_LIMIT)
			{
			   button->press=LONG_PRESS;
				 button->state_lock_time=0;//5ms*300=1.5S
				 //rc_beep.time=50;
				//Bling_Set(&Light_3,500,100,0.5,0,GPIO_PORT_PJ,GPIO_PIN5,0);//红色
			}
			else
			{
			   button->press=SHORT_PRESS;
				 button->state_lock_time=0;//5ms*300=1.5S
				 //rc_beep.time=10;
				//Bling_Set(&Light_3,200,100,0.5,0,GPIO_PORT_PJ,GPIO_PIN5,0);//红色
			}
		}
	}
	button->last_value=button->value;
	
	
	if(button->press==LONG_PRESS
	 ||button->press==SHORT_PRESS)//按下释放后，程序后台1.5S内无响应，复位按键状态
	{
	  button->state_lock_time++;
		if(button->state_lock_time>=300)
		{			
			 button->press=NO_PRESS;
			 button->state_lock_time=0;
		}
	}
}


void Read_Button_State_All(void)
{
  for(uint16_t i=0;i<BUTTON_NUM;i++)
	{
	  Read_Button_State_One(&_button.state[i]);
	}
}





void key_delay(int32_t period)
{
  while(period--)
  {
    delay_us(1);  
  }
}

#define LONG_PRESS_MAX  5000
int16_t Page_Number=0;
uint8_t Key_Right_Release=0;
uint8_t Oled_Show_Enable=1;
/***********************************************************
@函数名：Key_Scan
@入口参数：release
@出口参数：bool
功能描述：按键扫描，入口参数release决定是否开放按键扫描权限
正常扫描返回TRUE，按键按下时为低电平，释放后IO配置的是上拉
输入模式，悬空为高电平
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
bool Key_Scan(uint8_t release)
{
	uint16_t long_press_cnt=0;
  if(release==1)  return false;
	if(_button.state[UP].press==SHORT_PRESS)
	{
		Page_Number--;
		if(Page_Number<0) Page_Number=19;
		_button.state[UP].press=NO_PRESS;
		LCD_CLS();
	}

	if(_button.state[DOWN].press==SHORT_PRESS)
	{
		Page_Number++;
		if(Page_Number>19) Page_Number=0;
		_button.state[DOWN].press=NO_PRESS;
		LCD_CLS();
	}
	

		if(Controler_State==Lock_Controler
		  &&Throttle_Control<=(1000+Buttom_Safe_Deadband)
		  &&Yaw_Control>=Yaw_Max*Scale_Pecent_Max
			&&Roll_Control<-Pit_Rol_Max*Scale_Pecent_Max&&Pitch_Control==0)
		{
			while(Controler_State==Lock_Controler
					&&Throttle_Control<=(1000+Buttom_Safe_Deadband)
					&&Yaw_Control>=Yaw_Max*Scale_Pecent_Max
					&&Roll_Control<-Pit_Rol_Max*Scale_Pecent_Max&&Pitch_Control==0)
			{
				long_press_cnt++;
				key_delay(10);
				if(long_press_cnt>=LONG_PRESS_MAX)
				{
					Page_Number++;
					if(Page_Number>19) Page_Number=0;
					LCD_CLS();
					return true;
				}
			}
		}

		
		if(Controler_State==Lock_Controler
		  &&Throttle_Control<=(1000+Buttom_Safe_Deadband)
			&&Yaw_Control>=Yaw_Max*Scale_Pecent_Max
		  &&Roll_Control>Pit_Rol_Max*Scale_Pecent_Max&&Pitch_Control==0)
		{
			while(Controler_State==Lock_Controler
					&&Throttle_Control<=(1000+Buttom_Safe_Deadband)
					&&Yaw_Control>=Yaw_Max*Scale_Pecent_Max
					&&Roll_Control>Pit_Rol_Max*Scale_Pecent_Max&&Pitch_Control==0) 
			{
				long_press_cnt++;
				key_delay(10);
				if(long_press_cnt>=LONG_PRESS_MAX)
				{
					Page_Number--;
					if(Page_Number<0) Page_Number=19;
					LCD_CLS();
					return true;
				}
			}
		}	
  return true;
}



/***********************************************************
@函数名：QuadShow
@入口参数：无
@出口参数：无
功能描述：显示屏刷新，每一页每一行数据结合使用说明书来观看，
本函数翻页需结合按键扫描来实现
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void QuadShow(uint8_t show_enable)
{
  uint16_t i=0;
	if(show_enable==1)
	{
		if(Page_Number==0)
		{
			LCD_clear_L(0,0);  display_6_8_string(0,0,"Basic");           display_6_8_number(40,0,Time0_Delta.Time_Delta);  								  display_6_8_number(75,0,Battery_Voltage);display_6_8_number(115,0,Page_Number+1);
			LCD_clear_L(0,1);  display_6_8_string(0,1,"Yaw:");            display_6_8_number(40,1,Yaw);                                       display_6_8_number(90,1,Yaw_Gyro);
			LCD_clear_L(0,2);  display_6_8_string(0,2,"Pitch:");          display_6_8_number(40,2,Pitch);                                     display_6_8_number(90,2,Pitch_Gyro);
			LCD_clear_L(0,3);  display_6_8_string(0,3,"Roll:");           display_6_8_number(40,3,Roll);                            					display_6_8_number(90,3,Roll_Gyro);
			LCD_clear_L(0,4);  display_6_8_string(0,4,"Baro:");           display_6_8_number(40,4,(uint32_t)(WP_Sensor.baro_pressure_raw));   display_6_8_number(90,4,WP_Sensor.baro_altitude);
			LCD_clear_L(0,5);  display_6_8_string(0,5,"US:");             display_6_8_number(40,5,GD_Distance);               			          display_6_8_number(80,5,NamelessQuad.Position[_YAW]);
			LCD_clear_L(0,6);  display_6_8_string(0,6,"TF:");             display_6_8_number(26,6,tfdata.distance);
																																		display_6_8_number(45,6,Button_ADCResult);
																																		display_6_8_number(90,6,NamelessQuad.Speed[_YAW]);
			LCD_clear_L(0,7);  display_6_8_number(0,7,SDK1_Mode_Setup);
												 display_6_8_number(15,7,Gyro_Safety_Calibration_Flag);  
												 display_6_8_number(30,7,WP_Sensor.temperature);         
												 display_6_8_number(75,7,Origion_NamelessQuad.Acceleration[_YAW]);
			
		}
		else if(Page_Number==1)
		{
			LCD_clear_L(0,0);  LCD_P6x8Str(0,0,"Accel:");write_6_8_number(115,0,Page_Number+1);
			if(Accel_Simple_Mode==0)  LCD_P6x8Str(50,0,"6-Side");
			else LCD_P6x8Str(50,0,"Simple");
			
			if(Pitch_Control>=Scale_Pecent_Max*Pit_Rol_Max)
			{
				delay_ms(10);
				if(Pitch_Control>=Scale_Pecent_Max*Pit_Rol_Max)
				{
					while(Pitch_Control>=Scale_Pecent_Max*Pit_Rol_Max);
					if(Accel_Simple_Mode==0) Accel_Simple_Mode=1;
					else if(Accel_Simple_Mode==1) Accel_Simple_Mode=0;
					WriteFlashParameter(ACCEL_SIMPLE_MODE,Accel_Simple_Mode,&Table_Parameter);
					Bling_Set(&Light_2,500,50,0.2,0,GPIO_PORTF_BASE,GPIO_PIN_2,0);//蓝色
				}
			}
			
			if(_button.state[DOWN].press==LONG_PRESS)
			{
					_button.state[DOWN].press=NO_PRESS;
					if(Accel_Simple_Mode==0) Accel_Simple_Mode=1;
					else if(Accel_Simple_Mode==1) Accel_Simple_Mode=0;
					WriteFlashParameter(ACCEL_SIMPLE_MODE,Accel_Simple_Mode,&Table_Parameter);
					Bling_Set(&Light_2,500,50,0.2,0,GPIO_PORTF_BASE,GPIO_PIN_2,0);//蓝色				
			}		
			
			//长按上一页校准机架水平
			if(_button.state[UP].press==LONG_PRESS)
			{
					_button.state[UP].press=NO_PRESS;
					cal_flag=0x04;
					Bling_Set(&Light_2,500,50,0.2,0,GPIO_PORTF_BASE,GPIO_PIN_2,0);//蓝色				
			}	

			
			
			LCD_clear_L(0,1);  LCD_P6x8Str(0,1,"X:");              write_6_8_number(15,1,K[0]);   write_6_8_number(55,1,B[0]);   write_6_8_number(95,1,Accel_Hor_Read.x);
			LCD_clear_L(0,2);  LCD_P6x8Str(0,2,"Y:");              write_6_8_number(15,2,K[1]);   write_6_8_number(55,2,B[1]);   write_6_8_number(95,2,Accel_Hor_Read.y);
			LCD_clear_L(0,3);  LCD_P6x8Str(0,3,"Z:");              write_6_8_number(15,3,K[2]);   write_6_8_number(55,3,B[2]);   write_6_8_number(95,3,Accel_Hor_Read.z);
			LCD_clear_L(0,4);  LCD_P6x8Str(0,4,"Mag /T:");    		 write_6_8_number(40,4,WP_Sensor.mag_intensity);
																														 LCD_P6x8Str(80,4,"P/R_Off:");
			LCD_clear_L(0,5);  LCD_P6x8Str(0,5,"MX_OFF:");         write_6_8_number(40,5,Mag_Offset_Read.x);  write_6_8_number(80,5,Pitch_Offset);
			LCD_clear_L(0,6);  LCD_P6x8Str(0,6,"MY_OFF:");         write_6_8_number(40,6,Mag_Offset_Read.y);  write_6_8_number(80,6,Roll_Offset);
			LCD_clear_L(0,7);  LCD_P6x8Str(0,7,"MZ_OFF:");         write_6_8_number(40,7,Mag_Offset_Read.z);
		}
		else  if(Page_Number==2)
		{
			LCD_clear_L(0,0);  display_6_8_string(0,0,"RC Data");         display_6_8_number(115,0,Page_Number+1);
			LCD_clear_L(0,1);  display_6_8_string(0,1,"Yaw:");            display_6_8_number(40,1,Yaw_Control);                     display_6_8_number(90,1,PPM_Databuf[3]);
			LCD_clear_L(0,2);  display_6_8_string(0,2,"Pitch:");          display_6_8_number(40,2,Pitch_Control);                   display_6_8_number(90,2,PPM_Databuf[1]);
			LCD_clear_L(0,3);  display_6_8_string(0,3,"Roll:");           display_6_8_number(40,3,Roll_Control);                    display_6_8_number(90,3,PPM_Databuf[0]);
			LCD_clear_L(0,4);  display_6_8_string(0,4,"Thr:");            display_6_8_number(40,4,Throttle_Control);                display_6_8_number(90,4,PPM_Databuf[2]);
			LCD_clear_L(0,5);  display_6_8_string(0,5,"ch5-6:");          display_6_8_number(40,5,PPM_Databuf[4]);                  display_6_8_number(90,5,PPM_Databuf[5]);
			LCD_clear_L(0,6);  display_6_8_string(0,6,"ch7-8:");          display_6_8_number(40,6,PPM_Databuf[6]);                  display_6_8_number(90,6,PPM_Databuf[7]);
			
			LCD_clear_L(0,7);  display_6_8_string(0,7,"mode");             
			if(RC_Data.thr_mode==HAND_THR)        display_6_8_string(30,7,"Manual_Ctrl_Alt");
			else if(RC_Data.thr_mode==AUTO_THR&&RC_Data.flight_mode==STABILIZE)   display_6_8_string(30,7,"Auto_Ctrl_Alt");
			else if(RC_Data.thr_mode==AUTO_THR&&RC_Data.flight_mode==POS_GUIDE)   display_6_8_string(30,7,"Auto_Pos_Guide");
			else if(RC_Data.thr_mode==AUTO_THR&&RC_Data.flight_mode==POS_HOLD)    display_6_8_string(30,7,"Auto_Ctrl_Hover");
			else display_6_8_string(30,7,"Manual_Ctrl_Alt"); 
		}
		else if(Page_Number==3)
		{   
			LCD_clear_L(0,0);  display_6_8_string(0,0,"Ctrl Data P  I");  display_6_8_number(115,0,Page_Number+1);
			LCD_clear_L(0,1);  display_6_8_string(0,1,"Pit_Ang:");        display_6_8_number(50,1,Total_Controller.Pitch_Angle_Control.Kp);           display_6_8_number(90,1,Total_Controller.Pitch_Angle_Control.Ki); 
			LCD_clear_L(0,2);  display_6_8_string(0,2,"Rol_Ang:");        display_6_8_number(50,2,Total_Controller.Roll_Angle_Control.Kp);            display_6_8_number(90,2,Total_Controller.Roll_Angle_Control.Ki);    
			LCD_clear_L(0,3);  display_6_8_string(0,3,"Yaw_Ang:");        display_6_8_number(50,3,Total_Controller.Yaw_Angle_Control.Kp);             display_6_8_number(90,3,Total_Controller.Yaw_Angle_Control.Ki);    
			LCD_clear_L(0,4);  display_6_8_string(0,4,"Pit_Gyr:");        display_6_8_number(50,4,Total_Controller.Pitch_Gyro_Control.Kp);            display_6_8_number(90,4,Total_Controller.Pitch_Gyro_Control.Ki);    
			LCD_clear_L(0,5);  display_6_8_string(0,5,"Rol_Gyr:");        display_6_8_number(50,5,Total_Controller.Roll_Gyro_Control.Kp);             display_6_8_number(90,5,Total_Controller.Roll_Gyro_Control.Ki);    
			LCD_clear_L(0,6);  display_6_8_string(0,6,"Yaw_Gyr:");        display_6_8_number(50,6,Total_Controller.Yaw_Gyro_Control.Kp);              display_6_8_number(90,6,Total_Controller.Yaw_Gyro_Control.Ki);
			LCD_clear_L(0,7);  display_6_8_string(0,7,"Gyro_D:");         display_6_8_number(40,7,Total_Controller.Pitch_Gyro_Control.Kd);            display_6_8_number(70,7,Total_Controller.Roll_Gyro_Control.Kd);                  write_6_8_number(95,7,Total_Controller.Yaw_Gyro_Control.Kd);
		}
		else if(Page_Number==4)
		{
			LCD_clear_L(0,0);  LCD_P6x8Str(0,0,"Top_View_Openmv"); write_6_8_number(115,0,Page_Number+1);
			LCD_clear_L(0,1);  LCD_P6x8Str(0,1,"UART3_SDK_Mode:"); write_6_8_number(100,1,SDK1_Mode_Setup);
			
			LCD_clear_L(0,2); 			
			if(SDK1_Mode_Setup==0x00)     		 LCD_P6x8Str(0,2,"NCQ_SDK_Run");//默认模式
			else if(SDK1_Mode_Setup==0x01)     LCD_P6x8Str(0,2,"Color_Block");//色块追踪模式
			else if(SDK1_Mode_Setup==0x02)     LCD_P6x8Str(0,2,"Top_APrilTag");//AprilTag追踪模式
			else if(SDK1_Mode_Setup==0x03)     LCD_P6x8Str(0,2,"Track_Control");
			else if(SDK1_Mode_Setup==0x04)     LCD_P6x8Str(0,2,"Front_AprilTag");
			else if(SDK1_Mode_Setup==0x05)     LCD_P6x8Str(0,2,"Front_Surround_Pole");
			else 
			{
				LCD_P6x8Str(0,2,"User SDK:");
				write_6_8_number(70,2,SDK1_Mode_Setup-3);
			}
			
			

			LCD_clear_L(0,6);LCD_P6x8Str(0,6,"Now_SDK1_Mode:");write_6_8_number(100,6,Opv_Top_View_Target.sdk_mode-0xA0);		

			
			if(Pitch_Control>=Scale_Pecent_Max*Pit_Rol_Max)
			{
				delay_ms(10);
				if(Pitch_Control>=Scale_Pecent_Max*Pit_Rol_Max)
				{
					while(Pitch_Control>=Scale_Pecent_Max*Pit_Rol_Max);
					SDK1_Mode_Setup++;
					if(SDK1_Mode_Setup>16) SDK1_Mode_Setup=0;
					SDK_DT_Send_Check(SDK1_Mode_Setup,UART3_SDK);
					WriteFlashParameter(SDK1_MODE_DEFAULT,SDK1_Mode_Setup,&Table_Parameter);
					Bling_Set(&Light_2,500,50,0.2,0,GPIO_PORTF_BASE,GPIO_PIN_2,0);//蓝色
				}
			}
			
			if(_button.state[DOWN].press==LONG_PRESS)
			{
					_button.state[DOWN].press=NO_PRESS;
					SDK1_Mode_Setup++;
					if(SDK1_Mode_Setup>16) SDK1_Mode_Setup=0;
					SDK_DT_Send_Check(SDK1_Mode_Setup,UART3_SDK);
					WriteFlashParameter(SDK1_MODE_DEFAULT,SDK1_Mode_Setup,&Table_Parameter);	
					Bling_Set(&Light_2,500,50,0.2,0,GPIO_PORTF_BASE,GPIO_PIN_2,0);//蓝色				
			}	           
		}
		else if(Page_Number==5)
		{
			LCD_clear_L(0,0);  LCD_P6x8Str(0,0,"Front_View_Openmv"); write_6_8_number(115,0,Page_Number+1);
			LCD_clear_L(0,1);  LCD_P6x8Str(0,1,"UART1_SDK_Mode:");write_6_8_number(100,1,SDK2_Mode_Setup);
			
			LCD_clear_L(0,6);LCD_P6x8Str(0,6,"Now_SDK2_Mode:");write_6_8_number(100,6,Opv_Front_View_Target.sdk_mode-0xA0);		

			
			if(Pitch_Control>=Scale_Pecent_Max*Pit_Rol_Max)
			{
				delay_ms(10);
				if(Pitch_Control>=Scale_Pecent_Max*Pit_Rol_Max)
				{
					while(Pitch_Control>=Scale_Pecent_Max*Pit_Rol_Max);
					SDK2_Mode_Setup++;
					if(SDK2_Mode_Setup>16) SDK2_Mode_Setup=0;
					SDK_DT_Send_Check(SDK2_Mode_Setup,UART0_SDK);
					WriteFlashParameter(SDK2_MODE_DEFAULT,SDK2_Mode_Setup,&Table_Parameter);
					Bling_Set(&Light_2,500,50,0.2,0,GPIO_PORTF_BASE,GPIO_PIN_2,0);//蓝色
				}
			}
			
			if(_button.state[DOWN].press==LONG_PRESS)
			{
					_button.state[DOWN].press=NO_PRESS;
					SDK2_Mode_Setup++;
					if(SDK2_Mode_Setup>16) SDK2_Mode_Setup=0;
					SDK_DT_Send_Check(SDK2_Mode_Setup,UART0_SDK);
					WriteFlashParameter(SDK2_MODE_DEFAULT,SDK2_Mode_Setup,&Table_Parameter);
					Bling_Set(&Light_2,500,50,0.2,0,GPIO_PORTF_BASE,GPIO_PIN_2,0);//蓝色				
			}					
		}
		else if(Page_Number==6)
		{
			ssd1306_clear_display();
			ssd1306_draw_line(0,32,128,32,WHITE);
			ssd1306_draw_line(64,0,64,64,WHITE);			
			uint16_t x,y;
			y=32-32*constrain_float(0.5*Opv_Top_View_Target.height-Opv_Top_View_Target.y,-50,50)/50.0f;
			x=64-32*constrain_float(0.5*Opv_Top_View_Target.width-Opv_Top_View_Target.x,-50,50)/50.0f;
      if(Opv_Top_View_Target.flag==1)			ssd1306_fill_circle(x,y,5,WHITE);
			else ssd1306_draw_circle(x,y,5,WHITE);			
			
			ssd1306_display();
			display_6_8_string(0,0,"point_x:");  write_6_8_number(95,0,Opv_Top_View_Target.x);		write_6_8_number(115,0,Page_Number+1);
			display_6_8_string(0,1,"point_y::"); write_6_8_number(95,1,Opv_Top_View_Target.y);
			display_6_8_string(0,2,"point_s:");  write_6_8_number(95,2,Opv_Top_View_Target.pixel);
			display_6_8_string(0,3,"point_f:");  write_6_8_number(95,3,Opv_Top_View_Target.flag);
			display_6_8_string(0,4,"Tar_x:");    write_6_8_number(95,4,Opv_Top_View_Target.sdk_target.x);
			display_6_8_string(0,5,"Tar_y:");    write_6_8_number(95,5,Opv_Top_View_Target.sdk_target.y); 
			display_6_8_string(0,6,"FPS:");      write_6_8_number(95,6,Opv_Top_View_Target.fps);
			display_6_8_string(0,7,"Dis:");      write_6_8_number(30,7,Opv_Top_View_Target.sdk_target.x);
																					 write_6_8_number(95,7,Opv_Top_View_Target.sdk_target.y);
		}
		else if(Page_Number==7)
		{
			
			
			ssd1306_clear_display();
			ssd1306_draw_line(0,32,128,32,WHITE);
			ssd1306_draw_line(64,0,64,64,WHITE);
			
			uint16_t x,y;
			y=32-32*constrain_float(0.5*Opv_Front_View_Target.height-Opv_Front_View_Target.y,-50,50)/50.0f;
			x=64-32*constrain_float(0.5*Opv_Front_View_Target.width-Opv_Front_View_Target.x,-50,50)/50.0f;
      if(Opv_Front_View_Target.flag==1)			ssd1306_fill_circle(x,y,5,WHITE);
			else ssd1306_draw_circle(x,y,5,WHITE);			
			
			ssd1306_display();
			display_6_8_string(0,0,"point_x:");  write_6_8_number(95,0,Opv_Front_View_Target.x);	write_6_8_number(115,0,Page_Number+1);
			display_6_8_string(0,1,"point_y::"); write_6_8_number(95,1,Opv_Front_View_Target.y);
			display_6_8_string(0,2,"point_s:");  write_6_8_number(95,2,Opv_Front_View_Target.pixel);
			display_6_8_string(0,3,"point_f:");  write_6_8_number(95,3,Opv_Front_View_Target.flag);
			display_6_8_string(0,4,"Tar_x:");    write_6_8_number(95,4,Opv_Front_View_Target.sdk_target.x);
			display_6_8_string(0,5,"Tar_y:");    write_6_8_number(95,5,Opv_Front_View_Target.sdk_target.y); 
			display_6_8_string(0,6,"FPS:");      write_6_8_number(95,6,Opv_Front_View_Target.fps);
			display_6_8_string(0,7,"Dis:");      write_6_8_number(25,7,Opv_Front_View_Target.apriltag_distance);
																					 write_6_8_number(75,7,front_tofsense_distance);				
																					 write_6_8_number(120,7,front_tofsense_distance_valid_cnt);			
		}
		else if(Page_Number==8)
		{
			LCD_clear_L(0,0);  					 display_6_8_string(0,0,"Opt_Type:");  		
      if(Optical_Type_Present==1)  display_6_8_string(55,0,"LC306");
      else if(Optical_Type_Present==2)  display_6_8_string(55,0,"LC302");
      else if(Optical_Type_Present==3)  display_6_8_string(55,0,"GL9306");
      else if(Optical_Type_Present==4)  display_6_8_string(55,0,"PMW3901"); 


			if(Pitch_Control>=Scale_Pecent_Max*Pit_Rol_Max)
			{
				delay_ms(10);
				if(Pitch_Control>=Scale_Pecent_Max*Pit_Rol_Max)
				{
					while(Pitch_Control>=Scale_Pecent_Max*Pit_Rol_Max);
					Optical_Type_Present++;
					if(Optical_Type_Present>4) Optical_Type_Present=1;
					WriteFlashParameter(OPTICAL_TYPE,
															Optical_Type_Present,
															&Table_Parameter);
				}
			}

			if(_button.state[DOWN].press==LONG_PRESS)
			{
					_button.state[DOWN].press=NO_PRESS;
					Optical_Type_Present++;
					if(Optical_Type_Present>4) Optical_Type_Present=1;
					WriteFlashParameter(OPTICAL_TYPE,
															Optical_Type_Present,
															&Table_Parameter);
					Bling_Set(&Light_2,500,50,0.2,0,GPIO_PORTF_BASE,GPIO_PIN_2,0);//蓝色				
			}	   
					
			display_6_8_number(115,0,Page_Number+1);
			LCD_clear_L(0,1);  display_6_8_string(0,1,"flow_x:");        display_6_8_number(50,1,opt_origin_data.pixel_flow_x_integral);             display_6_8_number(90,1,opt_gyro_data.x); 
			LCD_clear_L(0,2);  display_6_8_string(0,2,"flow_y:");        display_6_8_number(50,2,opt_origin_data.pixel_flow_y_integral);             display_6_8_number(90,2,opt_gyro_data.y);    
			LCD_clear_L(0,3);  display_6_8_string(0,3,"time:");       		display_6_8_number(50,3,opt_origin_data.integration_timespan);              display_6_8_number(90,3,opt_data.dt);    
			LCD_clear_L(0,4);  display_6_8_string(0,4,"qual:");      		display_6_8_number(50,4,opt_origin_data.qual);            									  
			LCD_clear_L(0,5);  display_6_8_string(0,5,"gyro_x:");        display_6_8_number(50,5,opt_gyro_data.x);              
			LCD_clear_L(0,6);  display_6_8_string(0,6,"gyro_y:");        display_6_8_number(50,6,opt_gyro_data.y);           
			LCD_clear_L(0,7);  display_6_8_string(0,7,"opt_flow:");      display_6_8_number(40,7,opt_gyro_filter_data.x);            display_6_8_number(70,7,opt_gyro_filter_data.y);      
		}
		else if(Page_Number==9)
		{			
			uint16_t x,y,x1,y1;
			ssd1306_clear_display();
			ssd1306_draw_line(31,31,96,31,WHITE);
			ssd1306_draw_line(63,0,63,63,WHITE);
			ssd1306_draw_circle(63,31,31,WHITE);
			x1=(uint16_t)(63-31*Sin_Yaw);
			y1=(uint16_t)(31-31*Cos_Yaw);
			ssd1306_draw_line(63,31,x1,y1,WHITE);
			y=(uint16_t)(31+31*constrain_float(Pitch,-90,90)/90);
			x=(uint16_t)(63+31*constrain_float(Roll,-90,90)/90);
			ssd1306_fill_circle(x,y,5,WHITE);
			ssd1306_display();
			display_6_8_number(115,0,Page_Number+1);
			
			display_6_8_string(60,0,"N");
			display_6_8_string(60,7,"S");
			display_6_8_string(0,1,"Yaw:");            display_6_8_number(90,1,Yaw);                             
			display_6_8_string(0,2,"Pitch:");          display_6_8_number(90,2,Pitch); 
			display_6_8_string(0,3,"Roll:");           display_6_8_number(90,3,Roll); 
		}
		else if(Page_Number==10)
		{
			LCD_clear_L(0,0);
			display_6_8_string(10,0,"Accel_Correct");
			display_6_8_number(100,0,flight_direction);display_6_8_number(115,0,Page_Number+1);
			LCD_clear_L(0,1);
			display_6_8_number(0,1,acce_sample[0].x);
			display_6_8_number(40,1,acce_sample[0].y);
			display_6_8_number(70,1,acce_sample[0].z);
			LCD_clear_L(0,2);
			display_6_8_number(0,2,acce_sample[1].x);
			display_6_8_number(40,2,acce_sample[1].y);
			display_6_8_number(70,2,acce_sample[1].z);
			LCD_clear_L(0,3);
			display_6_8_number(0,3,acce_sample[2].x);
			display_6_8_number(40,3,acce_sample[2].y);
			display_6_8_number(70,3,acce_sample[2].z);
			LCD_clear_L(0,4);
			display_6_8_number(0,4,acce_sample[3].x);
			display_6_8_number(40,4,acce_sample[3].y);
			display_6_8_number(70,4,acce_sample[3].z);
			LCD_clear_L(0,5);
			display_6_8_number(0,5,acce_sample[4].x);
			display_6_8_number(40,5,acce_sample[4].y);
			display_6_8_number(70,5,acce_sample[4].z);
			LCD_clear_L(0,6);
			display_6_8_number(0,6,acce_sample[5].x);
			display_6_8_number(40,6,acce_sample[5].y);
			display_6_8_number(70,6,acce_sample[5].z);
		}
		else if(Page_Number==11)
		{
			LCD_clear_L(0,0);
			display_6_8_string(10,0,"Mag_Correct");
			display_6_8_number(90,0,Mag_Calibration_Mode);display_6_8_number(115,0,Page_Number+1);
			LCD_clear_L(0,1);
			display_6_8_number(0,1, (int16_t)(WP_Sensor.accel_raw.x));
			display_6_8_number(40,1,(int16_t)(WP_Sensor.accel_raw.y));
			display_6_8_number(70,1,(int16_t)(WP_Sensor.accel_raw.z));
			LCD_clear_L(0,2);
			display_6_8_string(0,2,"0 To 360");
			display_6_8_number(70,2,Yaw_Correct);
			LCD_clear_L(0,3);
			display_6_8_string(0,3,"Mag Is Okay?");
			display_6_8_number(80,3,Mag_Is_Okay_Flag[0]);
			display_6_8_number(90,3,Mag_Is_Okay_Flag[1]);
			display_6_8_number(105,3,Mag_Is_Okay_Flag[2]);
			LCD_clear_L(0,4);
			for(i=0;i<12;i++)
			{
				display_6_8_number(10*i,4,Mag_360_Flag[0][3*i]);
			}
			LCD_clear_L(0,5);
			for(i=0;i<12;i++)
			{
				display_6_8_number(10*i,5,Mag_360_Flag[1][3*i]);
			}
			LCD_clear_L(0,6);
			for(i=0;i<12;i++)
			{
				display_6_8_number(10*i,6,Mag_360_Flag[2][3*i]);
			}
			LCD_clear_L(0,7);
			if(Mag_Calibration_Mode==0) 		 display_6_8_string(0,7,"Make Z+ Upside Sky");
			else if(Mag_Calibration_Mode==1) display_6_8_string(0,7,"Make Y+ Upside Sky");
			else if(Mag_Calibration_Mode==2) display_6_8_string(0,7,"Make X+ Upside Sky");
			else if(Mag_Calibration_Mode==3) display_6_8_string(0,7,"Start With Yaw Move"); 
			
			
			//长按上一页校准机架水平
			if(_button.state[UP].press==LONG_PRESS)
			{
				_button.state[UP].press=NO_PRESS;
				cal_flag=0x03;
				cal_step=0x01;
				Bling_Set(&Light_2,500,50,0.2,0,GPIO_PORTF_BASE,GPIO_PIN_2,0);//蓝色				
			}	
			
		}
		else if(Page_Number==12)
		{
			LCD_clear_L(0,0);
			display_6_8_string(10,0,"Sensor Data");   display_6_8_number(115,0,Page_Number+1);
			LCD_clear_L(0,2);display_6_8_string(0,2,"mag:");
			display_6_8_number(25,2,WP_Sensor.mag_raw.x);
			display_6_8_number(60,2,WP_Sensor.mag_raw.y);
			display_6_8_number(95,2,WP_Sensor.mag_raw.z);
			LCD_clear_L(0,3);display_6_8_string(0,3,"acc:");
			display_6_8_number(25,3,WP_Sensor.accel_raw.x);
			display_6_8_number(60,3,WP_Sensor.accel_raw.y);
			display_6_8_number(95,3,WP_Sensor.accel_raw.z);
			LCD_clear_L(0,4);display_6_8_string(0,4,"gyr:");
			display_6_8_number(25,4,WP_Sensor.gyro_raw.x);
			display_6_8_number(60,4,WP_Sensor.gyro_raw.y);
			display_6_8_number(95,4,WP_Sensor.gyro_raw.z);
			LCD_clear_L(0,5);display_6_8_string(0,5,"bar:");
			display_6_8_number(25,5,WP_Sensor.baro_pressure_raw);
			display_6_8_number(60,5,WP_Sensor.baro_temp_raw);
			LCD_clear_L(0,6);display_6_8_string(0,6,"opt:");
			display_6_8_number(25,6,opt_origin_data.pixel_flow_x_integral);
			display_6_8_number(60,6,opt_origin_data.pixel_flow_y_integral);
			LCD_clear_L(0,7);display_6_8_string(0,7,"tfm:");
			display_6_8_number(25,7,tfdata.distance);
			display_6_8_number(65,7,tfdata.strength);
			display_6_8_number(95,7,tfdata.temperature);
		}
		else if(Page_Number==13)
		{
			LCD_clear_L(0,0);display_6_8_number(115,0,Page_Number+1); 
			LCD_clear_L(0,1);  
			display_6_8_string(5,0,"Ground_Sensor");display_6_8_number(110,1,Ground_Sensor_Now_Mode);
			
			LCD_clear_L(0,2); LCD_clear_L(0,3);
			display_6_8_string(5,2,"Now_Mode");
			
			LCD_clear_L(0,4); LCD_clear_L(0,5);  
			if(Ground_Sensor_Now_Mode==0x01)         	 display_6_8_string(5,4,"US100 Mode");
			else if(Ground_Sensor_Now_Mode==0x02)      display_6_8_string(5,4,"tfmini Mode");
			else if(Ground_Sensor_Now_Mode==0x03)      display_6_8_string(5,4,"tofsensor Mode");
			else if(Ground_Sensor_Now_Mode==0x04)      display_6_8_string(5,4,"vl53l1x Mode");
			
			LCD_clear_L(0,6); LCD_clear_L(0,7); 
			if(Pitch_Control>=Scale_Pecent_Max*Pit_Rol_Max)
			{
				delay_ms(10);
				if(Pitch_Control>=Scale_Pecent_Max*Pit_Rol_Max)
				{
					while(Pitch_Control>=Scale_Pecent_Max*Pit_Rol_Max);
					Ground_Sensor_Now_Mode++;
					if(Ground_Sensor_Now_Mode>3) Ground_Sensor_Now_Mode=1;
					WriteFlashParameter(GROUND_DISTANCE_DEFAULT,
															Ground_Sensor_Now_Mode,
															&Table_Parameter);
					ConfigureUART7();
				}
			}
			
			if(_button.state[DOWN].press==LONG_PRESS)
			{
					_button.state[DOWN].press=NO_PRESS;
					Ground_Sensor_Now_Mode++;
					if(Ground_Sensor_Now_Mode>3) Ground_Sensor_Now_Mode=1;
					WriteFlashParameter(GROUND_DISTANCE_DEFAULT,
															Ground_Sensor_Now_Mode,
															&Table_Parameter);
					ConfigureUART7();
					Bling_Set(&Light_2,500,50,0.2,0,GPIO_PORTF_BASE,GPIO_PIN_2,0);//蓝色				
			}				
			
			LCD_clear_L(0,6);display_6_8_string(0,6,"tf:");  							display_6_8_string(50,6,"us:");       display_6_8_string(90,6,"vl:");
			LCD_clear_L(0,7);display_6_8_number(0,7,tfdata.distance);     display_6_8_number(50,7,GD_Distance); display_6_8_number(90,7,vl52l1x_result_data.RangeMilliMeter/10.0);
		}
		else if(Page_Number==14)
		{
			LCD_clear_L(0,0);  display_6_8_number(115,0,Page_Number+1);
											   display_6_8_string(0,0,"ch1:");display_6_8_number(25,0,PPM_Databuf[0]);display_6_8_number(55,0,RC_Data.cal[0].max);display_6_8_number(85,0,RC_Data.cal[0].min);
			LCD_clear_L(0,1);  display_6_8_string(0,1,"ch2:");display_6_8_number(25,1,PPM_Databuf[1]);display_6_8_number(55,1,RC_Data.cal[1].max);display_6_8_number(85,1,RC_Data.cal[1].min);
			LCD_clear_L(0,2);  display_6_8_string(0,2,"ch3:");display_6_8_number(25,2,PPM_Databuf[2]);display_6_8_number(55,2,RC_Data.cal[2].max);display_6_8_number(85,2,RC_Data.cal[2].min);
			LCD_clear_L(0,3);  display_6_8_string(0,3,"ch4:");display_6_8_number(25,3,PPM_Databuf[3]);display_6_8_number(55,3,RC_Data.cal[3].max);display_6_8_number(85,3,RC_Data.cal[3].min);
			LCD_clear_L(0,4);  display_6_8_string(0,4,"ch5:");display_6_8_number(25,4,PPM_Databuf[4]);display_6_8_number(55,4,RC_Data.cal[4].max);display_6_8_number(85,4,RC_Data.cal[4].min);
			LCD_clear_L(0,5);  display_6_8_string(0,5,"ch6:");display_6_8_number(25,5,PPM_Databuf[5]);display_6_8_number(55,5,RC_Data.cal[5].max);display_6_8_number(85,5,RC_Data.cal[5].min);
			LCD_clear_L(0,6);  display_6_8_string(0,6,"ch7:");display_6_8_number(25,6,PPM_Databuf[6]);display_6_8_number(55,6,RC_Data.cal[6].max);display_6_8_number(85,6,RC_Data.cal[6].min);
			LCD_clear_L(0,7);  display_6_8_string(0,7,"ch8:");display_6_8_number(25,7,PPM_Databuf[7]);display_6_8_number(55,7,RC_Data.cal[7].max);display_6_8_number(85,7,RC_Data.cal[7].min);
		}
		else if(Page_Number==15)
		{
			LCD_clear_L(0,0);display_6_8_number(115,0,Page_Number+1);
			
			LCD_clear_L(0,1);display_6_8_string(0,0,"Please Move Thr");
			LCD_clear_L(0,2);LCD_clear_L(0,3);display_6_8_string(0,2,"Up And Pull Out");
			LCD_clear_L(0,4);LCD_clear_L(0,5);display_6_8_string(0,4,"The Power Supply");
			LCD_clear_L(0,6);LCD_clear_L(0,7);display_6_8_string(0,6,"ESC Calibration");
		}
		else if(Page_Number==16)
		{
			display_6_8_string(30,0,"pos:"); display_6_8_number(70,0,NamelessQuad.Position[_YAW]);	display_6_8_number(115,0,Page_Number+1);
			display_6_8_string(30,2,"vel:"); display_6_8_number(70,2,NamelessQuad.Speed[_YAW]);
			display_6_8_string(30,6,"acc:"); display_6_8_number(70,6,NamelessQuad.Acceleration[_YAW]);
			draw_ins_wave();
		} 
		else if(Page_Number==17)
		{
			
			LCD_clear_L(0,0);display_6_8_string(0,0,"tof_id");          display_6_8_string(50,0,"dis");										  display_6_8_string(90,0,"str");		display_6_8_number(115,0,Page_Number+1);
			LCD_clear_L(0,1);display_6_8_number(0,1,tofdata[0].id);		  display_6_8_number(15,1,tofdata[0].distance);				display_6_8_number(65,1,tofdata[0].signal_strength);
											 display_6_8_number(90,1, tofdata[5].distance);
			
			LCD_clear_L(0,2);display_6_8_number(0,2,tofdata[1].id);		  display_6_8_number(15,2,tofdata[1].distance);				display_6_8_number(65,2,tofdata[1].signal_strength);
											 display_6_8_number(90,2, tofdata[6].distance);		
			
			LCD_clear_L(0,3);display_6_8_number(0,3,tofdata[2].id);		  display_6_8_number(15,3,tofdata[2].distance);				display_6_8_number(65,3,tofdata[2].signal_strength);
											 display_6_8_number(90,3, tofdata[7].distance);	
			
			LCD_clear_L(0,4);display_6_8_number(0,4,tofdata[3].id);		  display_6_8_number(15,4,tofdata[3].distance);				display_6_8_number(65,4,tofdata[3].signal_strength);
											 display_6_8_number(90,4, tofdata[8].distance);		
			
			LCD_clear_L(0,5);display_6_8_number(0,5,tofdata[4].id);		  display_6_8_number(15,5,tofdata[4].distance);				display_6_8_number(65,5,tofdata[4].signal_strength);			
											 display_6_8_number(90,5, tofdata[9].distance);			
			
			LCD_clear_L(0,6);display_6_8_string(0,6,"tof_min");		      display_6_8_number(50,6,front_tofsense_distance);				display_6_8_number(90,6,front_tofsense_distance_valid_cnt);	
			LCD_clear_L(0,7);display_6_8_string(0,7,"front_us");        display_6_8_number(70,7,us100_front.distance);
		}
		else if(Page_Number==18)
		{
			LCD_clear_L(0,0);
			display_6_8_string(0,0,"GPS_Date"); display_6_8_number(50,0,0);display_6_8_number(100,0,Page_Number+1);
			LCD_clear_L(0,1);
			display_6_8_string(0,1,"Lo_deg:");
			display_6_8_number(50,1,Longitude_Deg);
			LCD_clear_L(0,2);
			display_6_8_string(0,2,"d/100w:");
			display_6_8_number(50,2,Longitude_Min);
			
			LCD_clear_L(0,3);
			display_6_8_string(0,3,"La_deg:");
			display_6_8_number(50,3,Latitude_Deg);
			LCD_clear_L(0,4);
			display_6_8_string(0,4,"d/100w:");
			display_6_8_number(50,4,Latitude_Min);
			
			LCD_clear_L(0,5);
			if(GPS_Fix_Flag[0]==0x01)
				display_6_8_string(0,5,"fix");
			else display_6_8_string(0,5,"nof");
			
			if(GPS_FixType==0x03)
				display_6_8_string(25,5,"3D");
			else if(GPS_FixType==0x02)
				display_6_8_string(25,5,"2D");
			else display_6_8_string(25,5,"0D");
			display_6_8_number(42,5,GPS_Sate_Num);
			display_6_8_number(60,5,TimeBeijing[0]);
			display_6_8_string(75,5,":");
			display_6_8_number(84,5,TimeBeijing[1]);
			display_6_8_string(100,5,":");
			display_6_8_number(105,5,TimeBeijing[2]);
			LCD_clear_L(0,6);
			display_6_8_string(0,6,"V:");
			display_6_8_number(10,6,GPS_Vel.N);
			display_6_8_number(65,6,GPS_Vel.E);
			LCD_clear_L(0,7);
			display_6_8_number(0,7,Horizontal_Acc_Est);
			display_6_8_number(80,7,GPS_Quality);
		}
		else if(Page_Number==19)
		{
			draw_gps_wave();																						display_6_8_string(30,0,"fus:");   												 display_6_8_string(95,0,"obs:");
			display_6_8_string(0,1,"P_E:");														  display_6_8_number(30,1,NamelessQuad.Position[_PITCH]);    display_6_8_number(95,1,Earth_Frame_To_XYZ.E);
			display_6_8_string(0,2,"P_N:");														  display_6_8_number(30,2,NamelessQuad.Position[_ROLL]);     display_6_8_number(95,2,Earth_Frame_To_XYZ.N);
			display_6_8_string(0,3,"V_E:");														  display_6_8_number(30,3,NamelessQuad.Speed[_PITCH]);    	 display_6_8_number(95,3,GPS_Speed_Resolve[1]);
			display_6_8_string(0,4,"V_N:");														  display_6_8_number(30,4,NamelessQuad.Speed[_ROLL]);    		 display_6_8_number(95,4,GPS_Speed_Resolve[0]);		
			display_6_8_string(0,5,"A_B:");	 														display_6_8_number(30,5,Acce_Bias[_PITCH]);								 display_6_8_number(95,4,GPS_Home_Set);  
			display_6_8_string(0,6,"GPS:"); 														display_6_8_number(30,6,GPS_Sate_Num);	  								 display_6_8_number(100,6,GPS_Quality);
			display_6_8_string(0,7,"Default:");                         display_6_8_number(30,7,Filter_Defeated_Flag);	
		}			
  }
	else
	{
	 LCD_CLS();
	}
}



