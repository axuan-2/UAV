/* Copyright (c)  2019-2025 Wuhan Nameless Innovation Technology Co.,Ltd. All rights reserved.*/
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
*               �޸�����:2021/03/20                  
*               �汾����Ӯ��PRO����CarryPilot_V3.0.4
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
#include "NamelessCotrun_SDK.h"

bool auto_altland(float taret_climb_rate,float target_climb_alt)
{
  return land_althold(taret_climb_rate,target_climb_alt);
}
//SDK��ģʽ��Ҫ�ڹ���ģʽ��ʹ��
uint8_t move_with_speed_target(float x_target,float y_target,float delta,SDK_Status *Status,uint16_t number)
{
  static float end_time=0;
  Vector2f vel_target;
  Testime dt;
  vel_target.x=x_target;
  vel_target.y=y_target;
  Test_Period(&dt);
  ncq_control_althold();//�߶ȿ�����Ȼ����
  if(Status->Status[number].Start_Flag==1
     &&Status->Status[number].Execute_Flag==1
       &&Status->Status[number].End_flag==1)
  {
    OpticalFlow_Control_Pure(0);//���֮�󣬽��й�����ͣ
    return 1;
  }
  else
  {
    if(Status->Status[number].Start_Flag==0) 
    {
      end_time=dt.Now_Time+delta;//��λms 
      Status->Status[number].Start_Flag=1;
    } 
    if(dt.Now_Time>end_time)
    {
      Status->Status[number].Execute_Flag=1;
      Status->Status[number].End_flag=1;
      OpticalFlow_Control_Pure(1);//���֮�󣬽��й�����ͣ
      OpticalFlow_Pos_Ctrl_Expect.x=0;
      OpticalFlow_Pos_Ctrl_Expect.y=0;
      end_time=0;
      Status->Transition_Time[number]=400;//400*5ms=2s
      return 1;//�������
    }
    else
    { 
      OpticalFlow_Pos_Ctrl_Expect.x=0;
      OpticalFlow_Pos_Ctrl_Expect.y=0;
      Status->Status[number].Execute_Flag=1;
      OpticalFlow_Vel_Control(vel_target);//�����ٶ�����
      return 0;
    }
  }
}
uint8_t ffflag=0;

uint8_t move_with_xy_target(float pos_x_target,float pos_y_target,SDK_Status *Status,uint16_t number)
{
  //static Vector2f pos_base;
  ncq_control_althold();//�߶ȿ�����Ȼ����
	if(((openmv.x_target!=0)||(openmv.y_target!=0)) && ((pythagorous2(OpticalFlow_Pos_Ctrl_Expect.x-OpticalFlow_SINS.Position[_PITCH],
                    OpticalFlow_Pos_Ctrl_Expect.y-OpticalFlow_SINS.Position[_ROLL])<=50.0f)))
		ffflag++;
	
  if(Status->Status[number].Start_Flag==0) 
  {
    //pos_base.x=OpticalFlow_SINS.Position[_PITCH];
    //pos_base.y=OpticalFlow_SINS.Position[_ROLL];
    OpticalFlow_Pos_Ctrl_Expect.x=OpticalFlow_SINS.Position[_PITCH]+pos_x_target;
    OpticalFlow_Pos_Ctrl_Expect.y=OpticalFlow_SINS.Position[_ROLL]+pos_y_target;
    Status->Status[number].Start_Flag=1;
  }
  
  if(Status->Status[number].Start_Flag==1
     &&Status->Status[number].Execute_Flag==1
       &&Status->Status[number].End_flag==1)
  {
    OpticalFlow_Control_Pure(0);//���֮�󣬽��й�����ͣ
//		SDK_Duty_Cnt++;
    return 1;
  }
  else
  { 
		
		//���޸�,ʵ�ּ�ͣ
    if((pythagorous2(OpticalFlow_Pos_Ctrl_Expect.x-OpticalFlow_SINS.Position[_PITCH],
                    OpticalFlow_Pos_Ctrl_Expect.y-OpticalFlow_SINS.Position[_ROLL])<=25.0f)||(ffflag>30))
    {
      Status->Status[number].Execute_Flag=1;
      Status->Status[number].End_flag=1;
      //pos_base.x=0;
      //pos_base.y=0;
      OpticalFlow_Pos_Ctrl_Expect.x=0;
      OpticalFlow_Pos_Ctrl_Expect.y=0;
      Status->Transition_Time[number]=70;
      OpticalFlow_Control_Pure(1);//���֮�󣬽��й�����ͣ
			
      return 1;
    }
    else
    { 
      Status->Status[number].Execute_Flag=1; 
      OpticalFlow_Pos_Control();//����λ�ÿ���
//			OpticalFlow_Pos_Ctrl_Output.x*=1.3;
      OpticalFlow_Vel_Control(OpticalFlow_Pos_Ctrl_Output);//�����ٶȿ��� 
      return 0;
    }
  }
}

