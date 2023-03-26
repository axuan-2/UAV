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
#include "Sensor.h"
#include "Filter.h"

#define  YAW_FUSION_MODE_QUAD  0	//1：偏航角来源于四元数更新，时间长容易偏移，
																	//   针对电赛室内环境磁场干扰大的情况，推荐此方式
																	//0：偏航角来源于偏航角速度与磁力计一阶互补，GPS定点时选择此方式
																	//   推荐采用外置磁力计数据
float Pitch,Roll,Yaw;//俯仰角、横滚角、偏航角
float Yaw_Gyro=0,Pitch_Gyro=0,Roll_Gyro=0;//俯仰角速度、横滚角速度、偏航角速度
float Pitch_Observation,Roll_Observation,Yaw_Observation;//俯仰观测角、横滚观测角、偏航观测角
float Inclination_Rate=0;
Vector3f_Body Circle_Angle;
float Gyro_Length=0;//陀螺仪模长
float qr[4][10]={0};
float K[3]={1.0,1.0,1.0};//默认标度(量程)误差
float B[3]={0,0,0};//默认零位误差
Sensor_Health Sensor_Flag;
float yaw_angle_deg_enu=0;


float invSqrt(float x)
{
  float halfx = 0.5f * x;
  float y = x;
  long i = *(long*)&y;  
  i = 0x5f3759df - (i>>1);
  y = *(float*)&i;
  y = y * (1.5f - (halfx * y * y));
  return y;
}

float Sin_Pitch=0,Sin_Roll=0,Sin_Yaw=0;
float Cos_Pitch=0,Cos_Roll=0,Cos_Yaw=0;
float rMat[3][3];
float sqf(float x) {return ((x)*(x));}
void imuComputeRotationMatrix(void)
{
  Sin_Pitch=sin(Pitch*DEG2RAD);
  Cos_Pitch=cos(Pitch*DEG2RAD);
  Sin_Roll =sin(Roll *DEG2RAD);
  Cos_Roll =cos(Roll *DEG2RAD);
  Sin_Yaw  =sin(Yaw  *DEG2RAD);
  Cos_Yaw  =cos(Yaw  *DEG2RAD);
  Inclination_Rate=(rMat[2][2]-Cos_Pitch * Cos_Roll);
  rMat[0][0]=Cos_Yaw* Cos_Roll;
  rMat[0][1]=Sin_Pitch*Sin_Roll*Cos_Yaw-Cos_Pitch * Sin_Yaw;
  rMat[0][2]=Sin_Pitch * Sin_Yaw+Cos_Pitch * Sin_Roll * Cos_Yaw;  
  rMat[1][0]=Sin_Yaw * Cos_Roll;
  rMat[1][1]=Sin_Pitch * Sin_Roll * Sin_Yaw +Cos_Pitch * Cos_Yaw;
  rMat[1][2]=Cos_Pitch * Sin_Roll * Sin_Yaw - Sin_Pitch * Cos_Yaw;
  rMat[2][0]=-Sin_Roll;
  rMat[2][1]= Sin_Pitch * Cos_Roll;
  rMat[2][2]= Cos_Pitch * Cos_Roll;
}


uint8_t Get_Baro_Offset()
{
  static uint16_t baro_sample_cnt=0;
  if(baro_sample_cnt<=100) 
  {
    baro_sample_cnt++;
  }    
  if(baro_sample_cnt==100) 
  {
    WP_Sensor.baro_presure_offset=WP_Sensor.baro_pressure_raw;
    return 1;
  }
  else if(baro_sample_cnt==101) 
  {
    float Tempbaro=(float)(WP_Sensor.baro_pressure_raw/WP_Sensor.baro_presure_offset)*1.0f;
    WP_Sensor.baro_altitude = 4433000.0f * (1 - powf((float)(Tempbaro),0.190295f));
		if(baro_flag==1)
		{
			WP_Sensor.baro_altitude_div=(WP_Sensor.baro_altitude-WP_Sensor.last_baro_altitude)/(Baro_Update_Dt*0.001f);			
		  WP_Sensor.last_baro_altitude=WP_Sensor.baro_altitude;			
			WP_Sensor.baro_altitude_acc=(WP_Sensor.baro_altitude_div-WP_Sensor.last_baro_altitude_div)/(Baro_Update_Dt*0.001f);
			WP_Sensor.last_baro_altitude_div=WP_Sensor.baro_altitude_div;
			baro_flag=0;
		}
    return 1;
  }
  return 0;
}


