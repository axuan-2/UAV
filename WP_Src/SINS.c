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
#include "Earth_Declination.h"
#include "SINS.h"


#define Altitute_Kalman_Filter_Enable 1


/***********************************************************
@函数名：Vector_From_BodyFrame2EarthFrame
@入口参数：Vector3f *bf,Vector3f *ef
@出口参数：无
功能描述：载体系向导航系转换
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void Vector_From_BodyFrame2EarthFrame(Vector3f *bf,Vector3f *ef)
{
  ef->x=rMat[0][0]*bf->x+rMat[0][1]*bf->y+rMat[0][2]*bf->z;
  ef->y=rMat[1][0]*bf->x+rMat[1][1]*bf->y+rMat[1][2]*bf->z;
  ef->z=rMat[2][0]*bf->x+rMat[2][1]*bf->y+rMat[2][2]*bf->z;
}

/***********************************************************
@函数名：Vector_From_EarthFrame2BodyFrame
@入口参数：Vector3f *ef,Vector3f *bf
@出口参数：无
功能描述：导航系向载体系转换
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void Vector_From_EarthFrame2BodyFrame(Vector3f *ef,Vector3f *bf)
{
  bf->x=rMat[0][0]*ef->x+rMat[1][0]*ef->y+rMat[2][0]*ef->z;
  bf->y=rMat[0][1]*ef->x+rMat[1][1]*ef->y+rMat[2][1]*ef->z;
  bf->z=rMat[0][2]*ef->x+rMat[1][2]*ef->y+rMat[2][2]*ef->z;
}

SINS NamelessQuad;
SINS Origion_NamelessQuad;
Butter_BufferData accel_feedback_filter_buf[3];
float Acceleration_Feedback[3];
Vector2f SINS_Accel_Body={0,0};
/***********************************************************
@函数名：SINS_Prepare
@入口参数：无
@出口参数：无
功能描述：惯导加速度准备
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void  SINS_Prepare(void)
{
  Vector2f SINS_Accel_Earth={0,0};
  Vector3f Body_Frame,Earth_Frame;
  /*Z-Y-X欧拉角转方向余弦矩阵
  //Pitch Roll  Yaw 分别对应Φ θ Ψ  
  X轴旋转矩阵
  R（Φ）
  {1      0        0    }
  {0      cosΦ    sinΦ}
  {0    -sinΦ    cosΦ }
  
  Y轴旋转矩阵
  R（θ）
  {cosθ     0        -sinθ}
  {0         1        0     }
  {sinθ     0        cosθ}
  
  Z轴旋转矩阵
  R（θ）
  {cosΨ      sinΨ       0}
  {-sinΨ     cosΨ       0}
  {0          0           1 }
  
  由Z-Y-X顺规有:
  载体坐标系到导航坐标系下旋转矩阵R(b2n)
  R(b2n) =R(Ψ)^T*R(θ)^T*R(Φ)^T
  
  R=
  {cosΨ*cosθ     -cosΦ*sinΨ+sinΦ*sinθ*cosΨ        sinΨ*sinΦ+cosΦ*sinθ*cosΨ}
  {cosθ*sinΨ     cosΦ*cosΨ +sinΦ*sinθ*sinΨ       -cosΨ*sinΦ+cosΦ*sinθ*sinΨ}
  {-sinθ          cosθsin Φ                          cosθcosΦ                   }
  */
  Body_Frame.x=ins_accel_filter.x;
  Body_Frame.y=ins_accel_filter.y;
  Body_Frame.z=ins_accel_filter.z;
  Vector_From_BodyFrame2EarthFrame(&Body_Frame,&Earth_Frame);
  Origion_NamelessQuad.Acceleration[_YAW]=Earth_Frame.z;
  Origion_NamelessQuad.Acceleration[_PITCH]=Earth_Frame.x;
  Origion_NamelessQuad.Acceleration[_ROLL]=Earth_Frame.y;
  
  
  Origion_NamelessQuad.Acceleration[_YAW]*=AcceGravity/AcceMax;
  Origion_NamelessQuad.Acceleration[_YAW]-=AcceGravity;//减去重力加速度
  Origion_NamelessQuad.Acceleration[_YAW]*=100;//加速度cm/s^2
  
  Origion_NamelessQuad.Acceleration[_PITCH]*=AcceGravity/AcceMax;
  Origion_NamelessQuad.Acceleration[_PITCH]*=100;//加速度cm/s^2
  
  Origion_NamelessQuad.Acceleration[_ROLL]*=AcceGravity/AcceMax;
  Origion_NamelessQuad.Acceleration[_ROLL]*=100;//加速度cm/s^2
  
  /******************************************************************************/
  //将无人机在导航坐标系下的沿着正东、正北方向的运动加速度旋转到当前航向的运动加速度:机头(俯仰)+横滚
  
  SINS_Accel_Earth.x=Origion_NamelessQuad.Acceleration[_PITCH];//沿地理坐标系，正东方向运动加速度,单位为CM
  SINS_Accel_Earth.y=Origion_NamelessQuad.Acceleration[_ROLL];//沿地理坐标系，正北方向运动加速度,单位为CM
  
  SINS_Accel_Body.x= SINS_Accel_Earth.x*Cos_Yaw+SINS_Accel_Earth.y*Sin_Yaw;  //横滚正向运动加速度  X轴正向
  SINS_Accel_Body.y=-SINS_Accel_Earth.x*Sin_Yaw+SINS_Accel_Earth.y*Cos_Yaw; //机头正向运动加速度  Y轴正向
  

  Body_Frame.x=LPButterworth(accel.x,&accel_feedback_filter_buf[0],&Butter_5HZ_Parameter_Acce);//30
  Body_Frame.y=LPButterworth(accel.y,&accel_feedback_filter_buf[1],&Butter_5HZ_Parameter_Acce);
  Body_Frame.z=LPButterworth(accel.z,&accel_feedback_filter_buf[2],&Butter_5HZ_Parameter_Acce);
	
  Vector_From_BodyFrame2EarthFrame(&Body_Frame,&Earth_Frame);
  Acceleration_Feedback[_YAW]=Earth_Frame.z;
  Acceleration_Feedback[_PITCH]=Earth_Frame.x;
  Acceleration_Feedback[_ROLL]=Earth_Frame.y;
  Acceleration_Feedback[_YAW]*=AcceGravity/AcceMax;
  Acceleration_Feedback[_YAW]-=AcceGravity;//减去重力加速度
  Acceleration_Feedback[_YAW]*=100;//加速度cm/s^2
  Acceleration_Feedback[_PITCH]*=AcceGravity/AcceMax;
  Acceleration_Feedback[_PITCH]*=100;//加速度cm/s^2
  Acceleration_Feedback[_ROLL]*=AcceGravity/AcceMax;
  Acceleration_Feedback[_ROLL]*=100;//加速度cm/s^2
}