uint8_t move_with_target(float pos_x_target,float pos_y_target,Duty_Status *Status,uint8_t *Start_flag)
{
  //static Vector2f pos_base;
  ncq_control_althold();//�߶ȿ�����Ȼ����
  if(*Start_flag==1)
  {
    //pos_base.x=OpticalFlow_SINS.Position[_PITCH];
    //pos_base.y=OpticalFlow_SINS.Position[_ROLL];
    *Start_flag=0;
  }
  
  if(Status->Start_Flag==0) 
  {
    OpticalFlow_Pos_Ctrl_Expect.x=OpticalFlow_SINS.Position[_PITCH]+pos_x_target;
    OpticalFlow_Pos_Ctrl_Expect.y=OpticalFlow_SINS.Position[_ROLL]+pos_y_target;
    Status->Start_Flag=1;
  }
  
  if(Status->Start_Flag==1
     &&Status->Execute_Flag==1
       &&Status->End_flag==1)
  {
    OpticalFlow_Control_Pure(0);//���֮�󣬽��й�����ͣ
    return 1;
  }
  else
  {    
    if(pythagorous2(OpticalFlow_Pos_Ctrl_Expect.x-OpticalFlow_SINS.Position[_PITCH],
                    OpticalFlow_Pos_Ctrl_Expect.y-OpticalFlow_SINS.Position[_ROLL])<=8.0f)
    {
      Status->Execute_Flag=1;
      Status->End_flag=1;
      //pos_base.x=0;
      //pos_base.y=0;
      OpticalFlow_Control_Pure(1);//���֮�󣬽��й�����ͣ
      OpticalFlow_Pos_Ctrl_Expect.x=0;
      OpticalFlow_Pos_Ctrl_Expect.y=0;
      return 1;
    }
    else
    { 
      Status->Execute_Flag=1; 
      OpticalFlow_Pos_Control();//����λ�ÿ���
      OpticalFlow_Vel_Control(OpticalFlow_Pos_Ctrl_Output);//�����ٶȿ��� 
      return 0;
    }
  }
}

float pos_x_exce_target,pos_y_exce_target;
Vector2f vel_po_target;
uint8_t move_with_xy_target_vel_po(float pos_x_target,float pos_y_target,float speed_target,SDK_Status *Status,uint16_t number)
{
  //static Vector2f pos_base;
  ncq_control_althold();//�߶ȿ�����Ȼ����
	if(((openmv.x_target!=0)||(openmv.y_target!=0)) && ((pythagorous2(OpticalFlow_Pos_Ctrl_Expect.x-OpticalFlow_SINS.Position[_PITCH],
                    OpticalFlow_Pos_Ctrl_Expect.y-OpticalFlow_SINS.Position[_ROLL])<=50.0f)))
		ffflag++;
	
  if(Status->Status[number].Start_Flag==0) 
  {
    //pos_base.x=OpticalFlow_SINS.Position[_PITCH];
    //pos_base.y=OpticalFlow_SINS.Position[_ROLL];
   // OpticalFlow_Pos_Ctrl_Expect.x=OpticalFlow_SINS.Position[_PITCH]+pos_x_target;
		//OpticalFlow_Pos_Ctrl_Expect.y=OpticalFlow_SINS.Position[_ROLL]+pos_y_target;
		pos_x_exce_target=OpticalFlow_SINS.Position[_PITCH]+pos_x_target;
    pos_y_exce_target=OpticalFlow_SINS.Position[_ROLL]+pos_y_target;
    Status->Status[number].Start_Flag=1;
  }
  
  if(Status->Status[number].Start_Flag==1
     &&Status->Status[number].Execute_Flag==1
       &&Status->Status[number].End_flag==1)
  {
    OpticalFlow_Control_Pure(0);//���֮�󣬽��й�����ͣ
//		SDK_Duty_Cnt++;
    return 1;
  }
  else
  { 
		
		//���޸�,ʵ�ּ�ͣ
    if((pythagorous2(pos_x_exce_target-OpticalFlow_SINS.Position[_PITCH],
                    pos_y_exce_target-OpticalFlow_SINS.Position[_ROLL])<=8.0f)||(ffflag>30))
    {
      Status->Status[number].Execute_Flag=1;
      Status->Status[number].End_flag=1;
      //pos_base.x=0;
      //pos_base.y=0;
      OpticalFlow_Pos_Ctrl_Expect.x=0;
      OpticalFlow_Pos_Ctrl_Expect.y=0;
      Status->Transition_Time[number]=70;
      OpticalFlow_Control_Pure(1);//���֮�󣬽��й�����ͣ
			
      return 1;
    }
    else
    { 
			if((pythagorous2(pos_x_exce_target-OpticalFlow_SINS.Position[_PITCH],
                    pos_y_exce_target-OpticalFlow_SINS.Position[_ROLL])<=35.0f))
			speed_target=0.85f*speed_target;
			if((pythagorous2(pos_x_exce_target-OpticalFlow_SINS.Position[_PITCH],
                    pos_y_exce_target-OpticalFlow_SINS.Position[_ROLL])<=20.0f))
			speed_target=0.6f*speed_target;
			if((pythagorous2(pos_x_exce_target-OpticalFlow_SINS.Position[_PITCH],
                    pos_y_exce_target-OpticalFlow_SINS.Position[_ROLL])<=13.0f))
			speed_target=0.3f*speed_target;
      Status->Status[number].Execute_Flag=1; 
      //OpticalFlow_Pos_Control();//����λ�ÿ���
			vel_po_target.x=speed_target;
			vel_po_target.y=speed_target;
      OpticalFlow_Vel_Control(vel_po_target);//�����ٶȿ��� 
      return 0;
    }
  }
}