float Accel_For_Cal[3]={0};
Vector3f gyro,accel,mag;
Vector3f gyro_filter,accel_filter,mag_filter,gyro_filter_bug;
Vector3f gyro_bpf_filter,accel_bpf_filter,accel_bpf_filter_bug;
Vector3f ins_accel_filter;
Butter_BufferData ins_accel_filter_buf[3],gyro_filter_buf_bug[3],gyro_filter_buf[3],accel_filter_buf[3],mag_filter_buf[3],accel_cal_filter_buf[3],accel_for_cal_filter_buf[3];
Butter_BufferData gyro_bpf_filter_buf[3],accel_bpf_filter_buf[3],accel_bpf_filter_buf_bug[3];
uint8_t mag_update_flag=0;
Butter_BufferData imu_temperature;
void Sensor_Update(void)
{
  //传感器数据0.5~0.8ms采集完毕
  ICM20689_Read_Data(&WP_Sensor.gyro_raw,&WP_Sensor.accel_raw,&WP_Sensor.temperature);
  WP_Sensor.imu_updtate_flag=1;
	if(Gyro_Safety_Calibration_Flag==0)  
		Gyro_Safety_Calibration_Flag=Gyro_Calibration_Check(&WP_Sensor.gyro_raw);
	if(cal_flag==0x01) 
  {
		Gyro_Safety_Calibration_Flag=0;
		cal_flag=0x00;
	}	
  mag_update_flag=Compass_Read_Data(&WP_Sensor.mag_raw);
	Compass_Fault_Check();
  SPL06_Read_Data(&WP_Sensor.baro_temp_raw,&WP_Sensor.baro_pressure_raw);
  Get_Baro_Offset();
  
  //得到校准后的角速度、加速度、磁力计数据
  gyro.x=WP_Sensor.gyro_raw.x-gyro_offset.x;
  gyro.y=WP_Sensor.gyro_raw.y-gyro_offset.y;
  gyro.z=WP_Sensor.gyro_raw.z-gyro_offset.z;
  
  accel.x=K[0]*WP_Sensor.accel_raw.x-B[0]*One_G_TO_Accel;
  accel.y=K[1]*WP_Sensor.accel_raw.y-B[1]*One_G_TO_Accel;
  accel.z=K[2]*WP_Sensor.accel_raw.z-B[2]*One_G_TO_Accel;
  
  mag.x=WP_Sensor.mag_raw.x-mag_offset.x;
  mag.y=WP_Sensor.mag_raw.y-mag_offset.y;
  mag.z=WP_Sensor.mag_raw.z-mag_offset.z;
  //原始数据滤波处理 
	WP_Sensor.mag_intensity=pythagorous3(mag.x,mag.y,mag.z);	
	
//  IIR_High_Order_Filter(&Accel_X_Butter_Filter,accel.x);accel_filter.x=Accel_X_Butter_Filter.output[Accel_X_Butter_Filter.N-1];//accel_filter.x=LPButterworth(accel.x,&accel_filter_buf[0],&Accel_Parameter);
//  IIR_High_Order_Filter(&Accel_Y_Butter_Filter,accel.y);accel_filter.y=Accel_Y_Butter_Filter.output[Accel_Y_Butter_Filter.N-1];//accel_filter.y=LPButterworth(accel.y,&accel_filter_buf[1],&Accel_Parameter);
//  IIR_High_Order_Filter(&Accel_Z_Butter_Filter,accel.z);accel_filter.z=Accel_Z_Butter_Filter.output[Accel_Z_Butter_Filter.N-1];//accel_filter.z=LPButterworth(accel.z,&accel_filter_buf[2],&Accel_Parameter);		


//	IIR_High_Order_Filter(&Gyro_X_Butter_Filter,gyro.x);gyro_filter.x=Gyro_X_Butter_Filter.output[Gyro_X_Butter_Filter.N-1];//	gyro_filter.x=LPButterworth(gyro.x,&gyro_filter_buf_bug[0],&Gyro_Parameter);
//  IIR_High_Order_Filter(&Gyro_Y_Butter_Filter,gyro.y);gyro_filter.y=Gyro_Y_Butter_Filter.output[Gyro_Y_Butter_Filter.N-1];//	gyro_filter.y=LPButterworth(gyro.y,&gyro_filter_buf_bug[1],&Gyro_Parameter);
//  IIR_High_Order_Filter(&Gyro_Z_Butter_Filter,gyro.z);gyro_filter.z=Gyro_Z_Butter_Filter.output[Gyro_Z_Butter_Filter.N-1];//	gyro_filter.z=LPButterworth(gyro.z,&gyro_filter_buf_bug[2],&Gyro_Parameter);		
//	
//  IIR_High_Order_Filter(&Accel_X_INS,accel.x);ins_accel_filter.x=Accel_X_INS.output[Accel_X_INS.N-1];//ins_accel_filter.x=LPButterworth(accel.x,&ins_accel_filter_buf[0],&Ins_Accel_Parameter);
//  IIR_High_Order_Filter(&Accel_Y_INS,accel.y);ins_accel_filter.y=Accel_Y_INS.output[Accel_Y_INS.N-1];//ins_accel_filter.y=LPButterworth(accel.y,&ins_accel_filter_buf[1],&Ins_Accel_Parameter);
//  IIR_High_Order_Filter(&Accel_Z_INS,accel.z);ins_accel_filter.z=Accel_Z_INS.output[Accel_Z_INS.N-1];//ins_accel_filter.z=LPButterworth(accel.z,&ins_accel_filter_buf[2],&Ins_Accel_Parameter);
	
	  //原始数据滤波处理	
  static float last_temperature;
	last_temperature=WP_Sensor._temperature;
  WP_Sensor._temperature=last_temperature+0.1f*(WP_Sensor.temperature-last_temperature);
	
	
	accel_filter.x=LPButterworth(accel.x,&accel_filter_buf[0],&Accel_Parameter);
	accel_filter.y=LPButterworth(accel.y,&accel_filter_buf[1],&Accel_Parameter);
	accel_filter.z=LPButterworth(accel.z,&accel_filter_buf[2],&Accel_Parameter);	
		
	gyro_filter.x=LPButterworth(gyro.x,&gyro_filter_buf_bug[0],&Gyro_Parameter);
  gyro_filter.y=LPButterworth(gyro.y,&gyro_filter_buf_bug[1],&Gyro_Parameter);
  gyro_filter.z=LPButterworth(gyro.z,&gyro_filter_buf_bug[2],&Gyro_Parameter);	

//  accel_filter.x=accel.x;
//	accel_filter.y=accel.y;
//  accel_filter.z=accel.z;

//  gyro_filter.x=gyro.x;
//	gyro_filter.y=gyro.y;
//  gyro_filter.z=gyro.z;
	
	
  ins_accel_filter.x=LPButterworth(accel.x,&ins_accel_filter_buf[0],&Ins_Accel_Parameter);
  ins_accel_filter.y=LPButterworth(accel.y,&ins_accel_filter_buf[1],&Ins_Accel_Parameter);
  ins_accel_filter.z=LPButterworth(accel.z,&ins_accel_filter_buf[2],&Ins_Accel_Parameter);
	
	
  Accel_For_Cal[0]=LPButterworth(WP_Sensor.accel_raw.x,&accel_for_cal_filter_buf[0],&Calibrate_Parameter);
  Accel_For_Cal[1]=LPButterworth(WP_Sensor.accel_raw.y,&accel_for_cal_filter_buf[1],&Calibrate_Parameter);
  Accel_For_Cal[2]=LPButterworth(WP_Sensor.accel_raw.z,&accel_for_cal_filter_buf[2],&Calibrate_Parameter);	  
  WP_Sensor.acce_filter[0]=Accel_For_Cal[0];
  WP_Sensor.acce_filter[1]=Accel_For_Cal[1];
  WP_Sensor.acce_filter[2]=Accel_For_Cal[2];
}

