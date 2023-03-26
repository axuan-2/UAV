#include "DIY_OP.h"
#include "Headfile.h"
#include "OpticalFlow_Control.h"

DIy_opmv openmv;
int A_flag=0,A_flag_cnt=0,xy_counter=0;
int A_END=0;
uint8_t A_cor=11;
uint8_t Cali_count=0;
void A_correct(void)
{
	A_flag++;
	A_flag_cnt++;
	if(A_END==1) return;
	if((abs(openmv.x_target)<A_cor) && (abs(openmv.y_target)<A_cor) &&( (abs(openmv.x_target)>0)||(abs(openmv.y_target)>0)) )
		xy_counter++;
	else
		xy_counter/=2;
	if(xy_counter>=Cali_count)
	{
		A_END=1;
		
		return;
	}
	if(A_flag_cnt>=1600) A_END=1;
	static uint8_t miss_cnt=1;
	static uint8_t miss_flag=0;
	if(A_flag>=7)
	{
      A_flag=0;
	  if(openmv.state!=1)//目标点检测跟踪
		{			
			miss_cnt=1;
			miss_flag=0;			
			Total_Controller.SDK_Roll_Position_Control.Expect=0;
			if(openmv.x_target>20.0f)
			Total_Controller.SDK_Roll_Position_Control.FeedBack=-0.32f*openmv.x_target;
			else
			Total_Controller.SDK_Roll_Position_Control.FeedBack=-0.38f*openmv.x_target;	
			PID_Control_SDK_Err_LPF(&Total_Controller.SDK_Roll_Position_Control,1);
			
			Total_Controller.SDK_Pitch_Position_Control.Expect=0;
			if(openmv.y_target>20.0f)
			Total_Controller.SDK_Pitch_Position_Control.FeedBack=-0.32f*openmv.y_target;
			else
			Total_Controller.SDK_Pitch_Position_Control.FeedBack=-0.4f*openmv.y_target;	
			PID_Control_SDK_Err_LPF(&Total_Controller.SDK_Pitch_Position_Control,1);
//			openmv.y_target=1000;
//			openmv.x_target=1000;
			OpticalFlow_Pos_Ctrl_Output.x=-Total_Controller.SDK_Roll_Position_Control.Control_OutPut;
			OpticalFlow_Pos_Ctrl_Output.y=-Total_Controller.SDK_Pitch_Position_Control.Control_OutPut;	
		}
		else//丢失目标
		{
		  miss_flag=1;
		}		
	}

	
	if(miss_flag==1)//目标丢失
	{
		if(miss_cnt==1)//初始丢失跟踪目标后，锁定当前位置后，进行普通光流控制
		{
			miss_cnt=2;
			OpticalFlow_Control_Pure(1);//20ms		
		}
		else if(miss_cnt==2)//丢失跟踪目标后，进行普通光流控制
		{
			OpticalFlow_Control_Pure(0);//20ms		
		}
	}
  else//目标未丢失,10ms
	{
		OpticalFlow_Vel_Control(OpticalFlow_Pos_Ctrl_Output);//速度控制周期20ms
	}			
	
}

int down_star_flag=0;
int down_expe_flag=0;
int down_end_flag=0;
int dowm_flag=0;
uint8_t flag1;
void DWOM_correct(double z_vel,float distance)
{
	static double target_rate=0;
  static float target_alt=0;
	static float target_distance=0;
  static uint8_t end_flag=0;	
	target_rate=z_vel;
	target_distance=distance;
  Testime dt;
  Test_Period(&dt);
	OpticalFlow_Control_Pure(0);
//	OpticalFlow_Control(0);
    if(down_star_flag==1
     &&down_expe_flag==1
       &&down_end_flag==1)
    {   
	    Flight_Alt_Hold_Control(ALTHOLD_AUTO_POS_CTRL,distance,NUL);
        return;
    }	
	else
	{
	if(down_star_flag==0) 
	{
		target_rate=z_vel;
		target_alt=0;
		end_flag=1;
        down_star_flag=1;
        Unwanted_Lock_Flag=0;//允许自动上锁
        OpticalFlow_Pos_Ctrl_Expect.x=0;
        OpticalFlow_Pos_Ctrl_Expect.y=0;
	}
	 if(end_flag==1)
    {  
//		//auto_altland(target_rate,target_alt);float target_alt,float target_vel
//		if(ABS(target_distance-NamelessQuad.Position[_YAW])<=30.0f)target_rate=0.8*z_vel;
//		if(ABS(target_distance-NamelessQuad.Position[_YAW])<=15.0f)target_rate=0.6*z_vel;
//		//if(ABS(target_distance-NamelessQuad.Position[_YAW])<=20.0f)target_rate=2/5*z_vel;
	  Flight_Alt_Hold_Control(ALTHOLD_AUTO_VEL_CTRL,target_alt,target_rate);
		if(ABS(target_distance-NamelessQuad.Position[_YAW])<=5.0f )
      {			
        end_flag=0;
        target_rate=0;
        target_alt=0;
        down_expe_flag=1;
        down_end_flag=1;
        
        OpticalFlow_Control_Pure(1);//完成之后，进行光流悬停
        OpticalFlow_Pos_Ctrl_Expect.x=0;
        OpticalFlow_Pos_Ctrl_Expect.y=0;
        Total_Controller.High_Position_Control.Expect=0;
        return;
      }
			down_expe_flag=1;
		}
	}
	return;
}

