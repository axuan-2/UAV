/* Copyright (c)  2018-2025 Wuhan Nameless Innovation Technology Co.,Ltd. All rights reserved.*/
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
*               修改日期:2021/04/30                    
*               版本：筑梦者PLUS——GankerPilot_V3.0
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
#include "Developer_Mode.h"
#include "NamelessCotrun_SDK.h"
#include "DIY_OP.h"
int8_t SDK1_Mode_Setup=0x00;
int8_t SDK2_Mode_Setup=0x00;

//int8_t SDK_Mode_Transform[]={12,11,21,11,16,13,20,7,11,19,11,16,13,20,8,11,19,11,16,13,20,9,11,19,11,16,13,20,10,11};
//                           //B   C  D    E          
//int8_t SDK_Mode_Len = 30-1;

int8_t SDK_Mode_Transform[]={12,11,21};
                           //B   C  D    E          
int8_t SDK_Mode_Len = 3-1;
int8_t SDK_TransForm_Flag=0;
extern uint16_t SDK_Duty_Cnt;
//SDK延时起飞
int16_t delay_cnt=500;
bool delay_flag=0;
void Delay_Time()
{
	if(delay_cnt<=0) delay_flag=1;
	else delay_cnt-=1;
}

int16_t Delay_Transform_Time = 160;
bool Delay_Transform_Flag = 0;
void SDK_Transform_Delay(void)
{
			if(Delay_Transform_Time>=0)Delay_Transform_Time--;
			else Delay_Transform_Flag=1;
}
uint8_t SDK_finish_flag=0;
float x_ta,y_ta;
void SDK_Transform_function()
{
		if(SDK_TransForm_Enable)
		{
			if(SDK_TransForm_Flag<SDK_Mode_Len)
		    {
					Delay_Transform_Time=160;
					Delay_Transform_Flag=0;
					SDK_TransForm_Flag+=1;
					SDK1_Mode_Setup=SDK_Mode_Transform[SDK_TransForm_Flag];
				
					//俯视openmv主导
					switch(SDK_TransForm_Flag)
					{
						case 4:case 11:case 18:case 25://bu
						{
							Delay_Transform_Time=210;
							openmv.x_target=0;
							openmv.y_target=0;
							SDK_DT_Send_Data(0);
							openmv.x_target=0;
							openmv.y_target=0;
						}
						break;
						case 2:case 9:case 16:case 23:// 第二次校准
							SDK_DT_Send_Data(2);
						break;
						case 13:// 第二次校准
							Delay_Transform_Time=500;
					}
				}
			else
			{
					//SDK_TransForm_Flag=SDK_Mode_Len;
					//SDK1_Mode_Setup=SDK_Mode_Transform[SDK_TransForm_Flag];
				   SDK1_Mode_Setup=16;
					 SDK_finish_flag=1;
				  // SDK_DT_Send_Data(0x00,1);
			}
		}
		else
				{
					// todo
				}
}
int sdk_16_flag=0;
uint8_t dow_flag,dow_flag_1,dow_flag_2,dow_flag_3,dow_flag_4,dow_flag_5,dow_flag_18;
void Auto_Flight_Ctrl(uint8_t mode)
{
	switch(mode)
	{
    case 12://用户事先指定的SDK开发者模式
		{
			Delay_Time();
			if(SDK_Duty_Cnt==2)  
			{		
				A_cor=20;
				  SDK_Transform_Delay();
	     		  if(Delay_Transform_Flag==1)
						{
							Cali_count=30;
							SDK_Transform_function();
						}
				  else 
					{
						Flight_Alt_Hold_Control(ALTHOLD_MANUAL_CTRL,NUL,NUL);//高度控制
						OpticalFlow_Control_Pure(0);
					}
			}
			else
			{
				if(delay_flag==1) 
					NCQ_SDK_Run();//水内含平控制+高度控制
				else
				{
					Flight_Alt_Hold_Control(ALTHOLD_MANUAL_CTRL,NUL,NUL);//高度控制
					OpticalFlow_Control_Pure(0);
				}
			}
		}
		break;
		case 13:
		{
			SDK_Transform_Delay();
				if(Delay_Transform_Flag==1)SDK_Transform_function();
			else 
			{
				Flight_Alt_Hold_Control(ALTHOLD_MANUAL_CTRL,NUL,NUL);//高度控制
				OpticalFlow_Control_Pure(0);
			}
		}
		break;
    case 1://俯视OPENMV视觉追踪色块
		{
			Color_Block_Control_Pliot();//俯视OPENMV视觉水平追踪
			Controller.yaw_outer_control_output  =RC_Data.rc_rpyt[RC_YAW];			
			Flight_Alt_Hold_Control(ALTHOLD_MANUAL_CTRL,NUL,NUL);//高度控制
		}
		break;
    case 2://俯视OPENMV视觉追踪AprilTag，控制逻辑与追踪色块一致
		{
			Top_APrilTag_Control_Pliot();//俯视OPENMV视觉水平追踪
			Controller.yaw_outer_control_output  =RC_Data.rc_rpyt[RC_YAW];			
			Flight_Alt_Hold_Control(ALTHOLD_MANUAL_CTRL,NUL,NUL);//高度控制
		}
		break;		
    case 3://俯视OPENMV循迹控制，默认黑线，阈值可在openmv程序中调整
		{
			Self_Track_Control_Pliot();//循迹控制内含有水平控制+偏航控制
			Flight_Alt_Hold_Control(ALTHOLD_MANUAL_CTRL,NUL,NUL);//高度控制
			
		}
		break;
    case 4://前视OPENMV视觉追踪	
		{
			Front_AprilTag_Control_Pliot();//前视OPENMV视觉追踪
			Controller.yaw_outer_control_output  =RC_Data.rc_rpyt[RC_YAW];
			Flight_Alt_Hold_Control(ALTHOLD_MANUAL_CTRL,NUL,NUL);//高度控制
		}
		break;		
    case 5:
		{
			Front_Surround_Pole_Control_Pilot();
			Flight_Alt_Hold_Control(ALTHOLD_MANUAL_CTRL,NUL,NUL);//高度控制
			
		  //预留模式1,写好后需要加break跳出
		}
		break;
    case 6:
		{
			if(dow_flag_5==0)
			{
				OpticalFlow_Control_Pure(1);
				dow_flag_5=1;
			}
			else
				OpticalFlow_Control_Pure(0);
//			
			Controller.yaw_outer_control_output  =RC_Data.rc_rpyt[RC_YAW];
			if(ABS(90.0f-NamelessQuad.Position[_YAW])<=5.0f)
			{
				dow_flag_18=0;
				 x_ta=-200;
				 y_ta=0;
				 sdk_16_flag=0;
				 xy_star_flag=0;
				 xy_expe_flag=0;
				 xy_end_flag=0;
				 xy_flag=0;
				 xy_circle_cnt=0;
				 SDK_Transform_Delay();
				 if(Delay_Transform_Flag==1)
					 SDK_Transform_function();
				 else 
				 {
					 Flight_Alt_Hold_Control(ALTHOLD_MANUAL_CTRL,NUL,NUL);//高度控制
				 }
			}
			else
				Flight_Alt_Hold_Control(ALTHOLD_AUTO_POS_CTRL,90,NUL);//高度控制
		  //预留模式2,写好后需要加break跳出
		}
		break;
    case 7:
		{
			if(SDK_Duty_Cnt==7)  
			{		
				A_cor=20;
				SDK_Transform_Delay();
					if(Delay_Transform_Flag==1)SDK_Transform_function();
				else 
				{
					A_flag=0,A_flag_cnt=0,xy_counter=0;
					A_END=0;
					dow_flag_5=0;
					dow_flag_1=0;
					Cali_count=30;
					Flight_Alt_Hold_Control(ALTHOLD_MANUAL_CTRL,NUL,NUL);//高度控制
					OpticalFlow_Control_Pure(0);
				}
			}
			else
				NCQ_SDK_Run();//水内含平控制+高度控
		}
		break;
    case 8:
		{
			if(SDK_Duty_Cnt==12)  
			{		
				A_cor=20;
				  SDK_Transform_Delay();
	     		  if(Delay_Transform_Flag==1)SDK_Transform_function();
				  else 
					{
						A_flag=0,A_flag_cnt=0,xy_counter=0;
						A_END=0;
						dow_flag_5=0;
						dow_flag_1=0;
						Cali_count=30;
						Flight_Alt_Hold_Control(ALTHOLD_MANUAL_CTRL,NUL,NUL);//高度控制
						OpticalFlow_Control_Pure(0);
					}
			}
			else
				NCQ_SDK_Run();//水内含平控制+高度控
		}
		break;
    case 9:
		{
			if(SDK_Duty_Cnt==13)  
			{		
				A_cor=20;
				  SDK_Transform_Delay();
	     		  if(Delay_Transform_Flag==1)SDK_Transform_function();
				  else 
					{
						A_flag=0,A_flag_cnt=0,xy_counter=0;
						A_END=0;
						dow_flag_5=0;
						dow_flag_1=0;
						Cali_count=30;
						Flight_Alt_Hold_Control(ALTHOLD_MANUAL_CTRL,NUL,NUL);//高度控制
						OpticalFlow_Control_Pure(0);
					}
			}
			else
				NCQ_SDK_Run();//水内含平控制+高度控
		}
		break;
    case 10:
		{
			if(SDK_Duty_Cnt==18)  
			{		
				A_cor=20;
				Cali_count=30;
				  SDK_Transform_Delay();
	     		  if(Delay_Transform_Flag==1)SDK_Transform_function();
				  else 
					{
						Flight_Alt_Hold_Control(ALTHOLD_MANUAL_CTRL,NUL,NUL);//高度控制
						OpticalFlow_Control_Pure(0);
					}
			}
			else
				NCQ_SDK_Run();//水内含平控制+高度控
		}
		break;
    case 11:
		{
			A_correct();
		   if(A_END==1)  
			{		
				  SDK_Transform_Delay();
	     		  if(Delay_Transform_Flag==1)SDK_Transform_function();
				  else 
					{
						if(dow_flag_1==0)
						{
							OpticalFlow_Control_Pure(1);
							dow_flag_1=1;
						}
						else
							OpticalFlow_Control_Pure(0);
					}
			}
			Flight_Alt_Hold_Control(ALTHOLD_MANUAL_CTRL,NUL,NUL);//高度控制
		  //预留模式7,写好后需要加break跳出
		}
		break;
    case 18:
		{
			if(dow_flag_18==0)
			{
				OpticalFlow_Control_Pure(1);
				dow_flag=1;
			}
			else
			OpticalFlow_Control_Pure(0);
			if(xy_flag==0)
			move_xy_target(x_ta,y_ta);
			else
			{
				 A_flag=0,A_flag_cnt=0,xy_counter=0;
				 A_END=0;
				 dow_flag_5=0;
				 dow_flag_1=0;
				 SDK_Transform_Delay();
				if(Delay_Transform_Flag==1)SDK_Transform_function();
				 else 
				 {
					 Flight_Alt_Hold_Control(ALTHOLD_MANUAL_CTRL,NUL,NUL);//高度控制
					 OpticalFlow_Control_Pure(0);
				 }
			}
		  //预留模式8,写好后需要加break跳出
		}
    case 29:
		{
			if(SDK_Duty_Status.Status[2].Start_Flag==1
			 &&SDK_Duty_Status.Status[2].Execute_Flag==1
				 &&SDK_Duty_Status.Status[2].End_flag==1)
			{
				SDK_Transform_Delay();
				if(Delay_Transform_Flag==1)SDK_Transform_function();
			}
			
			move_with_z_vel_target(50,70,&SDK_Duty_Status,3-1);
		
		  //预留模式9,写好后需要加break跳出
		}
    case 14:
		{
			
			A_down_correct();
			land_a_run();
		}
    case 15://前面预留case不满足情况下执行此情形
		{
			Controller.roll_outer_control_output =RC_Data.rc_rpyt[RC_ROLL];
			Controller.pitch_outer_control_output=RC_Data.rc_rpyt[RC_PITCH];
			Controller.yaw_outer_control_output  =RC_Data.rc_rpyt[RC_YAW];
			Flight_Alt_Hold_Control(ALTHOLD_MANUAL_CTRL,NUL,NUL);//高度控制
		}
		break;
		case 16://SDK模式中原地降落至地面怠速后停桨,用于任务执行完成后降落
		{
			OpticalFlow_Control(0);
			if(Landon_Earth_Flag==1) sdk_16_flag=1;
			Controller.yaw_outer_control_output  =RC_Data.rc_rpyt[RC_YAW];
			if(sdk_16_flag==0)
				Flight_Alt_Hold_Control(ALTHOLD_AUTO_VEL_CTRL,NUL,-40);//高度控制
			else
			{
				 SDK_Transform_Delay();
	     		if(Delay_Transform_Flag==1)SDK_Transform_function();
				 else 
				 {
					 Flight_Alt_Hold_Control(ALTHOLD_MANUAL_CTRL,NUL,NUL);//高度控制
				 }
			}
			
		}
    break;	
		case 19://SDK模式中原地降落至地面怠速后停桨,用于任务执行完成后降落
		{
			if(dow_flag==0)
			{
				OpticalFlow_Control_Pure(1);
				dow_flag=1;
			}
			else
				OpticalFlow_Control_Pure(0);
//			
			Controller.yaw_outer_control_output  =RC_Data.rc_rpyt[RC_YAW];
			if(ABS(75.0f-NamelessQuad.Position[_YAW])<=5.0f)
			{
				 A_flag=0,A_flag_cnt=0,xy_counter=0;
         A_END=0;
				 dow_flag_1=0;
				 A_cor=7;
				 Cali_count=90;
				 SDK_Transform_Delay();
				 if(Delay_Transform_Flag==1)
					 SDK_Transform_function();
				 else 
				 {
					 Flight_Alt_Hold_Control(ALTHOLD_MANUAL_CTRL,NUL,NUL);//高度控制
				 }
			}
			else
				Flight_Alt_Hold_Control(ALTHOLD_AUTO_POS_CTRL,75,NUL);//高度控制
		}
		case 20://SDK模式中原地降落至地面怠速后停桨,用于任务执行完成后降落
		{
			if(dow_flag_5==0)
			{
				OpticalFlow_Control_Pure(1);
				dow_flag_5=1;
			}
			else
				OpticalFlow_Control_Pure(0);
//			
			Controller.yaw_outer_control_output  =RC_Data.rc_rpyt[RC_YAW];
			if(ABS(90.0f-NamelessQuad.Position[_YAW])<=5.0f)
			{
				 x_ta=-120;
				 y_ta=-120;
				 sdk_16_flag=0;
				 xy_star_flag=0;
				 xy_expe_flag=0;
				 xy_end_flag=0;
				 xy_flag=0;
				dow_flag_18=0;
				 xy_circle_cnt=0;
				 SDK_Transform_Delay();
				 if(Delay_Transform_Flag==1)
					 SDK_Transform_function();
				 else 
				 {
					 Flight_Alt_Hold_Control(ALTHOLD_MANUAL_CTRL,NUL,NUL);//高度控制
				 }
			}
			else
				Flight_Alt_Hold_Control(ALTHOLD_AUTO_POS_CTRL,90,NUL);//高度控制
		}
    break;	
    case 21://SDK模式中原地降落至地面怠速后停桨,用于任务执行完成后降落
		{
			if(dow_flag==0)
			{
				OpticalFlow_Control_Pure(1);
				dow_flag=1;
			}
			else
				OpticalFlow_Control_Pure(0);
//			
			Controller.yaw_outer_control_output  =RC_Data.rc_rpyt[RC_YAW];
			if(ABS(65.0f-NamelessQuad.Position[_YAW])<=5.0f)
			{
				 A_flag=0,A_flag_cnt=0,xy_counter=0;
         A_END=0;
				 dow_flag_1=0;
				 A_cor=7;
				 Cali_count=90;
				 SDK_Transform_Delay();
				 if(Delay_Transform_Flag==1)
					 SDK_Transform_function();
				 else 
				 {
					 Flight_Alt_Hold_Control(ALTHOLD_MANUAL_CTRL,NUL,NUL);//高度控制
				 }
			}
			else
				Flight_Alt_Hold_Control(ALTHOLD_AUTO_POS_CTRL,65,NUL);//高度控制
		}
    break;	
		
	  default:
		{
			Controller.roll_outer_control_output =RC_Data.rc_rpyt[RC_ROLL];
			Controller.pitch_outer_control_output=RC_Data.rc_rpyt[RC_PITCH];
			Controller.yaw_outer_control_output  =RC_Data.rc_rpyt[RC_YAW];
			Flight_Alt_Hold_Control(ALTHOLD_MANUAL_CTRL,NUL,NUL);//高度控制
		}
	}
}