void Observation_Angle_Calculate(void)
{
  float ACCE_X_TEMP,ACCE_Y_TEMP,ACCE_Z_TEMP;
  float _magtemp[3]={0};  
  ACCE_X_TEMP=accel.x;
  ACCE_Y_TEMP=accel.y;
  ACCE_Z_TEMP=accel.z;
  Roll_Observation=-57.3*atan(ACCE_X_TEMP*invSqrt(ACCE_Y_TEMP*ACCE_Y_TEMP+ACCE_Z_TEMP*ACCE_Z_TEMP));//横滚角
  Pitch_Observation=57.3*atan(ACCE_Y_TEMP*invSqrt(ACCE_X_TEMP*ACCE_X_TEMP+ACCE_Z_TEMP*ACCE_Z_TEMP));//俯仰角
  /************磁力计倾角补偿*****************/
	if(mag_update_flag==1)
	{
		mag_filter.x=mag.x;
		mag_filter.y=mag.y;
		mag_filter.z=mag.z;			
		_magtemp[0]=mag_filter.x;
		_magtemp[1]=mag_filter.y;
		_magtemp[2]=mag_filter.z;
		

		/***********反正切得到磁力计观测角度*********/	
		magn.y=  _magtemp[1] * Cos_Pitch- _magtemp[2] * Sin_Pitch;
		magn.x=  _magtemp[0] * Cos_Roll
						+_magtemp[1] * Sin_Roll * Sin_Pitch
					  +_magtemp[2] * Sin_Roll * Cos_Pitch;
		Yaw_Observation=atan2(magn.x,magn.y)*57.296f;	
		if(Yaw_Observation<0) Yaw_Observation=Yaw_Observation+360;
		Yaw_Observation=constrain_float(Yaw_Observation,0,360);
	}
}

float euler_roll_init,euler_pitch_init,euler_yaw_init;
float q0_init, q1_init, q2_init, q3_init;

