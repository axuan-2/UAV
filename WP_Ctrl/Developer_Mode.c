/* Copyright (c)  2018-2025 Wuhan Nameless Innovation Technology Co.,Ltd. All rights reserved.*/
/*----------------------------------------------------------------------------------------------------------------------/
                ��Դ����������ѣ������ߵ���ʷ�Ѿ�֤�����ڹ�����������£���������Ŀ�Դ������������˰����ߣ��Է��ز�����
								Ŀ���Ƶķ�ʽ�в�ͨ���õĿ�Դ��Ŀ��Ҫ��רְ��Ա�����ۺ�����ְ��ֽ�ѧ��ͳ���û�����������ʵ������ɶԲ�
								Ʒ��һ�δ���������������������ۺϿ��ǣ������ɿؿ�Դ�����У������������˾���漤��ܴ��룬�汾������ʵ��
								���ܣ����ڹ�˾��Ʒ���������������Ӱ��ͻ�ѧϰ���������ȫ�����Ÿ��ͻ�ѧϰ���ͻ���ֲ�Ͷ��ο��������뱣����
								���Ȩ��
-----------------------------------------------------------------------------------------------------------------------/
*               ������ֻ��������ѧϰʹ�ã���Ȩ����Ȩ���������ƴ��Ŷӣ������ƴ��Ŷӽ��ɿس���Դ���ṩ�������ߣ�
*               ������ҪΪ�����ƴ��Ŷ��ṩ������δ��������ɣ����ý�Դ�����ṩ�����ˣ����ý�Դ����ŵ����Ϲ�����������أ� 
*               �������Դ�����Ĳ�����緢��������Ϊ�������ƴ��Ŷӽ���֮�Է��ɽ��������
-----------------------------------------------------------------------------------------------------------------------
*                                                 Ϊʲôѡ���������£�
*                                         �ж����ļ۸�������׵Ŀ�Դ�ɿأ�
*                                         ����ҵ������֮������µ��ۺ����
*                                         ׷�����û����飬��Ч����ѧϰ֮·��
*                                         ���²��ٹµ�������������տ�Դ�߶ȣ�
*                                         ��Ӧ���ҷ�ƶ���٣��ٽ��������ƹ�ƽ��
*                                         ��ʱ���ܶ�����ʣ����������˹�ͬ�塣 
-----------------------------------------------------------------------------------------------------------------------
*               ������Ϣ���ܶ���ֹ��ǰ�����������˳���������
*               ��Դ���ף���ѧ����ϧ��ף������Ϯ�����׳ɹ�������
*               ѧϰ�����ߣ��������Ƽ���DJI��ZEROTECH��XAG��AEE��GDU��AUTEL��EWATT��HIGH GREAT�ȹ�˾��ҵ
*               ��ְ�����뷢�ͣ�15671678205@163.com���豸ע��ְ����λ����λ��������
*               �����ƴ���Դ�ɿ�QQȺ��2��Ⱥ465082224��1��Ⱥ540707961����Ա������
*               CSDN���ͣ�http://blog.csdn.net/u011992534
*               �ſ�ID��NamelessCotrun����С��
*               Bվ��ѧ��Ƶ��https://space.bilibili.com/67803559/#/video
*               �ͻ�ʹ���ĵá��Ľ������������http://www.openedv.com/forum.php?mod=viewthread&tid=234214&extra=page=1
*               �Ա����̣�https://shop348646912.taobao.com/?spm=2013.1.1000126.2.5ce78a88ht1sO2
*               �ٶ�����:�����ƴ���Դ�ɿ�
*               ��˾����:www.nameless.tech
*               �������¹����׿�TI��Դ�ɿ���Ƴ��ԡ�֪��ר��:https://zhuanlan.zhihu.com/p/54471146
*               �޸�����:2021/04/30                    
*               �汾��������PLUS����GankerPilot_V3.0
*               ��Ȩ���У�����ؾ���
*               Copyright(C) 2019-2025 �人�������¿Ƽ����޹�˾ 
*               All rights reserved
-----------------------------------------------------------------------------------------------------------------------
*               ��Ҫ��ʾ��
*               �����Ա�����ת�ֵķɿء��������ѡ�����ѧ�ܵĶ����Խ��ۺ�Ⱥѧϰ������
*               ����ֱ�����������������������ϣ��������´�����������Ȩ�����˲��ý�
*               ���ϴ��봫���Ϲ��������أ�������ı��ΪĿ���������ϴ��룬�����д˲�
*               ���ߣ���˾����ǰ��֪����1���ڼ�ʱ�����������ѧУ����λ����������
*               ������ַ��Ϣ�ᱻ�����ڹ�˾�������ٷ�΢�Ź���ƽ̨���ٷ��������͡�֪��
*               ר���Լ��Ա�������ҳ���Թ�ʾ���棬����������Ϊ�����Ϊ�����۵㣬Ӱ��
*               ��ѧ���ҹ���������������ܿ�ͺ������˻����������������ء�
*               �����Ϊ����˾����ش���ʧ�ߣ����Է���;���������л���ĺ�����лл������
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
//SDK��ʱ���
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
				
					//����openmv����
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
						case 2:case 9:case 16:case 23:// �ڶ���У׼
							SDK_DT_Send_Data(2);
						break;
						case 13:// �ڶ���У׼
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
    case 12://�û�����ָ����SDK������ģʽ
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
						Flight_Alt_Hold_Control(ALTHOLD_MANUAL_CTRL,NUL,NUL);//�߶ȿ���
						OpticalFlow_Control_Pure(0);
					}
			}
			else
			{
				if(delay_flag==1) 
					NCQ_SDK_Run();//ˮ�ں�ƽ����+�߶ȿ���
				else
				{
					Flight_Alt_Hold_Control(ALTHOLD_MANUAL_CTRL,NUL,NUL);//�߶ȿ���
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
				Flight_Alt_Hold_Control(ALTHOLD_MANUAL_CTRL,NUL,NUL);//�߶ȿ���
				OpticalFlow_Control_Pure(0);
			}
		}
		break;
    case 1://����OPENMV�Ӿ�׷��ɫ��
		{
			Color_Block_Control_Pliot();//����OPENMV�Ӿ�ˮƽ׷��
			Controller.yaw_outer_control_output  =RC_Data.rc_rpyt[RC_YAW];			
			Flight_Alt_Hold_Control(ALTHOLD_MANUAL_CTRL,NUL,NUL);//�߶ȿ���
		}
		break;
    case 2://����OPENMV�Ӿ�׷��AprilTag�������߼���׷��ɫ��һ��
		{
			Top_APrilTag_Control_Pliot();//����OPENMV�Ӿ�ˮƽ׷��
			Controller.yaw_outer_control_output  =RC_Data.rc_rpyt[RC_YAW];			
			Flight_Alt_Hold_Control(ALTHOLD_MANUAL_CTRL,NUL,NUL);//�߶ȿ���
		}
		break;		
    case 3://����OPENMVѭ�����ƣ�Ĭ�Ϻ��ߣ���ֵ����openmv�����е���
		{
			Self_Track_Control_Pliot();//ѭ�������ں���ˮƽ����+ƫ������
			Flight_Alt_Hold_Control(ALTHOLD_MANUAL_CTRL,NUL,NUL);//�߶ȿ���
			
		}
		break;
    case 4://ǰ��OPENMV�Ӿ�׷��	
		{
			Front_AprilTag_Control_Pliot();//ǰ��OPENMV�Ӿ�׷��
			Controller.yaw_outer_control_output  =RC_Data.rc_rpyt[RC_YAW];
			Flight_Alt_Hold_Control(ALTHOLD_MANUAL_CTRL,NUL,NUL);//�߶ȿ���
		}
		break;		
    case 5:
		{
			Front_Surround_Pole_Control_Pilot();
			Flight_Alt_Hold_Control(ALTHOLD_MANUAL_CTRL,NUL,NUL);//�߶ȿ���
			
		  //Ԥ��ģʽ1,д�ú���Ҫ��break����
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
					 Flight_Alt_Hold_Control(ALTHOLD_MANUAL_CTRL,NUL,NUL);//�߶ȿ���
				 }
			}
			else
				Flight_Alt_Hold_Control(ALTHOLD_AUTO_POS_CTRL,90,NUL);//�߶ȿ���
		  //Ԥ��ģʽ2,д�ú���Ҫ��break����
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
					Flight_Alt_Hold_Control(ALTHOLD_MANUAL_CTRL,NUL,NUL);//�߶ȿ���
					OpticalFlow_Control_Pure(0);
				}
			}
			else
				NCQ_SDK_Run();//ˮ�ں�ƽ����+�߶ȿ�
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
						Flight_Alt_Hold_Control(ALTHOLD_MANUAL_CTRL,NUL,NUL);//�߶ȿ���
						OpticalFlow_Control_Pure(0);
					}
			}
			else
				NCQ_SDK_Run();//ˮ�ں�ƽ����+�߶ȿ�
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
						Flight_Alt_Hold_Control(ALTHOLD_MANUAL_CTRL,NUL,NUL);//�߶ȿ���
						OpticalFlow_Control_Pure(0);
					}
			}
			else
				NCQ_SDK_Run();//ˮ�ں�ƽ����+�߶ȿ�
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
						Flight_Alt_Hold_Control(ALTHOLD_MANUAL_CTRL,NUL,NUL);//�߶ȿ���
						OpticalFlow_Control_Pure(0);
					}
			}
			else
				NCQ_SDK_Run();//ˮ�ں�ƽ����+�߶ȿ�
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
			Flight_Alt_Hold_Control(ALTHOLD_MANUAL_CTRL,NUL,NUL);//�߶ȿ���
		  //Ԥ��ģʽ7,д�ú���Ҫ��break����
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
					 Flight_Alt_Hold_Control(ALTHOLD_MANUAL_CTRL,NUL,NUL);//�߶ȿ���
					 OpticalFlow_Control_Pure(0);
				 }
			}
		  //Ԥ��ģʽ8,д�ú���Ҫ��break����
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
		
		  //Ԥ��ģʽ9,д�ú���Ҫ��break����
		}
    case 14:
		{
			
			A_down_correct();
			land_a_run();
		}
    case 15://ǰ��Ԥ��case�����������ִ�д�����
		{
			Controller.roll_outer_control_output =RC_Data.rc_rpyt[RC_ROLL];
			Controller.pitch_outer_control_output=RC_Data.rc_rpyt[RC_PITCH];
			Controller.yaw_outer_control_output  =RC_Data.rc_rpyt[RC_YAW];
			Flight_Alt_Hold_Control(ALTHOLD_MANUAL_CTRL,NUL,NUL);//�߶ȿ���
		}
		break;
		case 16://SDKģʽ��ԭ�ؽ��������浡�ٺ�ͣ��,��������ִ����ɺ���
		{
			OpticalFlow_Control(0);
			if(Landon_Earth_Flag==1) sdk_16_flag=1;
			Controller.yaw_outer_control_output  =RC_Data.rc_rpyt[RC_YAW];
			if(sdk_16_flag==0)
				Flight_Alt_Hold_Control(ALTHOLD_AUTO_VEL_CTRL,NUL,-40);//�߶ȿ���
			else
			{
				 SDK_Transform_Delay();
	     		if(Delay_Transform_Flag==1)SDK_Transform_function();
				 else 
				 {
					 Flight_Alt_Hold_Control(ALTHOLD_MANUAL_CTRL,NUL,NUL);//�߶ȿ���
				 }
			}
			
		}
    break;	
		case 19://SDKģʽ��ԭ�ؽ��������浡�ٺ�ͣ��,��������ִ����ɺ���
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
					 Flight_Alt_Hold_Control(ALTHOLD_MANUAL_CTRL,NUL,NUL);//�߶ȿ���
				 }
			}
			else
				Flight_Alt_Hold_Control(ALTHOLD_AUTO_POS_CTRL,75,NUL);//�߶ȿ���
		}
		case 20://SDKģʽ��ԭ�ؽ��������浡�ٺ�ͣ��,��������ִ����ɺ���
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
					 Flight_Alt_Hold_Control(ALTHOLD_MANUAL_CTRL,NUL,NUL);//�߶ȿ���
				 }
			}
			else
				Flight_Alt_Hold_Control(ALTHOLD_AUTO_POS_CTRL,90,NUL);//�߶ȿ���
		}
    break;	
    case 21://SDKģʽ��ԭ�ؽ��������浡�ٺ�ͣ��,��������ִ����ɺ���
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
					 Flight_Alt_Hold_Control(ALTHOLD_MANUAL_CTRL,NUL,NUL);//�߶ȿ���
				 }
			}
			else
				Flight_Alt_Hold_Control(ALTHOLD_AUTO_POS_CTRL,65,NUL);//�߶ȿ���
		}
    break;	
		
	  default:
		{
			Controller.roll_outer_control_output =RC_Data.rc_rpyt[RC_ROLL];
			Controller.pitch_outer_control_output=RC_Data.rc_rpyt[RC_PITCH];
			Controller.yaw_outer_control_output  =RC_Data.rc_rpyt[RC_YAW];
			Flight_Alt_Hold_Control(ALTHOLD_MANUAL_CTRL,NUL,NUL);//�߶ȿ���
		}
	}
}