/*****************算法技术博客讲解***************************************************
1、四旋翼定高篇之惯导加速度+速度+位置三阶互补融合方案:
http://blog.csdn.net/u011992534/article/details/61924200
2、四旋翼惯导融合之观测传感器滞后问题汇总与巴特沃斯低通滤波器设计
（气压计MS5611、GPS模块M8N、超声波、PX4FLOW等）:
http://blog.csdn.net/u011992534/article/details/73743955
3、从APM源码分析GPS、气压计惯导融合
http://blog.csdn.net/u011992534/article/details/78257684
**********************************************************************************/
float pos_correction[3]={0,0,0};
float acc_correction[3]={0,0,0};
float vel_correction[3]={0,0,0};
float SpeedDealt[3]={0};
/****气压计三阶互补滤波方案——参考开源飞控APM****/
#define TIME_CONTANST_ZER    2.0f
#define K_ACC_ZER 	        (1.0f / (TIME_CONTANST_ZER * TIME_CONTANST_ZER * TIME_CONTANST_ZER))//1
#define K_VEL_ZER	          (3.0f / (TIME_CONTANST_ZER * TIME_CONTANST_ZER))//3
#define K_POS_ZER           (3.0f / TIME_CONTANST_ZER)//3
void Strapdown_INS_High()
{
	static uint16 Speed_Sync_Cnt=0,Pos_Sync_Cnt=0;
	uint16 Cnt=0;
	float Delta_Time,Altitude_Dealt=0;
	Testime SINS_High_Delta;
  Test_Period(&SINS_High_Delta);
  Delta_Time=SINS_High_Delta.Time_Delta/1000.0f;
  if(Delta_Time>2*WP_Duty_Dt||Delta_Time<WP_Duty_Dt||isnan(Delta_Time)!=0)   Delta_Time=WP_Duty_Dt;   
	//由观测量（气压计）得到状态误差
	Altitude_Dealt=constrain_float(Observation_Altitude-NamelessQuad.Pos_History[_YAW][High_Delay_Cnt],-Observation_Err_Max,Observation_Err_Max);//气压计(超声波)与SINS估计量的差，单位cm
  //三路积分反馈量修正惯导
  acc_correction[_YAW] +=Altitude_Dealt* K_ACC_ZER*Delta_Time ;//加速度矫正量
  vel_correction[_YAW] +=Altitude_Dealt* K_VEL_ZER*Delta_Time ;//速度矫正量
  pos_correction[_YAW] +=Altitude_Dealt* K_POS_ZER*Delta_Time ;//位置矫正量
  //加速度计矫正后更新
  NamelessQuad.Last_Acceleration[_YAW]=NamelessQuad.Acceleration[_YAW];//上一次加速度量
  NamelessQuad.Acceleration[_YAW]=Origion_NamelessQuad.Acceleration[_YAW]+acc_correction[_YAW];
  //速度增量矫正后更新，用于更新位置,由于步长h=0.005,相对较长，
  //这里采用二阶龙格库塔法更新微分方程，不建议用更高阶，因为加速度信号非平滑
  SpeedDealt[_YAW]=(NamelessQuad.Last_Acceleration[_YAW]
                    +NamelessQuad.Acceleration[_YAW])*Delta_Time/2.0f;
  //原始位置更新
  Origion_NamelessQuad.Position[_YAW]+=(NamelessQuad.Speed[_YAW]+0.5f*SpeedDealt[_YAW])*Delta_Time;
  //位置矫正后更新
  NamelessQuad.Position[_YAW]=Origion_NamelessQuad.Position[_YAW]+pos_correction[_YAW];
  //原始速度更新
  Origion_NamelessQuad.Speed[_YAW]+=SpeedDealt[_YAW];
  //速度矫正后更新
  NamelessQuad.Speed[_YAW]=Origion_NamelessQuad.Speed[_YAW]+vel_correction[_YAW]; 
	
	Pos_Sync_Cnt++;
	if(Pos_Sync_Cnt>=2)//10ms滑动一次
	{
		for(Cnt=Num-1;Cnt>0;Cnt--)
		{
			NamelessQuad.Pos_History[_YAW][Cnt]=NamelessQuad.Pos_History[_YAW][Cnt-1];
		}
		Pos_Sync_Cnt=0;
	}
	NamelessQuad.Pos_History[_YAW][0]=NamelessQuad.Position[_YAW];
	
	
	Speed_Sync_Cnt++;
	if(Speed_Sync_Cnt>=2)//10ms滑动一次
	{
		for(Cnt=Num-1;Cnt>0;Cnt--)
		{
			NamelessQuad.Vel_History[_YAW][Cnt]=NamelessQuad.Vel_History[_YAW][Cnt-1];
		}
		Speed_Sync_Cnt=0;
	}
	NamelessQuad.Vel_History[_YAW][0]=NamelessQuad.Speed[_YAW];
}