void Euler_Angle_Init()
{
  float ACCE_X_TEMP,ACCE_Y_TEMP,ACCE_Z_TEMP;
  float _magtemp[3]={0};    
  ICM20689_Read_Data(&WP_Sensor.gyro_raw,&WP_Sensor.accel_raw,&WP_Sensor.temperature);
  accel.x=K[0]*WP_Sensor.accel_raw.x-B[0]*One_G_TO_Accel;
  accel.y=K[1]*WP_Sensor.accel_raw.y-B[1]*One_G_TO_Accel;
  accel.z=K[2]*WP_Sensor.accel_raw.z-B[2]*One_G_TO_Accel;
  ACCE_X_TEMP=accel.x;
  ACCE_Y_TEMP=accel.y;
  ACCE_Z_TEMP=accel.z;
  Roll_Observation=-57.3*atan(ACCE_X_TEMP*invSqrt(ACCE_Y_TEMP*ACCE_Y_TEMP+ACCE_Z_TEMP*ACCE_Z_TEMP));//横滚角
  Pitch_Observation=57.3*atan(ACCE_Y_TEMP*invSqrt(ACCE_X_TEMP*ACCE_X_TEMP+ACCE_Z_TEMP*ACCE_Z_TEMP));//俯仰角
  
  Sin_Pitch=sin(Pitch_Observation* DEG2RAD);
  Cos_Pitch=cos(Pitch_Observation* DEG2RAD);
  Sin_Roll=sin(Roll_Observation* DEG2RAD);
  Cos_Roll=cos(Roll_Observation* DEG2RAD);
  while(mag_update_flag==0)
  {
    delay_ms(5);
    mag_update_flag=Compass_Read_Data(&WP_Sensor.mag_raw);
  }
  /************磁力计倾角补偿*****************/
  _magtemp[0]=WP_Sensor.mag_raw.x-mag_offset.x;
  _magtemp[1]=WP_Sensor.mag_raw.y-mag_offset.y;
  _magtemp[2]=WP_Sensor.mag_raw.z-mag_offset.z;
  
  /***********反正切得到磁力计观测角度*********/
	magn.y=  _magtemp[1] * Cos_Pitch- _magtemp[2] * Sin_Pitch;
	magn.x=  _magtemp[0] * Cos_Roll
					+_magtemp[1] * Sin_Roll * Sin_Pitch
					+_magtemp[2] * Sin_Roll * Cos_Pitch;
	Yaw_Observation=atan2(magn.x,magn.y)*57.296f;	
	if(Yaw_Observation<0) Yaw_Observation=Yaw_Observation+360;
	Yaw_Observation=constrain_float(Yaw_Observation,0,360);
	yaw_angle_deg_enu=Yaw_Observation;
  euler_pitch_init=Roll_Observation*DEG2RAD; //初始化欧拉翻滚角
  euler_roll_init=Pitch_Observation*DEG2RAD; //初始化欧拉俯仰角
  euler_yaw_init  =Yaw_Observation*DEG2RAD ;
}