uint8_t move_with_z_target(float z_target,float z_vel,float delta,SDK_Status *Status,uint16_t number)
{
  static float target_rate=0;
  static float target_alt=0;
  static uint8_t end_flag=0;
  //static float z_base;
  static float end_time=0;
  Testime dt;
  Test_Period(&dt);
  OpticalFlow_Control_Pure(0);//ˮƽλ�ÿ�����Ȼ����
  
  if(Status->Status[number].Start_Flag==1
     &&Status->Status[number].Execute_Flag==1
       &&Status->Status[number].End_flag==1)
  {   
    ncq_control_althold();
    return 1;
  }
  else
  {
    if(Status->Status[number].Start_Flag==0) 
    {  
      if(z_target==0)//�ٶȿ��ƣ�һ��������ֻ����һ��
      {
        //z_base=NamelessQuad.Position[_YAW];
        target_rate=z_vel;
        target_alt=0;
        end_flag=1;
      }
      else//λ�ÿ��ƣ�һ��������ֻ����һ��
      {
        //z_base=NamelessQuad.Position[_YAW];
        target_rate=0;
        target_alt=NamelessQuad.Position[_YAW]+z_target;
        end_flag=2;
      }
      end_time=dt.Now_Time+delta;//��λms 
      Status->Status[number].Start_Flag=1;
      
      Unwanted_Lock_Flag=0;//�����Զ�����
      OpticalFlow_Pos_Ctrl_Expect.x=0;
      OpticalFlow_Pos_Ctrl_Expect.y=0;
    }
    
    
    if(end_flag==1)
    {  
      auto_altland(target_rate,target_alt);
      if(dt.Now_Time>end_time)
      { 
        end_flag=0;
        target_rate=0;
        target_alt=0;
        //z_base=0;
        end_time=0;
        Status->Status[number].Execute_Flag=1;
        Status->Status[number].End_flag=1;
        Status->Transition_Time[number]=40;
        
        OpticalFlow_Control_Pure(1);//���֮�󣬽��й�����ͣ
        OpticalFlow_Pos_Ctrl_Expect.x=0;
        OpticalFlow_Pos_Ctrl_Expect.y=0;
        
        Total_Controller.High_Position_Control.Expect=0;
        return 1;//�������
      }
      Status->Status[number].Execute_Flag=1;
    }
    else if(end_flag==2)
    {
      auto_altland(target_rate,target_alt);    
      if(ABS(target_alt-NamelessQuad.Position[_YAW])<=5.0f)
      {
        end_flag=0;
        target_rate=0;
        target_alt=0;
        //z_base=0;
        end_time=0;
        Status->Status[number].Execute_Flag=1;
        Status->Status[number].End_flag=1;
        Status->Transition_Time[number]=40;
        
        OpticalFlow_Control_Pure(1);//���֮�󣬽��й�����ͣ
        OpticalFlow_Pos_Ctrl_Expect.x=0;
        OpticalFlow_Pos_Ctrl_Expect.y=0;
        
        Total_Controller.High_Position_Control.Expect=0;
        return 1;
      }
      Status->Status[number].Execute_Flag=1;
    } 
  }
  return 0;
}



uint8_t move_with_z_vel_target(double z_vel,float distance,SDK_Status *Status,uint16_t number)
{
  static double target_rate=0;
  static float target_alt=0;
	static float target_distance=0;
  static uint8_t end_flag=0;	
	target_rate=z_vel;
	target_distance=distance;
  Testime dt;
  Test_Period(&dt);
	OpticalFlow_Control_Pure(0);//ˮƽλ�ÿ�����Ȼ����
  if(Status->Status[number].Start_Flag==1
     &&Status->Status[number].Execute_Flag==1
       &&Status->Status[number].End_flag==1)
  {   
//    Flight_Alt_Hold_Control(ALTHOLD_MANUAL_CTRL,NUL,NUL);//�߶ȿ���
	  Flight_Alt_Hold_Control(ALTHOLD_AUTO_POS_CTRL,distance,NUL);
    return 1;
  }	
	else
	{
	if(Status->Status[number].Start_Flag==0) 
	{
		target_rate=z_vel;
		target_alt=0;
		end_flag=1;
    Status->Status[number].Start_Flag=1;
    Unwanted_Lock_Flag=0;//�����Զ�����
    OpticalFlow_Pos_Ctrl_Expect.x=0;
    OpticalFlow_Pos_Ctrl_Expect.y=0;
	}
	 if(end_flag==1)
    {  
		//auto_altland(target_rate,target_alt);float target_alt,float target_vel
		if(ABS(target_distance-NamelessQuad.Position[_YAW])<=50.0f)
			target_rate=0.8*z_vel;
		if(ABS(target_distance-NamelessQuad.Position[_YAW])<=30.0f)
			target_rate=0.6*z_vel;
		//if(ABS(target_distance-NamelessQuad.Position[_YAW])<=20.0f)target_rate=2/5*z_vel;
	  Flight_Alt_Hold_Control(ALTHOLD_AUTO_VEL_CTRL,target_alt,target_rate);
		if(ABS(target_distance-NamelessQuad.Position[_YAW])<=10.0f)
      {			
        end_flag=0;
        target_rate=0;
        target_alt=0;
        Status->Status[number].Execute_Flag=1;
        Status->Status[number].End_flag=1;
        Status->Transition_Time[number]=70;
        
        OpticalFlow_Control_Pure(1);//���֮�󣬽��й�����ͣ
        OpticalFlow_Pos_Ctrl_Expect.x=0;
        OpticalFlow_Pos_Ctrl_Expect.y=0;
        Total_Controller.High_Position_Control.Expect=0;
        return 1;//�������
      }
			Status->Status[number].Execute_Flag=1;
		}
	}
	return 0;
}