int A_down_cnt=0;
uint8_t m_flag=0;
void A_down_correct(void)
{
	A_down_cnt++;
	static uint8_t miss_cnt=1;
	static uint8_t miss_flag=0;
	if(A_down_cnt>=8)//目标点检测跟踪
	{
		A_down_cnt=0;
		Total_Controller.SDK_Roll_Position_Control.Expect=0;
		if(openmv.x_target>25.0f)
		Total_Controller.SDK_Roll_Position_Control.FeedBack=-0.30f*openmv.x_target;
		else
		Total_Controller.SDK_Roll_Position_Control.FeedBack=-0.45f*openmv.x_target;	
		PID_Control_SDK_Err_LPF(&Total_Controller.SDK_Roll_Position_Control,1);
		
		Total_Controller.SDK_Pitch_Position_Control.Expect=0;
		if(openmv.y_target>20.0f)
		Total_Controller.SDK_Pitch_Position_Control.FeedBack=-0.20f*openmv.y_target;
		else
		Total_Controller.SDK_Pitch_Position_Control.FeedBack=-0.30f*openmv.y_target;	
		PID_Control_SDK_Err_LPF(&Total_Controller.SDK_Pitch_Position_Control,1);
//			openmv.y_target=1000;
//			openmv.x_target=1000;
		OpticalFlow_Pos_Ctrl_Output.x=-Total_Controller.SDK_Roll_Position_Control.Control_OutPut;
		OpticalFlow_Pos_Ctrl_Output.y=-Total_Controller.SDK_Pitch_Position_Control.Control_OutPut;	
	}
	if(openmv.x_target==0 && openmv.y_target==0)
	{
		miss_flag=1;
		m_flag=1;
	}
	else//丢失目标
	{
		miss_flag=0;
		m_flag=0;
		miss_cnt=1;		
	}		


	if(miss_flag==1)//目标丢失
	{
		if(miss_cnt==1)//初始丢失跟踪目标后，锁定当前位置后，进行普通光流控制
		{
			miss_cnt=2;
			OpticalFlow_Control_Pure(1);//20ms		
		}
		else if(miss_cnt==2)//丢失跟踪目标后，进行普通光流控制
		{
			OpticalFlow_Control_Pure(0);//20ms		
		}
	}
	else//目标未丢失,10ms
	{
		OpticalFlow_Vel_Control(OpticalFlow_Pos_Ctrl_Output);//速度控制周期20ms
	}			
}
int xy_star_flag=0;
int xy_expe_flag=0;
int xy_end_flag=0;
int xy_flag=0;
int xy_circle_cnt=0;
int x_c,y_c;
void move_xy_target(float pos_x_target,float pos_y_target)
{
  //static Vector2f pos_base;
  ncq_control_althold();//高度控制依然进行
	if(((openmv.x_target!=0)||(openmv.y_target!=0)) && ((pythagorous2(OpticalFlow_Pos_Ctrl_Expect.x-OpticalFlow_SINS.Position[_PITCH],
                    OpticalFlow_Pos_Ctrl_Expect.y-OpticalFlow_SINS.Position[_ROLL])<=abs(pos_y_target+pos_x_target)*0.25)))
		xy_circle_cnt++;
	
  if(xy_star_flag==0) 
  {
    //pos_base.x=OpticalFlow_SINS.Position[_PITCH];
    //pos_base.y=OpticalFlow_SINS.Position[_ROLL];
    OpticalFlow_Pos_Ctrl_Expect.x=OpticalFlow_SINS.Position[_PITCH]+pos_x_target;
    OpticalFlow_Pos_Ctrl_Expect.y=OpticalFlow_SINS.Position[_ROLL]+pos_y_target;
		x_c=OpticalFlow_Pos_Ctrl_Expect.x;
		y_c=OpticalFlow_Pos_Ctrl_Expect.y;
    xy_star_flag=1;
  }
  
  if(xy_star_flag==1
     &&xy_expe_flag==1
       &&xy_end_flag==1)
  {
    OpticalFlow_Control_Pure(0);//完成之后，进行光流悬停
	  xy_flag++;
  return;
  }
  else
  { 
		
		//已修改,实现急停
    if((pythagorous2(OpticalFlow_Pos_Ctrl_Expect.x-OpticalFlow_SINS.Position[_PITCH],
                    OpticalFlow_Pos_Ctrl_Expect.y-OpticalFlow_SINS.Position[_ROLL])<=8.0f))
    {
      xy_expe_flag=1;
      xy_end_flag=1;
      //pos_base.x=0;
      //pos_base.y=0;
      OpticalFlow_Pos_Ctrl_Expect.x=0;
      OpticalFlow_Pos_Ctrl_Expect.y=0;
      OpticalFlow_Control_Pure(1);//完成之后，进行光流悬停
			
      OpticalFlow_Pos_Ctrl_Expect.x=0;
      OpticalFlow_Pos_Ctrl_Expect.y=0;
      return;
    }
    else
    { 
      xy_expe_flag=1; 
      OpticalFlow_Pos_Control();//光流位置控制
      OpticalFlow_Vel_Control(OpticalFlow_Pos_Ctrl_Output);//光流速度控制 
    return;
    }
  }
}