void Euler_Angle_Init_Again(void)
{
  float ACCE_X_TEMP,ACCE_Y_TEMP,ACCE_Z_TEMP;
  float MagTemp[3]={0};    
  accel.x=K[0]*WP_Sensor.accel_raw.x-B[0]*One_G_TO_Accel;
  accel.y=K[1]*WP_Sensor.accel_raw.y-B[1]*One_G_TO_Accel;
  accel.z=K[2]*WP_Sensor.accel_raw.z-B[2]*One_G_TO_Accel;
  ACCE_X_TEMP=accel.x;
  ACCE_Y_TEMP=accel.y;
  ACCE_Z_TEMP=accel.z;
  Roll_Observation=-57.3*atan(ACCE_X_TEMP*invSqrt(ACCE_Y_TEMP*ACCE_Y_TEMP+ACCE_Z_TEMP*ACCE_Z_TEMP));//横滚角
  Pitch_Observation=57.3*atan(ACCE_Y_TEMP*invSqrt(ACCE_X_TEMP*ACCE_X_TEMP+ACCE_Z_TEMP*ACCE_Z_TEMP));//俯仰角  
  Sin_Pitch=sin(Pitch_Observation* DEG2RAD);
  Cos_Pitch=cos(Pitch_Observation* DEG2RAD);
  Sin_Roll=sin(Roll_Observation* DEG2RAD);
  Cos_Roll=cos(Roll_Observation* DEG2RAD);
  /************磁力计倾角补偿*****************/
  MagTemp[0]=WP_Sensor.mag_raw.x-mag_offset.x;
  MagTemp[1]=WP_Sensor.mag_raw.y-mag_offset.y;
  MagTemp[2]=WP_Sensor.mag_raw.z-mag_offset.z;
  magn.x=  MagTemp[0] * Cos_Roll+ MagTemp[2] * Sin_Roll;
  magn.y=  MagTemp[0] * Sin_Pitch*Sin_Roll
					+MagTemp[1] * Cos_Pitch
					-MagTemp[2] * Sin_Pitch*Cos_Roll;
  /***********反正切得到磁力计观测角度*********/
  Yaw_Observation=atan2(magn.x,magn.y)*57.296f;
  Yaw_Observation=constrain_float(Yaw_Observation,0,360);
  yaw_angle_deg_enu=Yaw_Observation;	
  euler_pitch_init=Roll_Observation*DEG2RAD; //初始化欧拉翻滚角
  euler_roll_init=Pitch_Observation*DEG2RAD; //初始化欧拉俯仰角
  euler_yaw_init  =Yaw_Observation*DEG2RAD ;
	q0 = cos(euler_yaw_init/2)*cos(euler_pitch_init/2)*cos(euler_roll_init/2)
			+sin(euler_yaw_init/2)*sin(euler_pitch_init/2)*sin(euler_roll_init/2);
	q1 = cos(euler_yaw_init/2)*cos(euler_pitch_init/2)*sin(euler_roll_init/2)
			-sin(euler_yaw_init/2)*sin(euler_pitch_init/2)*cos(euler_roll_init/2);
	q2 = cos(euler_yaw_init/2)*sin(euler_pitch_init/2)*cos(euler_roll_init/2)
			+sin(euler_yaw_init/2)*cos(euler_pitch_init/2)*sin(euler_roll_init/2);
	q3 = sin(euler_yaw_init/2)*cos(euler_pitch_init/2)*cos(euler_roll_init/2)
			-cos(euler_yaw_init/2)*sin(euler_pitch_init/2)*sin(euler_roll_init/2);
			
	for(int16_t i=9;i>0;i--)
	{ 
	  qr[0][i]=q0;
		qr[1][i]=q1;
		qr[2][i]=q2;
		qr[3][i]=q3;
	}
		qr[0][0]=q0;
		qr[1][0]=q1;
		qr[2][0]=q2;
		qr[3][0]=q3;
}


/****************** 根据初始化欧拉角初始化四元数 *****************************/
void Quaternion_Init()
{
  Euler_Angle_Init();
  q0_init = cos(euler_yaw_init/2)*cos(euler_pitch_init/2)*cos(euler_roll_init/2)
    +sin(euler_yaw_init/2)*sin(euler_pitch_init/2)*sin(euler_roll_init/2);
  q1_init = cos(euler_yaw_init/2)*cos(euler_pitch_init/2)*sin(euler_roll_init/2)
    -sin(euler_yaw_init/2)*sin(euler_pitch_init/2)*cos(euler_roll_init/2);
  q2_init = cos(euler_yaw_init/2)*sin(euler_pitch_init/2)*cos(euler_roll_init/2)
    +sin(euler_yaw_init/2)*cos(euler_pitch_init/2)*sin(euler_roll_init/2);
  q3_init = sin(euler_yaw_init/2)*cos(euler_pitch_init/2)*cos(euler_roll_init/2)
    -cos(euler_yaw_init/2)*sin(euler_pitch_init/2)*sin(euler_roll_init/2);
}