//#define NCQ_SDK_DUTY1 move_with_speed_target(10,0,2000 ,&SDK_Duty_Status,1-1)//��
//#define NCQ_SDK_DUTY2 move_with_speed_target(0,10,2000 ,&SDK_Duty_Status,2-1)//ǰ
//#define NCQ_SDK_DUTY3 move_with_speed_target(-10,0,2000,&SDK_Duty_Status,3-1)//��
//#define NCQ_SDK_DUTY4 move_with_speed_target(0,-10,2000,&SDK_Duty_Status,4-1)//��



#define NCQ_SDK_DUTY1 move_with_z_target(160,0,0,&SDK_Duty_Status,1-1)
#define NCQ_SDK_DUTY2	move_with_xy_target(0,235,&SDK_Duty_Status,2-1)

#define NCQ_SDK_DUTY3 move_with_xy_target(-65,-65,&SDK_Duty_Status,3-1)
#define NCQ_SDK_DUTY4 move_with_z_target(80,0,0,&SDK_Duty_Status,4-1)
#define NCQ_SDK_DUTY5 move_with_xy_target(-80,-80,&SDK_Duty_Status,5-1)
#define NCQ_SDK_DUTY6 move_with_z_target(-30,0,0,&SDK_Duty_Status,6-1)
#define NCQ_SDK_DUTY7 move_with_xy_target(-65,-65,&SDK_Duty_Status,7-1)

#define NCQ_SDK_DUTY8 move_with_xy_target(65,65,&SDK_Duty_Status,8-1)
#define NCQ_SDK_DUTY9 move_with_z_target(60,0,0,&SDK_Duty_Status,9-1)
#define NCQ_SDK_DUTY10 move_with_xy_target(80,80,&SDK_Duty_Status,10-1)
#define NCQ_SDK_DUTY11 move_with_z_target(-30,0,0,&SDK_Duty_Status,11-1)
#define NCQ_SDK_DUTY12 move_with_xy_target(65,65,&SDK_Duty_Status,12-1)

#define NCQ_SDK_DUTY13 move_with_xy_target(-230,0,&SDK_Duty_Status,13-1)

#define NCQ_SDK_DUTY14 move_with_xy_target(65,-65,&SDK_Duty_Status,14-1)
#define NCQ_SDK_DUTY15 move_with_z_target(60,0,0,&SDK_Duty_Status,15-1)
#define NCQ_SDK_DUTY16 move_with_xy_target(95,-95,&SDK_Duty_Status,16-1)
#define NCQ_SDK_DUTY17 move_with_z_target(-30,0,0,&SDK_Duty_Status,17-1)
#define NCQ_SDK_DUTY18 move_with_xy_target(65,-65,&SDK_Duty_Status,18-1)
SDK_Status SDK_Duty_Status;
uint16_t SDK_Duty_Cnt=0;
uint16_t SDK_Transition_Time=0;
int flag_12_=0;
void NCQ_SDK_Run(void)
{
  if(SDK_Duty_Status.Transition_Time[SDK_Duty_Cnt]>=1)
    SDK_Duty_Status.Transition_Time[SDK_Duty_Cnt]--;//���Ź���ʱ��
  
  if(SDK_Duty_Status.Status[SDK_Duty_Cnt].Start_Flag==1
     &&SDK_Duty_Status.Status[SDK_Duty_Cnt].Execute_Flag==1
       &&SDK_Duty_Status.Status[SDK_Duty_Cnt].End_flag==1
         &&SDK_Duty_Status.Transition_Time[SDK_Duty_Cnt]==0)
    {
			SDK_Duty_Cnt++;
			ffflag=0;
			switch(SDK_Duty_Cnt)
			{
				case 1:case 6:case 11://yuan
					SDK_DT_Send_Data(1);
				break;
				case 17://fang
					SDK_DT_Send_Data(3);
					return;
				case 2: case 7:case 12:
					return;
				case 13:
				{
					SDK_DT_Send_Data(1);
					return;	
				}
			}
		}
  
  if(SDK_Duty_Cnt>=NCQ_SDK_DUTY_MAX) SDK_Duty_Cnt=NCQ_SDK_DUTY_MAX;
  
  if(SDK_Duty_Cnt==0)       
	{
		
		NCQ_SDK_DUTY1;
	}		
  else if(SDK_Duty_Cnt==1)   
	{
		if(height_flag==1)
			Total_Controller.High_Position_Control.Expect-=100;
		if(Total_Controller.High_Position_Control.Expect<=0) Total_Controller.High_Position_Control.Expect=0;
		NCQ_SDK_DUTY2;
	}
  else if(SDK_Duty_Cnt==2)   NCQ_SDK_DUTY3;
  else if(SDK_Duty_Cnt==3)   NCQ_SDK_DUTY4;
  else if(SDK_Duty_Cnt==4)   
	{
		NCQ_SDK_DUTY5;
	}
  else if(SDK_Duty_Cnt==5)   NCQ_SDK_DUTY6;
  else if(SDK_Duty_Cnt==6)   NCQ_SDK_DUTY7;
	
	else if(SDK_Duty_Cnt==7)   NCQ_SDK_DUTY8;
  else if(SDK_Duty_Cnt==8)   NCQ_SDK_DUTY9;
  else if(SDK_Duty_Cnt==9)   NCQ_SDK_DUTY10;
  else if(SDK_Duty_Cnt==10)   NCQ_SDK_DUTY11;
	else if(SDK_Duty_Cnt==11)   NCQ_SDK_DUTY12;
	
	else if(SDK_Duty_Cnt==12)   
	{if(flag_12_<=1) {SDK_DT_Send_Data(1);flag_12_++;}NCQ_SDK_DUTY13;}
	
	else if(SDK_Duty_Cnt==13)   NCQ_SDK_DUTY14;
  else if(SDK_Duty_Cnt==14)   NCQ_SDK_DUTY15;
  else if(SDK_Duty_Cnt==15)   NCQ_SDK_DUTY16;
  else if(SDK_Duty_Cnt==16)   NCQ_SDK_DUTY17;
	else if(SDK_Duty_Cnt==17)   NCQ_SDK_DUTY18;
	
  else
  {
    ncq_control_althold();//�߶ȿ���
    OpticalFlow_Control(0);//λ�ÿ���
  }
}


