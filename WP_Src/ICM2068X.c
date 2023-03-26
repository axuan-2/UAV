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
#include "myiic.h"
#include "ICM2068X.h"
Butter_Parameter Ins_Accel_Parameter;
Butter_Parameter Accel_Parameter;
Butter_Parameter Gyro_Parameter;
Butter_Parameter Calibrate_Parameter;
uint8_t IMU_ID=0x68; 


uint8_t IMU_Config_Write[6]={
	0x00,//PWR_MGMT_1
	0x00,//SMPLRT_DIV	
	0x02,//MPU_CONFIG
	0x08,//GYRO_CONFIG
	0x08,//ACCEL_CONFIG
	0x03//ACCEL_CONFIG2
};
uint8_t IMU_Config_Read[6];
/***********************************************************
@函数名：ICM20689_Init
@入口参数：无
@出口参数：无
功能描述：MPU6050初始化
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
uint8_t ICM20689_Init(void)//ICM20689初始化
{
	uint8_t fault=0;
	IMU_ID=Single_ReadI2C(ICM20689_ADRESS,WHO_AM_I);
	if(IMU_ID==IMU_ICM20689_ID)//ICM20689
	{	
		i2cWrite(ICM20689_ADRESS,PWR_MGMT_1  , IMU_Config_Write[0]);//关闭所有中断,解除休眠
		i2cWrite(ICM20689_ADRESS,SMPLRT_DIV  , IMU_Config_Write[1]); // sample rate.  Fsample= 1Khz/(<this value>+1) = 1000Hz					
		i2cWrite(ICM20689_ADRESS,MPU_CONFIG  , IMU_Config_Write[2]);//0x00设置陀螺仪、温度内部低通滤波频率范围，陀螺仪250hz，噪声带宽306.6hz，温度4000hz
																												  //0x01设置陀螺仪、温度内部低通滤波频率范围，陀螺仪176hz，噪声带宽177hz，温度188hz
																												  //0x02设置陀螺仪、温度内部低通滤波频率范围，陀螺仪92hz，噪声带宽108.6hz，温度98hz
																												  //0x03设置陀螺仪、温度内部低通滤波频率范围，陀螺仪41hz，噪声带宽59hz，温度42hz		
																												  //0x04设置陀螺仪、温度内部低通滤波频率范围，陀螺仪20hz，噪声带宽30.5hz，温度20hz
																									 //0x05设置陀螺仪、温度内部低通滤波频率范围，陀螺仪10hz，噪声带宽15.6hz，温度10hz
		i2cWrite(ICM20689_ADRESS,GYRO_CONFIG , IMU_Config_Write[3]);//设置陀螺仪量程，500deg/s
		i2cWrite(ICM20689_ADRESS,ACCEL_CONFIG, IMU_Config_Write[4]);// Accel scale 4g (8192 LSB/g)	
		i2cWrite(ICM20689_ADRESS,ACCEL_CONFIG2,IMU_Config_Write[5]);
																									 //0x00设置加速度计内部低通滤波频率范围，加速度218.1hz，噪声带宽235hz		
																									 //0x01设置加速度计内部低通滤波频率范围，加速度218.1hz，噪声带宽235hz
																									 //0x02设置加速度计内部低通滤波频率范围，加速度99.0hz，噪声带宽121.3hz		
																									 //0x03设置加速度计内部低通滤波频率范围，加速度44.8hz，噪声带宽61.5hz
																									 //0x04设置加速度计内部低通滤波频率范围，加速度21.2hz，噪声带宽31.0hz
																									 //0x05设置加速度计内部低通滤波频率范围，加速度10.2hz，噪声带宽15.5hz	
		//Single_WriteI2C(ICM20689_ADRESS,ACCEL_CONFIG2,0xC3);//设置加速度计内部低通滤波频率范围，加速度1046.0hz，噪声带宽1100.0hz
		delay_ms(100);		
		IMU_Config_Read[0]=i2cRead(ICM20689_ADRESS,PWR_MGMT_1);
		IMU_Config_Read[1]=i2cRead(ICM20689_ADRESS,SMPLRT_DIV);
		IMU_Config_Read[2]=i2cRead(ICM20689_ADRESS,MPU_CONFIG);
		IMU_Config_Read[3]=i2cRead(ICM20689_ADRESS,GYRO_CONFIG);
		IMU_Config_Read[4]=i2cRead(ICM20689_ADRESS,ACCEL_CONFIG);
		IMU_Config_Read[5]=i2cRead(ICM20689_ADRESS,ACCEL_CONFIG2);
		if(IMU_Config_Read[0]!=IMU_Config_Write[0]) fault=1;
		if(IMU_Config_Read[1]!=IMU_Config_Write[1]) fault=1;
		if(IMU_Config_Read[2]!=IMU_Config_Write[2]) fault=1;
		if(IMU_Config_Read[3]!=IMU_Config_Write[3]) fault=1;
		if(IMU_Config_Read[4]!=IMU_Config_Write[4]) fault=1;
		if(IMU_Config_Read[5]!=IMU_Config_Write[5]) fault=1;
	}
  delay_ms(500);	
  IMU_Calibration();
  Set_Cutoff_Frequency(Imu_Sampling_Freq, 95,&Gyro_Parameter);//姿态角速度反馈滤波参数  60 50
  Set_Cutoff_Frequency(Imu_Sampling_Freq, 30,&Accel_Parameter);//姿态解算加计修正滤波值 20 30
  Set_Cutoff_Frequency(Imu_Sampling_Freq, 2 ,&Calibrate_Parameter);//传感器校准加计滤波值
  Set_Cutoff_Frequency(Imu_Sampling_Freq, 60,&Ins_Accel_Parameter);//INS加计滤波值
	
	return fault;
}

/***********************************************************
@函数名：ICM20689_Read_Data
@入口参数：vector3f *gyro,vector3f *accel
@出口参数：无
功能描述：MPU6050数据采集
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void ICM20689_Read_Data(vector3f *gyro,vector3f *accel,float *temperature)//读取MPU6050数据
{
	uint8_t buf[14];
	int16_t temp;
	i2cReadData(ICM20689_ADRESS,ACCEL_XOUT_H,buf,14);
	accel->x=(int16_t)((buf[0]<<8)|buf[1]);
	accel->y=-(int16_t)((buf[2]<<8)|buf[3]);
	accel->z=-(int16_t)((buf[4]<<8)|buf[5]);	
	temp		=(int16_t)((buf[6]<<8)|buf[7]);
	gyro->x	=(int16_t)((buf[8]<<8)|buf[9]);
	gyro->y	=-(int16_t)((buf[10]<<8)|buf[11]);
	gyro->z	=-(int16_t)((buf[12]<<8)|buf[13]);	
	if(IMU_ID==IMU_ICM20689_ID)
	{
		//ICM20689
		//Room Temperature Offset=25 °C
		//Sensitivity=326.8 LSB/°C
		//TEMP_degC = ((TEMP_OUT –RoomTemp_Offset)/Temp_Sensitivity) +25degC
		*temperature=25.0f+(double)((temp-25.0f)/326.8f);
	}	
}


Vector3f gyro_offset;
s32 g_Gyro_xoffset = 0, g_Gyro_yoffset = 0, g_Gyro_zoffset = 0;
/***********************************************************
@函数名：IMU_Calibration
@入口参数：无
@出口参数：无
功能描述：陀螺仪开机零偏标定
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void IMU_Calibration(void)
{
  u8 i;
	vector3f gyro_offset_temp;
	ReadFlashParameterOne(GYRO_X_OFFSET,&gyro_offset_temp.x);
	ReadFlashParameterOne(GYRO_Y_OFFSET,&gyro_offset_temp.y);
	ReadFlashParameterOne(GYRO_Z_OFFSET,&gyro_offset_temp.z);	
	if(isnan(gyro_offset_temp.x)==0
		&&isnan(gyro_offset_temp.y)==0
		 &&isnan(gyro_offset_temp.z)==0)//如果之前已经温度校准过，开机时直接用之前校准的数据 
	{
		 gyro_offset.x=gyro_offset_temp.x;
		 gyro_offset.y=gyro_offset_temp.y;
		 gyro_offset.z=gyro_offset_temp.z;
	}
	else
	{
		delay_ms(500);
		for (i = 0; i < 100; i++)			//连续采样30次，一共耗时30*3=90ms
		{
			g_Gyro_xoffset +=Double_ReadI2C(ICM20689_ADRESS,GYRO_XOUT_H);
			g_Gyro_yoffset +=Double_ReadI2C(ICM20689_ADRESS,GYRO_YOUT_H);
			g_Gyro_zoffset +=Double_ReadI2C(ICM20689_ADRESS,GYRO_ZOUT_H);
			delay_ms(10);
		}
		gyro_offset.x =(g_Gyro_xoffset/100);//得到标定偏移
		gyro_offset.y =(g_Gyro_yoffset/100);
		gyro_offset.z =(g_Gyro_zoffset/100);
	}
}