#define Ra  1.0f//1.0
#define KALMAN_DT (Baro_Update_Dt*0.001f)
float Q[2]={0.5f*Ra*KALMAN_DT*KALMAN_DT,Ra*KALMAN_DT};
float R_Baro=35;//75  150  100
float R_Ground=5;
float R_VL53L1X=0.05;
float R_TOFSENSOR=5;
float R=5;
float Acce_Bias_Gain_Kp={
  0.0f,//0.3   0.1~0.75
};
float Acce_Bias_Gain_Ki={
  0.005,//0.005
};
float Pre_conv[4]=
{
  1.10,0.83,//4.95 2.83								//0.18,0.1,
  0.83,1.33	//2.83 3.49								//0.1,0.18
};//上一次协方差
float kalman_k[2]={0};//增益矩阵
/***********************************************************
@函数名：KalmanFilter
@入口参数：float Observation,//位置观测量
					 uint16 Pos_Delay_Cnt,//观测传感器延时量
					 SINS *Ins_Kf,//惯导结构体
					 float System_drive,//系统原始驱动量，惯导加速度
					 float *Q,
					 float R,
					 float dt,
					 uint16 N,
					 uint8_t update_flag
@出口参数：无
功能描述：高度方向惯导卡尔曼滤波
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
float observation_div=0;
float last_observation=0;
float observation_acc;
uint8_t observation_reset_flag=0;
float observation_div_err[10]={0};
float observation_div_rmse[10]={0};

float observation_err[10]={0};
float observation_rmse[10]={0};

float observation_acc_err[10]={0};
float observation_acc_rmse[10]={0};
void  KalmanFilter(float Observation,//位置观测量
                   uint16 Pos_Delay_Cnt,//观测传感器延时量
                   SINS *Ins_Kf,//惯导结构体
                   float System_drive,//系统原始驱动量，惯导加速度
                   float *Q,
                   float R,
                   float dt,
                   uint16 N,
                   uint8_t *update_flag)
{
  uint16 Cnt=0;
  static uint16 Speed_Sync_Cnt=0,Pos_Sync_Cnt=0;
	static int16_t Unusual_Protect_Flag=0;
	static int16_t Unusual_Protect_Cnt=0;
	static uint8_t update_bias=0;
	float R_Temp=0;
  float Temp_conv[4]={0};//先验协方差
  float Conv_Z=0,Z_Cor=0;
  float Ctemp=0;
  float rmse_temp1=0,rmse_temp2,rmse_temp3;
	//if(Unusual_Protect_Flag>0)  Unusual_Protect_Flag--;
	
	R_Temp=constrain_float(R,0,10000);
  //先验状态
  Ins_Kf->Acceleration[N]=System_drive;
  Ins_Kf->Acceleration[N]=Ins_Kf->Acce_Bias_All[N]+Ins_Kf->Acceleration[N];
  Ins_Kf->Position[N] +=Ins_Kf->Speed[N]*dt+(Ins_Kf->Acceleration[N]*dt*dt)/2.0f;
  Ins_Kf->Speed[N]+=Ins_Kf->Acceleration[N]*dt;
  if(*update_flag==1)
  {
		for(uint16_t i=9;i>0;i--)//计算标准差
		{
		  observation_div_err[i]=observation_div_err[i-1];
			observation_div_rmse[i]=observation_div_rmse[i-1];
		  observation_err[i]=observation_err[i-1];
			observation_rmse[i]=observation_rmse[i-1];
			observation_acc_err[i]=observation_acc_err[i-1];
			observation_acc_rmse[i]=observation_acc_rmse[i-1];
		}
		observation_div_err[0]=Butterworth_Buffer_Baro.Output_Butter[2]-Ins_Kf->Vel_History[N][8*Pos_Delay_Cnt];
	  observation_err[0]=Observation-Ins_Kf->Pos_History[N][2*Pos_Delay_Cnt];
		observation_acc_err[0]=Butterworth_Buffer_Baro_Acc.Output_Butter[2]-Ins_Kf->Acce_History[N][8*Pos_Delay_Cnt];
	
		for(int16_t i=9;i>=0;i--)//计算均方根误差
		{
		  rmse_temp1+=observation_div_err[i]*observation_div_err[i];
			rmse_temp2+=observation_err[i]*observation_err[i];
			rmse_temp3+=observation_acc_err[i]*observation_acc_err[i];
		}
		observation_div_rmse[0]=safe_sqrt(rmse_temp1/10.0f);
		observation_rmse[0]=safe_sqrt(rmse_temp2/10.0f);
		observation_acc_rmse[0]=safe_sqrt(rmse_temp3/10.0f);
		
		if(   observation_acc>20000	 //40g，1g=1000cm/s^2 50000cm/s^2约等于30g加速度，多旋翼动力装达不到
				||observation_acc<-15000//-30g失重向下最大1g重力加速
				//||(ABS(observation_div)>20*ABS(Ins_Kf->Vel_History[N][2*Pos_Delay_Cnt])	
		  )
		{
			Observation=Ins_Kf->Pos_History[N][Pos_Delay_Cnt];
			update_bias=0;
			Unusual_Protect_Flag=1;//N ms观测异常保护期内，不更新bias
			Unusual_Protect_Cnt=0;
		}
		else  
		{
			last_observation=Observation;
			update_bias=1;
		}
		
		//last_observation=Observation;
		//update_bias=1;
		
    //先验协方差
    Ctemp=Pre_conv[1]+Pre_conv[3]*KALMAN_DT;
    Temp_conv[0]=Pre_conv[0]+Pre_conv[2]*KALMAN_DT+Ctemp*KALMAN_DT+Q[0];
    Temp_conv[1]=Ctemp;
    Temp_conv[2]=Pre_conv[2]+Pre_conv[3]*KALMAN_DT;;
    Temp_conv[3]=Pre_conv[3]+Q[1];
    //计算卡尔曼增益
    Conv_Z=Temp_conv[0]+R_Temp;
    kalman_k[0]=Temp_conv[0]/Conv_Z;
    kalman_k[1]=Temp_conv[2]/Conv_Z;
    //融合数据输出
		Z_Cor=constrain_float(Observation-Ins_Kf->Pos_History[N][Pos_Delay_Cnt],-Observation_Err_Max,Observation_Err_Max);	
    /*
		if(Observation_Update_Type==1
			&&(observation_rmse[0]>=50
			||observation_div_rmse[0]>100
		  ||observation_acc_rmse[0]>2000)) 
		Z_Cor/=5; 
		*/
    Ins_Kf->Position[N] +=kalman_k[0]*Z_Cor;
    Ins_Kf->Speed[N] +=kalman_k[1]*Z_Cor;
		
		if(update_bias==1||Unusual_Protect_Flag==0)
		{
			Ins_Kf->Acce_Bias[N]+=Acce_Bias_Gain_Ki*Z_Cor;
		  Ins_Kf->Acce_Bias[N]=constrain_float(Ins_Kf->Acce_Bias[N],-200,200);
		  Ins_Kf->Acce_Bias_All[N]=Acce_Bias_Gain_Kp*Z_Cor+Ins_Kf->Acce_Bias[N];			
		}

    //更新状态协方差矩阵
    Pre_conv[0]=(1-kalman_k[0])*Temp_conv[0];
    Pre_conv[1]=(1-kalman_k[0])*Temp_conv[1];
    Pre_conv[2]=Temp_conv[2]-kalman_k[1]*Temp_conv[0];
    Pre_conv[3]=Temp_conv[3]-kalman_k[1]*Temp_conv[1]; 
		*update_flag=0;
				
		//if(ABS(Z_Cor)<=50&&Unusual_Protect_Flag==1) Unusual_Protect_Cnt++;
		if(Unusual_Protect_Flag==1) Unusual_Protect_Cnt++;
	  if(Unusual_Protect_Cnt>=2&&Unusual_Protect_Flag==1)//100ms
		{
			Unusual_Protect_Flag=0;
		  Unusual_Protect_Cnt=0;
		}
	}	
	
	
	Pos_Sync_Cnt++;
	if(Pos_Sync_Cnt>=2)//10ms滑动一次
	{
		for(Cnt=Num-1;Cnt>0;Cnt--)
		{
			Ins_Kf->Pos_History[N][Cnt]=Ins_Kf->Pos_History[N][Cnt-1];
		}
		Pos_Sync_Cnt=0;
  }
	Ins_Kf->Pos_History[N][0]=Ins_Kf->Position[N];
	
	
  Speed_Sync_Cnt++;
  if(Speed_Sync_Cnt>=2)//10ms滑动一次
  {
    for(Cnt=Num-1;Cnt>0;Cnt--)
    {
      Ins_Kf->Vel_History[N][Cnt]=Ins_Kf->Vel_History[N][Cnt-1];
    }
    Speed_Sync_Cnt=0;
  }
  Ins_Kf->Vel_History[N][0]=Ins_Kf->Speed[N];
}