void NCQ_SDK_Reset(void)
{
  uint16_t i=0;
  for(i=0;i<SDK_Duty_Max;i++)
  {
    SDK_Duty_Status.Status[i].Start_Flag=0;
    SDK_Duty_Status.Status[i].Execute_Flag=0;
    SDK_Duty_Status.Status[i].End_flag=0;
  }
  SDK_Duty_Cnt=0;
}





uint8_t data[6]={'0','1','2','3'};
unsigned char sdk_data_to_send[50];
void SDK_DT_Send_Data(int cnt_)
{
//	cJSON* cjson_send = NULL;
//	cjson_send = cJSON_CreateObject();
//	cJSON_AddNumberToObject(cjson_send, "mode", data[0]);
  USART3_Send(&data[cnt_], 1);
//	cJSON_Delete(cjson_send);
}

void SDK_DT_Send_Check(unsigned char mode,COM_SDK com)
{
  sdk_data_to_send[0]=0xFF;
  sdk_data_to_send[1]=0xFE;
  sdk_data_to_send[2]=0xA0;
  sdk_data_to_send[3]=2;
  sdk_data_to_send[4]=mode;
  sdk_data_to_send[5]=0 ;
  u8 sum = 0;
  for(u8 i=0;i<6;i++) sum += sdk_data_to_send[i];
  sdk_data_to_send[6]=sum;
//	if(com==UART3_SDK) USART3_Send(sdk_data_to_send, 7);
	if(com==UART0_SDK) USART0_Send(sdk_data_to_send, 7); 
}



#define SDK_TARGET_X_OFFSET  0
#define SDK_TARGET_Y_OFFSET  0//-12


Target_Check Opv_Top_View_Target,Opv_Front_View_Target;
float SDK_Target_Yaw_Gyro=0;
#define  Pixel_Size    0.0024f//6um=0.000006m=0.0006cm
                       //320---0.0012
                       //160---0.0024
                       //80 ---0.0048
#define  Focal_Length  0.42f

#define OV7725_Sensor_Width_MM    		3.984f//3984um
#define OV7725_Sensor_Height_MM   		2.952f//2952um
#define Pixel_Image_Width    		160//320
#define Pixel_Image_Height   		120//240
#define Pixel_Image_Focal_MM 		4.2f
#define Pixel_Image_View_Angle_X  (56.72/2)//deg(50.75/2)
#define Pixel_Image_View_Angle_Y  (44.07/2)//deg(38.72/2)

#define AprilTag_Side_Length  13.6f//cm13.6

float _Pixel_Image_View_Angle_X,_Pixel_Image_View_Angle_Y;
void Get_Camera_Wide_Angle(float view_angle)
{
	float fh=5.0f/FastTan(0.5f*view_angle*DEG2RAD);
  _Pixel_Image_View_Angle_X=2*RAD2DEG*fast_atan(4/fh);
  _Pixel_Image_View_Angle_Y=2*RAD2DEG*fast_atan(3/fh);
}
	
float _P1=0,_P2=0;
uint16_t _CX=0,_CY=0;
float _TX=0,_TY=0;
float _DX=0,_DY=0;
void Sensor_Parameter_Sort(uint16_t tx,uint16_t ty,float pitch,float roll,float alt)
{
//	Get_Camera_Wide_Angle(68);//��������ͷ��ǣ�����õ�X��Y���ӽ�
	float theta_x_max=0,theta_y_max=0;
	theta_x_max=Pixel_Image_View_Angle_X;
	theta_y_max=Pixel_Image_View_Angle_Y;
  _P1=0.5f*Pixel_Image_Width/FastTan(theta_x_max*DEG2RAD);	
	_P2=0.5f*Pixel_Image_Height/FastTan(theta_y_max*DEG2RAD);
	
	_CX=Pixel_Image_Width/2;
	_CY=Pixel_Image_Height/2;
	
	float tmp_x=0,tmp_y=0;
	tmp_x=fast_atan((_CX-tx)/_P1);
	tmp_y=fast_atan((_CY-ty)/_P2);
	
	_TX= FastTan(tmp_x+roll*DEG2RAD) *_P1;
	_TY= FastTan(tmp_y+pitch*DEG2RAD)*_P2;
	
//	_DX=alt*_TX/_P1;
//	_DY=alt*_TY/_P2;
	_DX=0.5f*alt*_TX/_P1;
	_DY=0.5f*alt*_TY/_P2;

	Opv_Top_View_Target.sdk_target.x=_DX;
  Opv_Top_View_Target.sdk_target.y=_DY;
}