void WP_Quad_Init(void)//初始四元数初始化
{
  Quaternion_Init();
  q0=q0_init;
  q1=q1_init;
  q2=q2_init;
  q3=q3_init;
	
	for(int16_t i=9;i>0;i--)
	{ 
	  qr[0][i]=q0;
		qr[1][i]=q1;
		qr[2][i]=q2;
		qr[3][i]=q3;
	}
		qr[0][0]=q0;
		qr[1][0]=q1;
		qr[2][0]=q2;
		qr[3][0]=q3;
}
Testime Imu_Delta;
float IMU_Dt=0.0f;
float Yaw_Gyro_Earth_Frame=0;
#define sampleFreq 200
#define AHRS_SYNC_CNT 0
uint8_t ahrs_sync_flag=0;
float q0=1.0f,q1=0,q2=0,q3=0;
volatile float beta=0.01f;//0.0175
float yaw_fus_beta=0.025f;//0.025f
float accel_mode=0;
float gx_delta=0,gy_delta=0,gz_delta=0;
float kp=0;
void Madgwick_AHRS_Update_IMU(float gx, float gy, float gz, 
                              float ax, float ay, float az,
                              float mx, float my, float mz,
                              float gyro_mold) 
{
  float recipNorm;
  float s0, s1, s2, s3;
  float qDot1, qDot2, qDot3, qDot4;
  float _2q0, _2q1, _2q2, _2q3, _4q0, _4q1, _4q2 ,_8q1, _8q2;
  float q0q0, q0q1, q0q2, q0q3, q1q1, q1q2, q1q3, q2q2, q2q3, q3q3;  
  float hx, hy, bx, bz;
  float halfwx, halfwy, halfwz;
  float halfex, halfey, halfez;
  float beta_temp=0;
	static uint16_t ahrs_sync_cnt=0;
  Test_Period(&Imu_Delta);
  IMU_Dt=(float)(Imu_Delta.Time_Delta/1000.0f);
  if(IMU_Dt>2*WP_Duty_Dt||IMU_Dt<WP_Duty_Dt||isnan(IMU_Dt)!=0)   IMU_Dt=WP_Duty_Dt;
	
  gx*=GYRO_CALIBRATION_COFF;
  gy*=GYRO_CALIBRATION_COFF;
  gz*=GYRO_CALIBRATION_COFF;
  //{-sinθ          cosθsin Φ                          cosθcosΦ                   }
  Yaw_Gyro_Earth_Frame=-Sin_Roll*gx+ Cos_Roll*Sin_Pitch *gy+ Cos_Pitch * Cos_Roll *gz;
  
  gx+=gx_delta;
  gy+=gy_delta;
  gz+=gz_delta;
  
  // Rate of change of quaternion from gyroscope
  qDot1 = 0.5f * (-q1 * gx - q2 * gy - q3 * gz)*DEG2RAD;
  qDot2 = 0.5f * ( q0 * gx + q2 * gz - q3 * gy)*DEG2RAD;
  qDot3 = 0.5f * ( q0 * gy - q1 * gz + q3 * gx)*DEG2RAD;
  qDot4 = 0.5f * ( q0 * gz + q1 * gy - q2 * gx)*DEG2RAD;
  
	ahrs_sync_cnt++;
	if(ahrs_sync_cnt>=2)//10ms
	{
		for(int16_t i=9;i>0;i--)
		{ 
			qr[0][i]=qr[0][i-1];
			qr[1][i]=qr[1][i-1];
			qr[2][i]=qr[2][i-1];
			qr[3][i]=qr[3][i-1];
		}
		ahrs_sync_cnt=0;
		ahrs_sync_flag=1;
	}
		qr[0][0]=q0;
		qr[1][0]=q1;
		qr[2][0]=q2;
		qr[3][0]=q3;
		
  // Compute feedback only if accelerometer measurement valid (avoids NaN in accelerometer normalisation)
  if(!((ax == 0.0f) && (ay == 0.0f) && (az == 0.0f))&&ahrs_sync_flag==1) 
  {
		accel_mode=GRAVITY_MSS*((safe_sqrt(sq(ax)+sq(ay)+sq(az))/AcceMax_1G)-1.0f);
    ahrs_sync_flag=0;		
    // Normalise accelerometer measurement
    recipNorm = invSqrt(ax * ax + ay * ay + az * az);
    ax *= recipNorm;
    ay *= recipNorm;
    az *= recipNorm;     
    // Normalise magnetometer measurement
    recipNorm = invSqrt(mx * mx + my * my + mz * mz);
    mx *= recipNorm;
    my *= recipNorm;
    mz *= recipNorm;		
    
    // Auxiliary variables to avoid repeated arithmetic  
		_2q0=2.0f *qr[0][AHRS_SYNC_CNT];//_2q0 = 2.0f * q0;
		_2q1=2.0f *qr[1][AHRS_SYNC_CNT];//_2q1 = 2.0f * q1;
		_2q2=2.0f *qr[2][AHRS_SYNC_CNT];//_2q2 = 2.0f * q2;
		_2q3=2.0f *qr[3][AHRS_SYNC_CNT];//_2q3 = 2.0f * q3;	
		_4q0=4.0f *qr[0][AHRS_SYNC_CNT];//_4q0 = 4.0f * q0;
		_4q1=4.0f *qr[1][AHRS_SYNC_CNT];//_4q1 = 4.0f * q1;
		_4q2=4.0f *qr[2][AHRS_SYNC_CNT];//_4q2 = 4.0f * q2;
		_8q1=8.0f *qr[1][AHRS_SYNC_CNT];//_8q1 = 8.0f * q1;
		_8q2=8.0f *qr[2][AHRS_SYNC_CNT];//_8q2 = 8.0f * q2;
		q0q0 = qr[0][AHRS_SYNC_CNT] * qr[0][AHRS_SYNC_CNT];//		q0q0 = q0 * q0;
    q0q1 = qr[0][AHRS_SYNC_CNT] * qr[1][AHRS_SYNC_CNT];//    q0q1 = q0 * q1;
    q0q2 = qr[0][AHRS_SYNC_CNT] * qr[2][AHRS_SYNC_CNT];//    q0q2 = q0 * q2;
    q0q3 = qr[0][AHRS_SYNC_CNT] * qr[3][AHRS_SYNC_CNT];//    q0q3 = q0 * q3;
    q1q1 = qr[1][AHRS_SYNC_CNT] * qr[1][AHRS_SYNC_CNT];//    q1q1 = q1 * q1;
    q1q2 = qr[1][AHRS_SYNC_CNT] * qr[2][AHRS_SYNC_CNT];//    q1q2 = q1 * q2;
    q1q3 = qr[1][AHRS_SYNC_CNT] * qr[3][AHRS_SYNC_CNT];//    q1q3 = q1 * q3;
    q2q2 = qr[2][AHRS_SYNC_CNT] * qr[2][AHRS_SYNC_CNT];//    q2q2 = q2 * q2;
    q2q3 = qr[2][AHRS_SYNC_CNT] * qr[3][AHRS_SYNC_CNT];//    q2q3 = q2 * q3;
    q3q3 = qr[3][AHRS_SYNC_CNT] * qr[3][AHRS_SYNC_CNT];//    q3q3 = q3 * q3;		  
    
    // Reference direction of Earth's magnetic field
    hx = 2.0f * (mx * (0.5f - q2q2 - q3q3) + my * (q1q2 - q0q3) + mz * (q1q3 + q0q2));
    hy = 2.0f * (mx * (q1q2 + q0q3) + my * (0.5f - q1q1 - q3q3) + mz * (q2q3 - q0q1));
    bx = sqrt(hx * hx + hy * hy);
    bz = 2.0f * (mx * (q1q3 - q0q2) + my * (q2q3 + q0q1) + mz * (0.5f - q1q1 - q2q2));
    
    halfwx = bx * (0.5f - q2q2 - q3q3) + bz * (q1q3 - q0q2);
    halfwy = bx * (q1q2 - q0q3) + bz * (q0q1 + q2q3);
    halfwz = bx * (q0q2 + q1q3) + bz * (0.5f - q1q1 - q2q2); 
    // Error is sum of cross product between estimated direction and measured direction of field vectors
    halfex = (my * halfwz - mz * halfwy);
    halfey = (mz * halfwx - mx * halfwz);
    halfez = (mx * halfwy - my * halfwx);
    
    gx_delta=kp*halfex;
    gy_delta=kp*halfey;
    gz_delta=kp*halfez;
    
    // Gradient decent algorithm corrective step
    s0 = _4q0 * q2q2 + _2q2 * ax + _4q0 * q1q1 - _2q1 * ay;
    s1 = _4q1 * q3q3 - _2q3 * ax + 4.0f * q0q0 * qr[1][AHRS_SYNC_CNT] - _2q0 * ay - _4q1 + _8q1 * q1q1 + _8q1 * q2q2 + _4q1 * az;
    s2 = 4.0f * q0q0 * qr[2][AHRS_SYNC_CNT] + _2q0 * ax + _4q2 * q3q3 - _2q3 * ay - _4q2 + _8q2 * q1q1 + _8q2 * q2q2 + _4q2 * az;
    s3 = 4.0f * q1q1 * qr[3][AHRS_SYNC_CNT] - _2q1 * ax + 4.0f * q2q2 * qr[3][AHRS_SYNC_CNT] - _2q2 * ay;
    
		recipNorm = invSqrt(s0 * s0 + s1 * s1 + s2 * s2 + s3 * s3); // normalise step magnitude
    s0 *= recipNorm;
    s1 *= recipNorm;
    s2 *= recipNorm;
    s3 *= recipNorm;
    
    beta_temp=beta+0.01f*IMU_Dt*constrain_float(gyro_mold,0,500);//0.0175f  0.025 
		beta_temp-=0.0005f*(accel_mode/GRAVITY_MSS);//0.005
    beta_temp=constrain_float(beta_temp,beta,0.06f);
    
    // Apply feedback step
    qDot1 -= beta_temp * s0;
    qDot2 -= beta_temp * s1;
    qDot3 -= beta_temp * s2;
    qDot4 -= beta_temp * s3;
  }
  
  // Integrate rate of change of quaternion to yield quaternion
  q0 += qDot1 * IMU_Dt;
  q1 += qDot2 * IMU_Dt;
  q2 += qDot3 * IMU_Dt;
  q3 += qDot4 * IMU_Dt;
  
  // Normalise quaternion
  recipNorm = invSqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
  q0 *= recipNorm;
  q1 *= recipNorm;
  q2 *= recipNorm;
  q3 *= recipNorm;
}