void Alt_SINS_Init(void)
{
	uint16_t _cnt=0;
	if(Sensor_Flag.Ground_Health==1)
	{
		NamelessQuad.Position[_YAW]=GD_Distance;
		for(_cnt=Num-1;_cnt>0;_cnt--){NamelessQuad.Pos_History[_YAW][_cnt]=GD_Distance;}
		NamelessQuad.Pos_History[_YAW][0]=GD_Distance;
		Total_Controller.High_Position_Control.Expect=GD_Distance;//将惯导高度设置为期望高度，有且仅设置一次
		
		Origion_NamelessQuad.Position[_YAW]=GD_Distance;
		Origion_NamelessQuad.Speed[_YAW]=0;
	}
	else
	{
		NamelessQuad.Position[_YAW]=WP_Sensor.baro_altitude;
		for(_cnt=Num-1;_cnt>0;_cnt--){NamelessQuad.Pos_History[_YAW][_cnt]=WP_Sensor.baro_altitude;}
		NamelessQuad.Pos_History[_YAW][0]=WP_Sensor.baro_altitude;
		Total_Controller.High_Position_Control.Expect=GD_Distance;//将惯导高度设置为期望高度，有且仅设置一次
		Origion_NamelessQuad.Position[_YAW]=WP_Sensor.baro_altitude;
		Origion_NamelessQuad.Speed[_YAW]=0;
	}
}


#ifdef Altitute_Kalman_Filter_Enable
	uint16_t SPL06_Sync_Cnt=6;
	uint16_t HCSR04_Sync_Cnt=5;
  uint16_t VL53L1_Sync_Cnt=0;
	uint16_t TOF_Sync_Cnt=5;
#else
	uint16_t SPL06_Sync_Cnt=0;
	uint16_t HCSR04_Sync_Cnt=10;
	uint16_t VL53L1_Sync_Cnt=20;
	uint16_t TOF_Sync_Cnt=10;
#endif