void SDK_DT_Reset()
{
  Opv_Top_View_Target.x=0;
  Opv_Top_View_Target.y=0; 
  Opv_Top_View_Target.pixel=0;
  Opv_Top_View_Target.flag=0;
	Opv_Top_View_Target.state=0;
	Opv_Top_View_Target.angle=0;
	Opv_Top_View_Target.distance=0;
	Opv_Top_View_Target.apriltag_id=0;
	Opv_Top_View_Target.width=0;
	Opv_Top_View_Target.height=0;
	Opv_Top_View_Target.fps=0;
	Opv_Top_View_Target.reserved1=0;
	Opv_Top_View_Target.reserved2=0;
	Opv_Top_View_Target.reserved3=0;
	Opv_Top_View_Target.reserved4=0;
	Opv_Top_View_Target.range_sensor1=0;
	Opv_Top_View_Target.range_sensor2=0;
	Opv_Top_View_Target.range_sensor3=0;
	Opv_Top_View_Target.range_sensor4=0;
	
	
	Opv_Front_View_Target=Opv_Top_View_Target;
}




uint16_t err_cnt[2]={0};
void Openmv_Data_Receive_Anl_1(uint8_t *data_buf,uint8_t num,Target_Check *target)
{
  uint8_t sum = 0;
  for(uint8_t i=0;i<(num-1);i++)  sum+=*(data_buf+i);
  if(!(sum==*(data_buf+num-1))) 	return;//�������У������
  if(!(*(data_buf)==0xFF && *(data_buf+1)==0xFC))return;//������֡ͷ����
	
	target->x				  =*(data_buf+4)<<8|*(data_buf+5);
    target->y					=*(data_buf+6)<<8|*(data_buf+7);
	target->pixel			=*(data_buf+8)<<8|*(data_buf+9);  
	target->flag			=*(data_buf+10);
	target->state		  =*(data_buf+11);		
	target->angle		  =*(data_buf+12)<<8|*(data_buf+13);
	target->distance  =*(data_buf+14)<<8|*(data_buf+15);
	target->apriltag_id=*(data_buf+16)<<8|*(data_buf+17);
	target->width	    =*(data_buf+18)<<8|*(data_buf+19);
	target->height	  =*(data_buf+20)<<8|*(data_buf+21);
	target->fps  		  =*(data_buf+22);
	target->reserved1 =*(data_buf+23);
	target->reserved2 =*(data_buf+24);
	target->reserved3 =*(data_buf+25);
	target->reserved4 =*(data_buf+26);
	//��չ���봫����
	target->range_sensor1 =*(data_buf+27)<<8|*(data_buf+28);
	target->range_sensor2 =*(data_buf+29)<<8|*(data_buf+30);
	target->range_sensor3 =*(data_buf+31)<<8|*(data_buf+32);
	target->range_sensor4 =*(data_buf+33)<<8|*(data_buf+34);

	target->x_pixel_size=Pixel_Size*(Pixel_Image_Width/target->width);
	target->y_pixel_size=Pixel_Size*(Pixel_Image_Height/target->height);

  target->apriltag_distance=AprilTag_Side_Length*Focal_Length/(target->x_pixel_size*FastSqrt(target->pixel));

  target->sdk_mode=*(data_buf+2);
	
  if(*(data_buf+2)==0XA1)//����
  {
		target->target_ctrl_enable=target->flag;		
		if(target->flag!=0)  
		{
			if(target->trust_cnt<20)	 
			{
				target->trust_cnt++;
				target->trust_flag=0;
			}
			else target->trust_flag=1;
		}
		else 
		{
			target->trust_cnt/=2;
		  target->trust_flag=0;
		}

    target->sdk_target_offset.x=SDK_TARGET_X_OFFSET;
    target->sdk_target_offset.y=SDK_TARGET_Y_OFFSET;
			
		Sensor_Parameter_Sort(target->x,target->y,Pitch,Roll,NamelessQuad.Position[_YAW]);
  }
	else if(*(data_buf+2)==0XA2)//AprilTag���
  {
		target->target_ctrl_enable=target->flag;		
		if(target->flag!=0)  
		{
			if(target->trust_cnt<20)	 
			{
				target->trust_cnt++;
				target->trust_flag=0;
			}
			else target->trust_flag=1;
		}
		else 
		{
			target->trust_cnt/=2;
		  target->trust_flag=0;
		}

    target->sdk_target_offset.x=SDK_TARGET_X_OFFSET;
    target->sdk_target_offset.y=SDK_TARGET_Y_OFFSET;
			
		Sensor_Parameter_Sort(target->x,target->y,Pitch,Roll,NamelessQuad.Position[_YAW]);
  }
	else if(*(data_buf+2)==0XA3)//�߼��
  {
		target->target_ctrl_enable=target->flag;		
		if(target->flag!=0)  
		{
			if(target->trust_cnt<20)	 
			{
				target->trust_cnt++;
				target->trust_flag=0;
			}
			else target->trust_flag=1;
		}
		else 
		{
			target->trust_cnt/=2;
		  target->trust_flag=0;
		}

    target->sdk_target_offset.x=SDK_TARGET_X_OFFSET;
    target->sdk_target_offset.y=SDK_TARGET_Y_OFFSET;
			
		Sensor_Parameter_Sort(target->x,target->y,Pitch,Roll,NamelessQuad.Position[_YAW]);
		
		if(target->angle>90) Opv_Top_View_Target.sdk_angle=target->angle-180;
		else Opv_Top_View_Target.sdk_angle=target->angle;	
  }
	else if(*(data_buf+2)==0XA4)//AprilTag���
  {
		target->target_ctrl_enable=target->flag;		
		if(target->flag!=0)  
		{
			if(target->trust_cnt<20)	 
			{
				target->trust_cnt++;
				target->trust_flag=0;
			}
			else target->trust_flag=1;
		}
		else 
		{
			target->trust_cnt/=2;
		  target->trust_flag=0;
		}

    target->sdk_target_offset.x=SDK_TARGET_X_OFFSET;
    target->sdk_target_offset.y=SDK_TARGET_Y_OFFSET;
			
		Sensor_Parameter_Sort(target->x,target->y,Pitch,Roll,NamelessQuad.Position[_YAW]);
  }	
}