Testime yaw_fus_time;
void Get_Status_Feedback(void)
{
	float yaw_obs_err=0;
	float yaw_fus_dt=0;
  Sensor_Update();//传感器数据更新
	Observation_Angle_Calculate();//观测量计算
  Madgwick_AHRS_Update_IMU(gyro_filter.x,gyro_filter.y,gyro_filter.z
                           ,accel_filter.x,accel_filter.y,accel_filter.z
                             ,mag_filter.x,mag_filter.y,mag_filter.z
                               ,Gyro_Length);	
  Pitch= atan2(2.0f * q2 * q3 + 2.0f * q0 * q1, -2.0f *q1 *q1 - 2.0f * q2* q2 + 1.0f) * RAD2DEG;		// Pitch
  Roll = asin(2.0f * q0* q2-2.0f * q1 * q3) * RAD2DEG;																								// Roll
 
#if (YAW_FUSION_MODE_QUAD)
	Yaw = atan2(2.0f * q1 * q2 + 2.0f * q0 * q3, -2.0f * q3 *q3 - 2.0f * q2 * q2 + 1.0f) * RAD2DEG;		// Yaw
#else
	Test_Period(&yaw_fus_time);
	yaw_fus_dt=(float)(yaw_fus_time.Time_Delta/1000.0f);
  if(yaw_fus_dt>2*WP_Duty_Dt||yaw_fus_dt<WP_Duty_Dt||isnan(yaw_fus_dt)!=0)   yaw_fus_dt=WP_Duty_Dt;
	
	if(Sensor_Flag.Mag_Health==TRUE)
	{		
		//偏航角一阶互补融合
		yaw_angle_deg_enu+=Yaw_Gyro_Earth_Frame*yaw_fus_dt;
		if(yaw_angle_deg_enu<0)   yaw_angle_deg_enu+=360;
		if(yaw_angle_deg_enu>360) yaw_angle_deg_enu-=360;
		
		yaw_obs_err=Yaw_Observation-yaw_angle_deg_enu;
		if(Yaw_Observation>270&&yaw_angle_deg_enu<90)      
			yaw_obs_err=360-yaw_obs_err;
		else if(Yaw_Observation<90&&yaw_angle_deg_enu>270) 
			yaw_obs_err=360+yaw_obs_err;
		
		if(mag_update_flag==1)
		{
			mag_update_flag=0;			
			yaw_angle_deg_enu +=yaw_obs_err* yaw_fus_beta;
			//偏航角数据量化到0~360
	  }
		if(yaw_angle_deg_enu<0)   Yaw=yaw_angle_deg_enu+360;
		else Yaw=yaw_angle_deg_enu;
		if(GPS_Home_Set==1)  Yaw=Yaw-Declination;//如果GPS home点已设置，获取当地磁偏角，得到地理真北
	}
	else
	{
		Yaw = atan2(2.0f * q1 * q2 + 2.0f * q0 * q3, -2.0f * q3 *q3 - 2.0f * q2 * q2 + 1.0f) * RAD2DEG;		// Yaw
	}
	
  if(Yaw<0.0f)   Yaw+=360.0f;
  if(Yaw>360.0f) Yaw-=360.0f;
	
#endif
		
  //计算三轴角速度，用于姿态内环控制
  Pitch_Gyro=gyro_filter.x*GYRO_CALIBRATION_COFF;
  Roll_Gyro =gyro_filter.y*GYRO_CALIBRATION_COFF;
  Yaw_Gyro  =gyro_filter.z*GYRO_CALIBRATION_COFF;
  Gyro_Length=sqrt(Yaw_Gyro*Yaw_Gyro+Pitch_Gyro*Pitch_Gyro+Roll_Gyro*Roll_Gyro);//单位deg/s
  imuComputeRotationMatrix();
  
  Circle_Angle.Pit+=Pitch_Gyro*IMU_Dt;
  Circle_Angle.Rol+=Roll_Gyro*IMU_Dt;
  Circle_Angle.Yaw+=Yaw_Gyro*IMU_Dt;
  if(Circle_Angle.Pit<0.0f)   Circle_Angle.Pit+=360.0f;
  if(Circle_Angle.Pit>360.0f) Circle_Angle.Pit-=360.0f;
  if(Circle_Angle.Rol<0.0f)   Circle_Angle.Rol+=360.0f;
  if(Circle_Angle.Rol>360.0f) Circle_Angle.Rol-=360.0f;
  if(Circle_Angle.Yaw<0.0f)   Circle_Angle.Yaw+=360.0f;
  if(Circle_Angle.Yaw>360.0f) Circle_Angle.Yaw-=360.0f;
  //更新位置、速度、加速度
  SINS_Prepare();//惯导加速度更新
  Strapdown_INS_High_Kalman();//高度方向卡尔曼滤波估计竖直速度、位置
}


  
//			if((Yaw_Observation>90 && yaw_angle_deg_enu<-90)|| (Yaw_Observation<-90 && yaw_angle_deg_enu>90))
//					 yaw_angle_deg_enu = -yaw_angle_deg_enu * (1-yaw_fus_beta) + Yaw_Observation * yaw_fus_beta;//0.02
//			else yaw_angle_deg_enu =  yaw_angle_deg_enu * (1-yaw_fus_beta) + Yaw_Observation * yaw_fus_beta;