uint16 High_Delay_Cnt=0;
uint8_t Observation_Update_Flag=0;
float Observation_Altitude=0;
uint8_t altitude_updtate_flag=0;
/***********************************************************
@函数名：Observation_Tradeoff
@入口参数：uint8_t HC_SR04_Enable
@出口参数：无
功能描述：高度方向观测传感器自由切换
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void Observation_Tradeoff(uint8_t ground_enable)
{
  static uint8_t observation_flag=2,last_observation_flag=2;
  uint16 Cnt=0;
  last_observation_flag=observation_flag;
  if(ground_enable==0)//无超声波/对地传感器参与
  {
    Observation_Altitude=WP_Sensor.baro_altitude;//高度观测量
    High_Delay_Cnt=SPL06_Sync_Cnt;
		observation_flag=1;
		observation_div=WP_Sensor.baro_altitude_div;
		observation_acc=WP_Sensor.baro_altitude_acc;
		if(WP_Sensor.baro_updtate_flag==1)
		{
			altitude_updtate_flag=WP_Sensor.baro_updtate_flag;
			WP_Sensor.baro_updtate_flag=0;
		}
     R=R_Baro;
  }
  else//有超声波/对地传感器参与
  {
		if(Ground_Sensor_Now_Mode==US100)
		{	
				if(Sensor_Flag.Ground_Health==1)
				{
					Observation_Altitude=GD_Distance;
					High_Delay_Cnt=HCSR04_Sync_Cnt;
					observation_flag=2;
					observation_div=GD_Distance_Div;
					observation_acc=GD_Distance_Acc;
					if(WP_Sensor.us100_updtate_flag==1)
					{
						altitude_updtate_flag=WP_Sensor.us100_updtate_flag;
						WP_Sensor.us100_updtate_flag=0;
					}
					R=R_Ground;
				}
				else
				{
					Observation_Altitude=WP_Sensor.baro_altitude;//高度观测量
					High_Delay_Cnt=SPL06_Sync_Cnt;
					observation_flag=1;
					observation_div=WP_Sensor.baro_altitude_div;
					observation_acc=WP_Sensor.baro_altitude_acc;
					if(WP_Sensor.baro_updtate_flag==1)
					{
						altitude_updtate_flag=WP_Sensor.baro_updtate_flag;
						WP_Sensor.baro_updtate_flag=0;
					}
					R=R_Baro;
				} 
		}
		else if(Ground_Sensor_Now_Mode==TFMINI)
		{	
				if(Sensor_Flag.Ground_Health==1)
				{
					Observation_Altitude=GD_Distance;
					High_Delay_Cnt=5;
					observation_flag=2;
					observation_div=GD_Distance_Div;
					observation_acc=GD_Distance_Acc;
					if(WP_Sensor.tfmini_updtate_flag==1)
					{
						altitude_updtate_flag=WP_Sensor.tfmini_updtate_flag;
						WP_Sensor.tfmini_updtate_flag=0;
					}
					R=R_Ground;
				}
				else
				{
					Observation_Altitude=WP_Sensor.baro_altitude;//高度观测量
					High_Delay_Cnt=SPL06_Sync_Cnt;
					observation_flag=1;
					observation_div=WP_Sensor.baro_altitude_div;
					observation_acc=WP_Sensor.baro_altitude_acc;
					if(WP_Sensor.baro_updtate_flag==1)
					{
						altitude_updtate_flag=WP_Sensor.baro_updtate_flag;
						WP_Sensor.baro_updtate_flag=0;
					}
					R=R_Baro;
				} 
		}
		else if(Ground_Sensor_Now_Mode==TOFSENSOR)
		{	
				if(Sensor_Flag.Ground_Health==1)
				{
					Observation_Altitude=GD_Distance;
					High_Delay_Cnt=TOF_Sync_Cnt;
					observation_flag=2;
					observation_div=GD_Distance_Div;
					observation_acc=GD_Distance_Acc;
					if(WP_Sensor.tofsensor_updtate_flag==1)
					{
						altitude_updtate_flag=WP_Sensor.tofsensor_updtate_flag;
						WP_Sensor.tofsensor_updtate_flag=0;
					}
					R=R_TOFSENSOR;
				}
				else
				{
					Observation_Altitude=WP_Sensor.baro_altitude;//高度观测量
					High_Delay_Cnt=SPL06_Sync_Cnt;
					observation_flag=1;
					observation_div=WP_Sensor.baro_altitude_div;
					observation_acc=WP_Sensor.baro_altitude_acc;
					if(WP_Sensor.baro_updtate_flag==1)
					{
						altitude_updtate_flag=WP_Sensor.baro_updtate_flag;
						WP_Sensor.baro_updtate_flag=0;
					}
					R=R_Baro;
				} 
		} 		
		else if(Ground_Sensor_Now_Mode==VL53L1X)
		{	
				if(Sensor_Flag.Ground_Health==1)
				{
					Observation_Altitude=GD_Distance;
					High_Delay_Cnt=VL53L1_Sync_Cnt;
					observation_flag=2;
					observation_div=GD_Distance_Div;
					observation_acc=GD_Distance_Acc;
					if(WP_Sensor.vl53l1x_updtate_flag==1)
					{
						altitude_updtate_flag=WP_Sensor.vl53l1x_updtate_flag;
						WP_Sensor.vl53l1x_updtate_flag=0;
					}
					R=R_VL53L1X;
				}
				else
				{
					Observation_Altitude=WP_Sensor.baro_altitude;//高度观测量
					High_Delay_Cnt=SPL06_Sync_Cnt;
					observation_flag=1;
					observation_div=WP_Sensor.baro_altitude_div;
					observation_acc=WP_Sensor.baro_altitude_acc;
					if(WP_Sensor.baro_updtate_flag==1)
					{
						altitude_updtate_flag=WP_Sensor.baro_updtate_flag;
						WP_Sensor.baro_updtate_flag=0;
					}
					R=R_Baro;
				} 
		}  		
		
		
    if(observation_flag==2&&last_observation_flag==1)//气压计切超声波
    {
			if(Ground_Sensor_Now_Mode==US100)
			{
				NamelessQuad.Position[_YAW]=GD_Distance;
				for(Cnt=Num-1;Cnt>0;Cnt--){NamelessQuad.Pos_History[_YAW][Cnt]=GD_Distance;}
				NamelessQuad.Pos_History[_YAW][0]=GD_Distance;
				Total_Controller.High_Position_Control.Expect=GD_Distance;//将惯导高度设置为期望高度，有且仅设置一次
				
				Origion_NamelessQuad.Position[_YAW]=GD_Distance;
				Origion_NamelessQuad.Speed[_YAW]=0;
				R=R_Ground;
			}
			else
			{	
				NamelessQuad.Position[_YAW]=GD_Distance;
				for(Cnt=Num-1;Cnt>0;Cnt--){NamelessQuad.Pos_History[_YAW][Cnt]=GD_Distance;}
				NamelessQuad.Pos_History[_YAW][0]=GD_Distance;
				Total_Controller.High_Position_Control.Expect=GD_Distance;//将惯导高度设置为期望高度，有且仅设置一次
				
				Origion_NamelessQuad.Position[_YAW]=GD_Distance;
				Origion_NamelessQuad.Speed[_YAW]=0;
				R=R_Ground;
		  }
			
			//复位三阶互补中间状态
			Origion_NamelessQuad.Position[_YAW]=NamelessQuad.Position[_YAW];
			Origion_NamelessQuad.Speed[_YAW]=NamelessQuad.Speed[_YAW];
			acc_correction[_YAW]=0 ;//加速度矫正量
			vel_correction[_YAW]=0; //速度矫正量
			pos_correction[_YAW]=0; //位置矫正量
			for(uint16_t i=0;i<Num;i++)
			{
				NamelessQuad.Pos_History[_YAW][i]=NamelessQuad.Position[_YAW];
				NamelessQuad.Vel_History[_YAW][i]=NamelessQuad.Speed[_YAW];
			}		
    }
    else if(observation_flag==1&&last_observation_flag==2)//超声波切气压计
    {
      NamelessQuad.Position[_YAW]=Observation_Altitude;
      for(Cnt=Num-1;Cnt>0;Cnt--){NamelessQuad.Pos_History[_YAW][Cnt]=Observation_Altitude;}
      NamelessQuad.Pos_History[_YAW][0]=Observation_Altitude;
      Total_Controller.High_Position_Control.Expect=Observation_Altitude;//将惯导高度设置为期望高度，有且仅设置一次
      Origion_NamelessQuad.Position[_YAW]=Observation_Altitude;
      Origion_NamelessQuad.Speed[_YAW]=0;
			
		  //复位三阶互补中间状态
			Origion_NamelessQuad.Position[_YAW]=NamelessQuad.Position[_YAW];
			Origion_NamelessQuad.Speed[_YAW]=NamelessQuad.Speed[_YAW];
			acc_correction[_YAW]=0 ;//加速度矫正量
			vel_correction[_YAW]=0; //速度矫正量
			pos_correction[_YAW]=0; //位置矫正量
			for(uint16_t i=0;i<Num;i++)
			{
				NamelessQuad.Pos_History[_YAW][i]=NamelessQuad.Position[_YAW];
				NamelessQuad.Vel_History[_YAW][i]=NamelessQuad.Speed[_YAW];
			}
			R=R_Baro;
		}
  }	
}

Testime SINS_High_Delta;
float Delta_T;
/***********************************************************
@函数名：Strapdown_INS_High_Kalman
@入口参数：无
@出口参数：无
功能描述：高度方向卡尔曼滤波
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void Strapdown_INS_High_Kalman(void)
{
  Test_Period(&SINS_High_Delta);
  Delta_T=SINS_High_Delta.Time_Delta/1000.0f;
  if(Delta_T>2*WP_Duty_Dt||Delta_T<WP_Duty_Dt||isnan(Delta_T)!=0)   Delta_T=WP_Duty_Dt;  
	if(Gyro_Safety_Calibration_Flag==1)
	{
		Observation_Tradeoff(1);
#ifdef Altitute_Kalman_Filter_Enable
		KalmanFilter(Observation_Altitude,//位置观测量
								 High_Delay_Cnt,//观测传感器延时量
								 &NamelessQuad,//惯导结构体
								 Origion_NamelessQuad.Acceleration[_YAW],//系统原始驱动量，惯导加速度
								 Q,
								 R,
								 Delta_T,
								 _YAW,
								 &altitude_updtate_flag);
#else		
		Strapdown_INS_High();
#endif
	}
	else
  {
		Alt_SINS_Init();
    acc_correction[_YAW]=-Origion_NamelessQuad.Acceleration[_YAW];
		//NamelessQuad.Acce_Bias[_YAW]=-Origion_NamelessQuad.Acceleration[_YAW];
  }
}

/*************************以下计算球面投影距离内容源于APM3.2 AP.Math.c文件******************************/
/***********************************************************
@函数名：longitude_scale
@入口参数：Location loc
@出口参数：无
功能描述：球面投影
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
float longitude_scale(Location loc)
{
  static int32_t last_lat;
  static float scale = 1.0;
  //比较两次纬度相差值，避免重复运算余弦函数
  if (ABS(last_lat - loc.lat) < 100000) {
    // we are within 0.01 degrees (about 1km) of the
    // same latitude. We can avoid the cos() and return
    // the same scale factor.
    return scale;
  }
  scale = cosf(loc.lat * 1.0e-7f * DEG_TO_RAD);
  scale = constrain_float(scale, 0.01f, 1.0f);
  last_lat = loc.lat;
  return scale;
}
/*
return the distance in meters in North/East plane as a N/E vector
from loc1 to loc2
*/
Vector2f location_diff(Location loc1,Location loc2)
{
  Vector2f Location_Delta;
  Location_Delta.x=(loc2.lat - loc1.lat) * LOCATION_SCALING_FACTOR;//距离单位为m
  Location_Delta.y=(loc2.lng - loc1.lng) * LOCATION_SCALING_FACTOR * longitude_scale(loc2);//距离单位为m
  return Location_Delta;
}