void Openmv_Data_Receive_Anl_2(uint8_t *data_buf,uint8_t num,Target_Check *target)
{
  uint8_t sum = 0;
  for(uint8_t i=0;i<(num-1);i++)  sum+=*(data_buf+i);
  if(!(sum==*(data_buf+num-1))) 	return;//�������У������
  if(!(*(data_buf)==0xFF && *(data_buf+1)==0xFC))return;//������֡ͷ����
	
	target->x				  =*(data_buf+4)<<8|*(data_buf+5);
  target->y					=*(data_buf+6)<<8|*(data_buf+7);
	target->pixel			=*(data_buf+8)<<8|*(data_buf+9);  
	target->flag			=*(data_buf+10);
	target->state		  =*(data_buf+11);		
	target->angle		  =*(data_buf+12)<<8|*(data_buf+13);
	target->distance  =*(data_buf+14)<<8|*(data_buf+15);
	target->apriltag_id=*(data_buf+16)<<8|*(data_buf+17);
	target->width	    =*(data_buf+18)<<8|*(data_buf+19);
	target->height	  =*(data_buf+20)<<8|*(data_buf+21);
	target->fps  		  =*(data_buf+22);
	target->reserved1 =*(data_buf+23);
	target->reserved2 =*(data_buf+24);
	target->reserved3 =*(data_buf+25);
	target->reserved4 =*(data_buf+26);
	//��չ���봫����
	target->range_sensor1 =*(data_buf+27)<<8|*(data_buf+28);
	target->range_sensor2 =*(data_buf+29)<<8|*(data_buf+30);
	target->range_sensor3 =*(data_buf+31)<<8|*(data_buf+32);
	target->range_sensor4 =*(data_buf+33)<<8|*(data_buf+34);

	target->x_pixel_size=Pixel_Size*(Pixel_Image_Width/target->width);
	target->y_pixel_size=Pixel_Size*(Pixel_Image_Height/target->height);
	
	
  target->apriltag_distance=AprilTag_Side_Length*Focal_Length/(target->x_pixel_size*FastSqrt(target->pixel));
	
	
	target->sdk_mode=*(data_buf+2);
	
	if(*(data_buf+2)==0XA4)//AprilTag���
  {
		target->target_ctrl_enable=target->flag;		
		if(target->flag!=0)  
		{
			if(target->trust_cnt<20)	 
			{
				target->trust_cnt++;
				target->trust_flag=0;
			}
			else target->trust_flag=1;
		}
		else 
		{
			target->trust_cnt/=2;
		  target->trust_flag=0;
		}

    target->sdk_target_offset.x=SDK_TARGET_X_OFFSET;
    target->sdk_target_offset.y=SDK_TARGET_Y_OFFSET;
    		
    target->sdk_target.x=target->x_cm=(target->x_pixel_size*(0.5*target->width-target->x )*target->apriltag_distance)/Focal_Length;
    target->sdk_target.y=target->y_cm=(target->y_pixel_size*(0.5*target->height-target->y)*target->apriltag_distance)/Focal_Length;
  }
	else if(*(data_buf+2)==0XA5)//��ɫ���˼��
	{
		target->target_ctrl_enable=target->flag;		
		if(target->flag!=0)  
		{
			if(target->trust_cnt<20)	 
			{
				target->trust_cnt++;
				target->trust_flag=0;
			}
			else target->trust_flag=1;
		}
		else 
		{
			target->trust_cnt/=2;
			target->trust_flag=0;
		}	
		//front_tofsense_distance
    target->sdk_target.x=target->x_cm=(target->x_pixel_size*(0.5*target->width-target->x )*front_tofsense_distance)/Focal_Length;
    target->sdk_target.y=target->y_cm=(target->y_pixel_size*(0.5*target->height-target->y)*front_tofsense_distance)/Focal_Length;
	}
	else
	{
	
	}
		
}