// return distance in meters between two locations
float get_distance(Location loc1,Location loc2){
  float dlat              = (float)(loc2.lat - loc1.lat);
  float dlong             = ((float)(loc2.lng - loc1.lng)) * longitude_scale(loc2);
  return pythagorous2(dlat, dlong) * LOCATION_SCALING_FACTOR;
}
/*************************以上计算球面投影距离内容源于APM3.2 AP.Math.c文件******************************/
float Body_Frame_To_XYZ[3]={0,0,0};//导航（地理）坐标系，机体横滚(Y正)、俯仰(X正)方向位置偏移
Vector3_Nav Earth_Frame_To_XYZ={0,0,0};//导航（地理）坐标系，天 、正北、正东方向上位置偏移
Vector2i GPSData_To_XY_Home={1143637460,306276000};//经、纬度
Location GPS_Home;//初始定位成功点信息
Location GPS_Present;//当前位置点信息
uint8_t GPS_Home_Set=0;
float Declination=0;//地球磁偏角
/***********************************************************
@函数名：Set_GPS_Home
@入口参数：无
@出口参数：无
功能描述：设置GPS原点
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void Set_GPS_Home(void)//设置Home点
{
	static uint16_t gps_home_cnt=0;
  if(GPS_Home_Set==0
	 &&GPS_Sate_Num>=9//星数大于等于9
	 &&Horizontal_Acc_Est<=1.5f)//水平位置估计精度小于1.5m
	 {
		 if(gps_home_cnt<=50) gps_home_cnt++;//刷新10hz，连续5S满足
	 }

   if(GPS_Home_Set==0&&gps_home_cnt==50)//全程只设置一次
  {
    GPSData_To_XY_Home.x=Longitude_Origion;//Longitude;
    GPSData_To_XY_Home.y=Latitude_Origion;//Latitude;
    GPS_Home_Set=1;//设定返航点
    GPS_Home.lng=GPSData_To_XY_Home.x;
    GPS_Home.lat=GPSData_To_XY_Home.y; 
    Strapdown_INS_Reset(&NamelessQuad,_PITCH,Earth_Frame_To_XYZ.E,0);//复位惯导融合
    Strapdown_INS_Reset(&NamelessQuad,_ROLL,Earth_Frame_To_XYZ.N,0);//复位惯导融合
    Declination=get_declination(0.0000001f*GPS_Home.lat,0.0000001f*GPS_Home.lng);//获取当地磁偏角
  }
}

/***********************************************************
@函数名：GPSData_Sort
@入口参数：无
@出口参数：无
功能描述：根据GPS原点，得到经纬度方向的位置偏移
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void GPSData_Sort()
{
  Vector2f location_delta={0,0};
  GPS_Present.lng=Longitude_Origion;//更新当前经纬度
  GPS_Present.lat=Latitude_Origion;
  location_delta=location_diff(GPS_Home,GPS_Present);//根据当前GPS定位信息与Home点位置信息计算正北、正东方向位置偏移
  /***********************************************************************************
  明确下导航系方向，这里正北、正东为正方向:
  沿着正东，经度增加,当无人机相对home点，往正东向移动时，此时GPS_Present.lng>GPS_Home.lng，得到的location_delta.x大于0;
  沿着正北，纬度增加,当无人机相对home点，往正北向移动时，此时GPS_Present.lat>GPS_Home.lat，得到的location_delta.y大于0;
  ******************************************************************************/
  location_delta.x*=100.0f;//沿地理坐标系，正北方向位置偏移,单位为CM
  location_delta.y*=100.0f;//沿地理坐标系，正东方向位置偏移,单位为CM
  Earth_Frame_To_XYZ.E=location_delta.y;//地理系下相对Home点正东位置偏移，单位为CM
  Earth_Frame_To_XYZ.N=location_delta.x;//地理系下相对Home点正北位置偏移，单位为CM
  //将无人机在导航坐标系下的沿着正东、正北方向的位置偏移旋转到当前航向的位置偏移:机头(俯仰)+横滚
  Body_Frame_To_XYZ[_PITCH]=location_delta.x*Cos_Yaw+location_delta.y*Sin_Yaw;//机头正向位置偏移  Y轴正向
  Body_Frame_To_XYZ[_ROLL]=-location_delta.x*Sin_Yaw+location_delta.y*Cos_Yaw;  //横滚正向位置偏移  X轴正向
}



#define Dealt_Update 0.1 //100ms更新一次
#define Process_Noise_Constant 1.0//1.0
#define Pos_Process_Noise  (0.5*Process_Noise_Constant*Dealt_Update*Dealt_Update)//0.005
#define Vel_Process_Noise  (Process_Noise_Constant*Dealt_Update)  //0.1
float   R_GPS[2]={Pos_Process_Noise,Vel_Process_Noise};
float   Q_GPS[2]={0.02,0.45};//{0.015,3.0}{0.075,3.0};
float   R_Acce_bias[2]={0.00025,0};//0.0001  0.001初始易发散
double  Pre_conv_GPS[2][4]=
{
  0.018 ,    0.005,  0.005    , 0.5,//0.0001 ,    0.00001,  0.00001    , 0.003,
  0.018 ,    0.005,  0.005    , 0.5,
};//上一次协方差
double  K_GPS[2][2]={0};//增益矩阵
float   Acce_Bias[2]={0,0};//0  0.001
/***********************************************************
@函数名：KalmanFilter_Horizontal_GPS
@入口参数：float Position_GPS,float Vel_GPS,float Position_Last,
					 float Vel_Last,
					 float *Position,float *Vel,
					 float *Acce,float *R,
					 float *Q,float dt,uint8_t Axis
@出口参数：无
功能描述：GPS水平方向卡尔曼滤波融合
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void   KalmanFilter_Horizontal_GPS(float Position_GPS,float Vel_GPS,float Position_Last,float Vel_Last,float *Position,float *Vel,float *Acce,float *R,float *Q,float dt,uint8_t Axis)
{
  float Conv_Z=0;
  float Z_Delta[2]={0};
  float Conv_Temp=0;
  double Temp_conv[4]={0};//先验协方差
  uint8 Label=0;
  if(Axis=='X') Label=0;
  else Label=1;
  //先验状态
  *Position +=*Vel*dt+((*Acce+Acce_Bias[Label])*dt*dt)/2.0f;
  *Vel+=(*Acce+Acce_Bias[Label])*dt;
  //先验协方差
  Conv_Temp=Pre_conv_GPS[Label][1]+Pre_conv_GPS[Label][3]*dt;
  Temp_conv[0]=Pre_conv_GPS[Label][0]+Pre_conv_GPS[Label][2]*dt+Conv_Temp*dt+R_GPS[0];
  Temp_conv[1]=Conv_Temp;
  Temp_conv[2]=Pre_conv_GPS[Label][2]+Pre_conv_GPS[Label][3]*dt;
  Temp_conv[3]=Pre_conv_GPS[Label][3]+R_GPS[1];
  //计算卡尔曼增益
  Conv_Z=1.0/((Temp_conv[0]+Q_GPS[0]*GPS_Quality)*(Temp_conv[3]+Q_GPS[1]*GPS_Quality)-Temp_conv[1]*Temp_conv[2]);
  //化简如下
  K_GPS[0][0]=( Temp_conv[0]*(Temp_conv[3]+Q_GPS[1]*GPS_Quality)-Temp_conv[1]*Temp_conv[2])*Conv_Z;
  K_GPS[0][1]=(Temp_conv[1]*Q_GPS[0]*GPS_Quality)*Conv_Z;
  K_GPS[1][0]=(Temp_conv[2]*Q_GPS[1]*GPS_Quality)*Conv_Z;
  K_GPS[1][1]=(-Temp_conv[1]*Temp_conv[2]+Temp_conv[3]*(Temp_conv[0]+Q_GPS[0]*GPS_Quality))*Conv_Z;
  //融合数据输出
  Z_Delta[0]=Position_GPS-Position_Last;
  Z_Delta[1]=Vel_GPS-Vel_Last;
	
	Z_Delta[0]=constrain_float(Z_Delta[0],-10000,10000);//100m
	Z_Delta[1]=constrain_float(Z_Delta[1],-2000,2000);//20m/s
	
  *Position +=K_GPS[0][0]*Z_Delta[0]+K_GPS[0][1]*Z_Delta[1];
  *Vel      +=K_GPS[1][0]*Z_Delta[0]+K_GPS[1][1]*Z_Delta[1];
	
  Acce_Bias[Label]+=R_Acce_bias[0]*Z_Delta[0]+R_Acce_bias[1]*Z_Delta[1];
	
	Acce_Bias[Label]=constrain_float(Acce_Bias[Label],-100,100);//100
	
  //更新状态协方差矩阵
  Pre_conv_GPS[Label][0]=(1-K_GPS[0][0])*Temp_conv[0]-K_GPS[0][1]*Temp_conv[2];
  Pre_conv_GPS[Label][1]=(1-K_GPS[0][0])*Temp_conv[1]-K_GPS[0][1]*Temp_conv[3];
  Pre_conv_GPS[Label][2]=(1-K_GPS[1][1])*Temp_conv[2]-K_GPS[1][0]*Temp_conv[0];
  Pre_conv_GPS[Label][3]=(1-K_GPS[1][1])*Temp_conv[3]-K_GPS[1][0]*Temp_conv[1];
}

void Strapdown_INS_Reset(SINS *Ins,uint8_t Axis,float Pos_Target,float Vel_Target)
{
  uint16_t Cnt=0;
  Ins->Position[Axis]=Pos_Target;//位置重置
  Ins->Speed[Axis]=Vel_Target;	 //速度重置
  Ins->Acceleration[Axis]=0.0f;  //加速度清零
  Ins->Acce_Bias[Axis]=0.0f;
  for(Cnt=Num-1;Cnt>0;Cnt--)//历史位置值，全部装载为当前观测值
  {
    Ins->Pos_History[Axis][Cnt]=Pos_Target;
  }
  Ins->Pos_History[Axis][0]=Pos_Target;
  for(Cnt=Num-1;Cnt>0;Cnt--)//历史速度值，全部装载为当前观测值
  {
    Ins->Vel_History[Axis][Cnt]=Vel_Target;
  }
  Ins->Vel_History[Axis][0]=Vel_Target;
}


#define X_Axis  0
#define Y_Axis  1
uint16 GPS_Vel_Delay_Cnt=10;//5:50ms
uint16 GPS_Pos_Delay_Cnt=10;//10:100ms
Testime SINS_Horizontal_Delta;
float Horizontal_Delta_T=0;
uint8_t Filter_Defeated_Flag=0;
void KalmanFilter_Horizontal(void)
{	
  int16 i=0;
	static int16 GPS_Position_Cnt=0;
  if(GPS_Home_Set==1//初始home点已设置
		&&Sensor_Flag.Mag_Health==TRUE)//磁力计数据有效
  {
    GPSData_Sort();
    Test_Period(&SINS_Horizontal_Delta);
    Horizontal_Delta_T=(float)(SINS_Horizontal_Delta.Time_Delta/1000.0f);
    if(Horizontal_Delta_T>2*WP_Duty_Dt||Horizontal_Delta_T<WP_Duty_Dt||isnan(Horizontal_Delta_T)!=0)   Horizontal_Delta_T=WP_Duty_Dt;
    GPS_Position_Cnt++;
    if(GPS_Position_Cnt>=2)//每10ms保存一次
    {	
      for(i=Num-1;i>0;i--)
      {			
				NamelessQuad.Pos_History[_PITCH][i]=NamelessQuad.Pos_History[_PITCH][i-1];
				NamelessQuad.Pos_History[_ROLL][i] =NamelessQuad.Pos_History[_ROLL][i-1];
				NamelessQuad.Vel_History[_PITCH][i]=NamelessQuad.Vel_History[_PITCH][i-1];
				NamelessQuad.Vel_History[_ROLL][i] =NamelessQuad.Vel_History[_ROLL][i-1];	
			}
      NamelessQuad.Pos_History[_PITCH][0]=NamelessQuad.Position[_PITCH];
      NamelessQuad.Pos_History[_ROLL][0] =NamelessQuad.Position[_ROLL];
      NamelessQuad.Vel_History[_PITCH][0]=NamelessQuad.Speed[_PITCH];
      NamelessQuad.Vel_History[_ROLL][0] =NamelessQuad.Speed[_ROLL];			
      GPS_Position_Cnt=0;
    }
    NamelessQuad.Acceleration[_PITCH]=Origion_NamelessQuad.Acceleration[_PITCH];
    NamelessQuad.Acceleration[_ROLL] =Origion_NamelessQuad.Acceleration[_ROLL];
    if(GPS_Update_Flag==1)
    {
      KalmanFilter_Horizontal_GPS(Earth_Frame_To_XYZ.E,
                                  GPS_Speed_Resolve[1],
																	NamelessQuad.Pos_History[_PITCH][GPS_Pos_Delay_Cnt],
																	NamelessQuad.Vel_History[_PITCH][GPS_Vel_Delay_Cnt],
                                  &NamelessQuad.Position[_PITCH],
                                  &NamelessQuad.Speed[_PITCH],
                                  &Origion_NamelessQuad.Acceleration[_PITCH],
                                  R_GPS,Q_GPS,Horizontal_Delta_T,'X');
      KalmanFilter_Horizontal_GPS(Earth_Frame_To_XYZ.N,
                                  GPS_Speed_Resolve[0],
																	NamelessQuad.Pos_History[_ROLL][GPS_Pos_Delay_Cnt],
																	NamelessQuad.Vel_History[_ROLL][GPS_Vel_Delay_Cnt],
                                  &NamelessQuad.Position[_ROLL],
                                  &NamelessQuad.Speed[_ROLL],
                                  &Origion_NamelessQuad.Acceleration[_ROLL],
                                  R_GPS,Q_GPS,Horizontal_Delta_T,'Y');
      GPS_Update_Flag=0;
    }
    else
    {
      NamelessQuad.Position[_PITCH] +=NamelessQuad.Speed[_PITCH]*Horizontal_Delta_T
        +0.5f*((NamelessQuad.Acceleration[_PITCH]+Acce_Bias[0])*Horizontal_Delta_T*Horizontal_Delta_T);
			NamelessQuad.Speed[_PITCH]+=(NamelessQuad.Acceleration[_PITCH]+Acce_Bias[0])*Horizontal_Delta_T;
      
      NamelessQuad.Position[_ROLL] +=NamelessQuad.Speed[_ROLL]*Horizontal_Delta_T
        +0.5f*((NamelessQuad.Acceleration[_ROLL]+Acce_Bias[1])*Horizontal_Delta_T*Horizontal_Delta_T);
      NamelessQuad.Speed[_ROLL]+=(NamelessQuad.Acceleration[_ROLL]+Acce_Bias[1])*Horizontal_Delta_T;
    }
    
    if(ABS(NamelessQuad.Position[_PITCH]-Earth_Frame_To_XYZ.E)>10000
       ||ABS(NamelessQuad.Position[_ROLL]-Earth_Frame_To_XYZ.N)>10000
         ||ABS(NamelessQuad.Speed[_PITCH]-GPS_Speed_Resolve[1])>10000
           ||ABS(NamelessQuad.Speed[_ROLL]-GPS_Speed_Resolve[0])>10000
             )
    {
      Filter_Defeated_Flag=1;//开始时，融合失败标志
    }
  }
}