uint8_t state = 0;
//uint8_t _buf[2][50];
//void SDK_Data_Receive_Prepare(uint8_t data,uint8_t label)
//{
//  static uint8_t _data_len = 0,_data_cnt = 0;
//  if(state==0&&data==0xFF)//֡ͷ1
//  {
//    state=1;
//    _buf[label][0]=data;
//  }
//  else if(state==1&&data==0xFC)//֡ͷ2
//  {
//    state=2;
//    _buf[label][1]=data;
//  }
//  else if(state==2&&data<0XFF)//�����ֽ�
//  {
//    state=3;
//    _buf[label][2]=data;
//  }
//  else if(state==3&&data<50)//���ݳ���
//  {
//    state = 4;
//    _buf[label][3]=data;
//    _data_len = data;
//    _data_cnt = 0;
//  }
//  else if(state==4&&_data_len>0)//�ж������ݳ��ȣ��ʹ���ٸ�
//  {
//    _data_len--;
//    _buf[label][4+_data_cnt++]=data;
//    if(_data_len==0) state = 5;
//  }
//  else if(state==5)//����������У���
//  {
//    state = 0;
//    _buf[label][4+_data_cnt]=data;
//		if(label==0)  Openmv_Data_Receive_Anl_1(_buf[label],_data_cnt+5,&Opv_Top_View_Target);
//		else Openmv_Data_Receive_Anl_2(_buf[label],_data_cnt+5,&Opv_Front_View_Target);
//  }
//  else state = 0;
//}
uint8_t _data_len = 0,_data_cnt = 0,data__;
int temp_1_;
//void SDK_Data_Receive_Prepare(uint8_t data_)
//{
//	data__=data_;
//  if(state==0&&data__=='*')//֡ͷ1
//    state=1;
//  else if(state==1&&(data__>'0')&&(data__<'9'))//֡ͷ2
//  {
//		_data_cnt++;
//		temp_1_+=(data__-'0')*pow(10,3-_data_cnt);
//		if(_data_cnt==3)
//		{
//			_data_cnt=0;
//			openmv.y_target=temp_1_-360;
//			state=2;
//			temp_1_=0;
//		}
//  }
//  else if(state==2&&data__==':')//�����ֽ�
//    state = 3;
//  else if(state==3&&data__=='*')//���ݳ���
//    state = 4;
//  else if(state==4&&(data__>'0')&&(data__<'9'))//֡ͷ2
//  {
//		_data_cnt++;
//		temp_1_+=(data__-'0')*pow(10,3-_data_cnt);
//		if(_data_cnt==3)
//		{
//			_data_cnt=0;
//			openmv.x_target=temp_1_-360;
//			state=5;
//			temp_1_=0;
//		}
//  }
//  else if(state==5&&data__==';')//����������У���
//		state = 0;
////  else if(state==5&&data=='&')//����������У���
////		state = 7;	
////	else if(state==7&&(data>'0')&&(data<'9'))//֡ͷ2
////  {
////		_data_cnt++;
////		temp_1_+=(data-'0')*pow(10,3-_data_cnt);
////		if(_data_cnt==3)
////		{
////			_data_cnt=0;
////			openmv.state=temp_1_/100;
////			state=7;
////			temp_1_=0;
////		}
////  }
////	else if(state==7&&data=='#')
////		state = 0;
////  else state = 0;
//}
uint8_t bufff[10];
int x_LPF,y_LPF,last_x=0,last_y=0;
void SDK_Data_Receive_Prepare(void)
{
	if(bufff[0]=='*'&&bufff[4]==':')
	{
		last_y=openmv.y_target;
		temp_1_+=(bufff[1]-'0')*100+(bufff[2]-'0')*10+(bufff[3]-'0');
		openmv.y_target=temp_1_-360;
		temp_1_=0;
		y_LPF=(5*openmv.y_target+5*last_y)/10;
	}
	if(bufff[5]=='!'&&bufff[9]==';')
	{
		last_x=openmv.x_target;
		temp_1_+=(bufff[6]-'0')*100+(bufff[7]-'0')*10+(bufff[8]-'0');
		openmv.x_target=temp_1_-360;
		temp_1_=0;
		x_LPF=(5*openmv.x_target+5*last_x)/10;
	}
	
	
}

uint16_t SDK_Data_Offset=0;

void SDK_Data_Prase_1(void)
{
  static uint16_t sdk_prase_cnt=0;
  uint16_t i=0;
  sdk_prase_cnt++;
  if(sdk_prase_cnt>=2)//5*2=10ms
  {
    if(COM3_Rx_Buf.Tail<SDK_Target_Length)//0-11����λ���ڴ���
    {
      SDK_Data_Offset=SDK_Target_Length;
    }
    else//12-23����λ���ڴ���
    {
      SDK_Data_Offset=0;
    }
		for(i=0;i<SDK_Target_Length;i++)
      bufff[i]=COM3_Rx_Buf.Ring_Buff[SDK_Data_Offset+i];
    SDK_Data_Receive_Prepare();
    sdk_prase_cnt=0;
  }
}

void SDK_Data_Prase_2(void)
{
  static uint16_t sdk_prase_cnt=0;
  uint16_t i=0;
  sdk_prase_cnt++;
  if(sdk_prase_cnt>=2)//5*2=10ms
  {
    if(COM0_Rx_Buf.Tail<SDK_Target_Length)//0-11����λ���ڴ���
    {
      SDK_Data_Offset=SDK_Target_Length;
    }
    else//12-23����λ���ڴ���
    {
      SDK_Data_Offset=0;
    }
    for(i=0;i<SDK_Target_Length;i++)
    {
      SDK_Data_Receive_Prepare();
    }
    sdk_prase_cnt=0;
  }
}





void SDK_Init(void)
{
  float sdk_mode_default=0;
  SDK_DT_Reset();//��λSDK�߼������
  ReadFlashParameterOne(SDK1_MODE_DEFAULT,&sdk_mode_default);
  if(isnan(sdk_mode_default)==0)
  {
    SDK1_Mode_Setup=(uint8_t)(sdk_mode_default);
//    SDK_DT_Send_Check(SDK1_Mode_Setup,UART3_SDK);//��ʼ��opemmv����ģʽ��Ĭ�����ϴι���״̬����
  }
	
  ReadFlashParameterOne(SDK2_MODE_DEFAULT,&sdk_mode_default);
  if(isnan(sdk_mode_default)==0)
  {
    SDK2_Mode_Setup=(uint8_t)(sdk_mode_default);
//    SDK_DT_Send_Check(SDK2_Mode_Setup,UART0_SDK);//��ʼ��opemmv����ģʽ��Ĭ�����ϴι���״̬����
  }
}
