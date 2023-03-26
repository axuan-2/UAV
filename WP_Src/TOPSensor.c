#include "Headfile.h"
#include "TOPSensor.h"

#define GROUND_RANGE_USE_I2C 0

enum TOFSENSE_TYPE
{
	TOFSENSE_UART=0,//����0.01-5m
	TOFSENSE_P,     //����0.03-8m
  TOFSENSE_F,     //����0.05-15m
  TOFSENSE_FP,    //����0.05-25m
	TOFSENSE_NUM_MAX
};

#define GROUND_RANGE_TOFSENSE  TOFSENSE_UART //�Եز�ഫ�������� 
#define TOFSENSE_DATA_EFFECTIVE_RATE 0.85f   //������Ч�� 
uint16_t TOFSENSR_MAX_RANGE[TOFSENSE_NUM_MAX]={500,800,1500,2500}; 




#define FRAME_HEADER 					0x57
#define Frame_MARK_READ_FRAME 0x10

void Check_Front_Tofsense(void);



void NLink_Data_Send(uint8_t *buf, uint32_t cnt)  
{
	USART7_Send(buf,cnt);//�û���ֲʱ����д�˴��ڷ��ͺ���
}


void  NLink_TOFSense_Read_Frame(uint8_t id)
{
	uint8_t buf[8];
	uint8_t check_sum=0;
	buf[0]=FRAME_HEADER;
	buf[1]=Frame_MARK_READ_FRAME;
	buf[2]=0xff;
	buf[3]=0xff;
	buf[4]=id;
	buf[5]=0xff;
	buf[6]=0xff;
	check_sum=buf[0]+buf[1]+buf[2]+buf[3]+buf[4]+buf[5]+buf[6];
	buf[7]=check_sum;//0x63;
	NLink_Data_Send(buf,8);
}


void TOFSense_Data_Receive_Anl(uint8_t *data_buf,uint8_t num)
{
  uint8_t sum = 0;
	uint8_t id=0;
  for(uint8_t i=0;i<(num-1);i++)  sum+=*(data_buf+i);
  if(!(sum==*(data_buf+num-1))) 	return;//�������У������
  if(!(*(data_buf)==0x57 && *(data_buf+1)==0x00))return;//������֡ͷ����
	id=data_buf[3];
	
	tofdata[id].pre_last_distance=tofdata[id].last_distance;//���ϴθ߶�
	tofdata[id].last_distance=tofdata[id].distance;//�ϴθ߶�
	tofdata[id].last_vel=tofdata[id].vel;//�ϴ��ٶ�
	tofdata[id].last_system_time=tofdata[id].system_time;
	tofdata[id].id=data_buf[3];
	tofdata[id].system_time=data_buf[4]|(data_buf[5]<<8)|(data_buf[6]<<16)|(data_buf[7]<<24);
	tofdata[id].dis=(int32_t)(data_buf[8]<< 8|data_buf[9]<<16|data_buf[10]<<24)/256;
	tofdata[id].dis_status=data_buf[11];
	tofdata[id].signal_strength=data_buf[12]|(data_buf[13]<<8);
	tofdata[id].distance=tofdata[id].dis/10.0f;//cm
	tofdata[id].vel=(tofdata[id].distance-tofdata[id].last_distance)/0.1f; //�۲��ٶ�
	tofdata[id].acc=(tofdata[id].vel-tofdata[id].last_vel)/0.1f;					  //�۲���ٶ�				
}


uint8_t tofsense_buf[20];
void TOF_Sense_Prase(uint8_t data)
{
  static uint8_t data_len = 0,data_cnt = 0;
  static uint8_t state = 0;
  if(state==0&&data==0x57)//�ж�֡ͷ1
  {
    state=1;
    tofsense_buf[0]=data;
  }
  else if(state==1&&data==0x00)//�ж�֡ͷ2
  {
    state=2;
    tofsense_buf[1]=data;
  }
  else if(state==2&&data==0xff)//�����ֽ�
  {
    state=3;
    tofsense_buf[2]=data;
  }
  else if(state==3&&data<=0xff)//id
  {
    state = 4;
    tofsense_buf[3]=data;
    data_len = 11;
    data_cnt = 0;
  }
  else if(state==4&&data_len>0)//���ݽ���
  {
    data_len--;
    tofsense_buf[4+data_cnt++]=data;
    if(data_len==0)  state = 5;
  }
  else if(state==5)//��У��
  {
    state = 0;
    tofsense_buf[4+data_cnt++]=data;
		TOFSense_Data_Receive_Anl(tofsense_buf,16);
  }
  else state = 0;
}
		
		
TOFSensor_Data tofdata[TOFSENSE_MAX_NUMBER]; 
uint16_t tof_tail,last_tof_tail,tof_offset;
systime tofsensor;
float front_target_distance=0;
float last_height,now_height,last_proc_height;
uint8_t height_flag;
void TOF_Statemachine(void)
{
	static uint16_t inquire_cnt=0;
	inquire_cnt++;
	switch(inquire_cnt)
	{
		case 2 :
		{
			NLink_TOFSense_Read_Frame(0x00);
		}
		break;
		case 4 :
		{
			NLink_TOFSense_Read_Frame(0x01);	
		}
		break;
		case 6:
		{
			NLink_TOFSense_Read_Frame(0x02);
		}
		break;
		case 8:
		{
			NLink_TOFSense_Read_Frame(0x03);
		}
		break;
		case 10:
		{
			NLink_TOFSense_Read_Frame(0x04);
		}
		break;
		case 12:
		{
			NLink_TOFSense_Read_Frame(0x05);
		}
		break;
		case 14:
		{
			NLink_TOFSense_Read_Frame(0x06);
		}
		break;
		case 16:
		{
			NLink_TOFSense_Read_Frame(0x07);
		}
		break;
		case 18:
		{
			NLink_TOFSense_Read_Frame(0x08);
		}
		break;
		case 20:
		{
			inquire_cnt=0;
			NLink_TOFSense_Read_Frame(0x09);
		}
		break;
		default:	;
	}
	
	Check_Front_Tofsense();

#if (GROUND_RANGE_USE_I2C==0) 	
  static uint16_t _cnt=0;
	_cnt++;
  if(_cnt>=20)//100ms=10hz
  {
		_cnt=0;
		GD_Distance=tofdata[0].distance*rMat[2][2];
		GD_Distance_Div=tofdata[0].vel;
		GD_Distance_Acc=tofdata[0].acc;		
		WP_Sensor.tofsensor_updtate_flag=1;	
//		if((SDK_Duty_Cnt==1)||(SDK_Duty_Cnt==9))
//		{
			if((GD_Distance-last_height<-50)&&(height_flag==0))
			{
				height_flag=1;
				now_height=NamelessQuad.Position[_YAW];
			}
			else if(GD_Distance-last_height>50)
				height_flag=0;
			last_height=GD_Distance;
//		}
			
	}			
	if((GD_Distance<=TOFSENSR_MAX_RANGE[GROUND_RANGE_TOFSENSE]*TOFSENSE_DATA_EFFECTIVE_RATE
		&&GD_Distance>0)//��þ�����������
     &&(tofdata[0].signal_strength!=0))//���ź�ǿ�Ȳ�Ϊ0
  	Sensor_Flag.Ground_Health=1;
	else  Sensor_Flag.Ground_Health=0;
#else
  static uint16_t _cnt=0;
	_cnt++;
  if(_cnt>=20)//100ms=10hz
  {
		_cnt=0;					
		GD_Distance=TOFSense_F_Data.distance*rMat[2][2];
		GD_Distance_Div=TOFSense_F_Data.vel;
		GD_Distance_Acc=TOFSense_F_Data.acc;		
		WP_Sensor.tofsensor_updtate_flag=1;	
	}			
	if((GD_Distance<=TOFSENSR_MAX_RANGE[GROUND_RANGE_TOFSENSE]*TOFSENSE_DATA_EFFECTIVE_RATE
		&&GD_Distance>0)//��þ�����������
     &&(TOFSense_F_Data.signal_strength!=0))//���ź�ǿ�Ȳ�Ϊ0
  	Sensor_Flag.Ground_Health=1;
	else  Sensor_Flag.Ground_Health=0;	
#endif
}





float front_tofsense_distance=0;
uint8_t front_tofsense_distance_valid_cnt=0;
uint8_t front_tofsense_distance_valid_flag=0;
void Check_Front_Tofsense()
{
	float _dis[5]={0},tmp=0;
	front_tofsense_distance_valid_cnt=0;
	front_tofsense_distance_valid_flag=0;
	uint8_t i=0;
	for(uint8_t j=1;j<5;j++)
	{
		if(tofdata[j].signal_strength!=0) 
		{
			front_tofsense_distance_valid_cnt++;
			_dis[i++]=tofdata[j].distance;
			front_tofsense_distance_valid_flag=1;
		}
	}
	
	//����һ�鳬�������US100
	if(us100_front.distance>0&&us100_front.distance<200)
	{
		front_tofsense_distance_valid_cnt++;
		_dis[i++]=us100_front.distance;
		front_tofsense_distance_valid_flag=1;
	}
		
	if(front_tofsense_distance_valid_cnt==0) 			front_tofsense_distance=200;//û��ɨ�赽���ˣ�Ĭ�ϸ�200cm
	else if(front_tofsense_distance_valid_cnt==1) front_tofsense_distance=_dis[0];
	else if(front_tofsense_distance_valid_cnt==2) front_tofsense_distance=fminf(_dis[0],_dis[1]);
	else if(front_tofsense_distance_valid_cnt==3) 
	{
		tmp=fminf(_dis[0],_dis[1]);
		tmp=fminf(tmp,_dis[2]);
		front_tofsense_distance=tmp;
	}
	else if(front_tofsense_distance_valid_cnt==4)  
	{
		tmp=fminf(_dis[0],_dis[1]);
		tmp=fminf(tmp,_dis[2]);
		tmp=fminf(tmp,_dis[3]);
		front_tofsense_distance=tmp;
	}
	else if(front_tofsense_distance_valid_cnt==5)  
	{
		tmp=fminf(_dis[0],_dis[1]);
		tmp=fminf(tmp,_dis[2]);
		tmp=fminf(tmp,_dis[3]);
		tmp=fminf(tmp,_dis[4]);
		front_tofsense_distance=tmp;
	}	
	
}







#define TOFSENSE_IIC_ADDRESS    0x08//7λ��ַ
#define PRODUCT_VERSION_REG  		0x00
#define HARDWAVE_VERSION_REG 		0x04
#define FIRMWAVE_VERSION_REG 		0x08

#define INTERFACE_MODE_REG   		0x0C
#define UART_BAUDRATE_REG    		0x10
#define CAN_BAUDRATE_REG     		0x14
#define FOV_REG              		0x18
#define BAND_REG             		0x1C
#define SYSTIME_REG          		0x20
#define DISTANCE_REG         		0x24
#define SIGNAL_STRENGTH_REG  		0x28
#define RANGE_PRECISION_REG  		0x2C


TOFSense_F TOFSense_F_Data;

uint8_t f_buf[20];
void TOFSense_IIC_Update(void)
{
#if (GROUND_RANGE_USE_I2C==1) 
	static uint32_t _cnt=0;
	_cnt++;
	if(_cnt%20==0)//10hz����ˢ��,���50hz
	{
		f_buf[0]=Single_ReadI2C0(TOFSENSE_IIC_ADDRESS,INTERFACE_MODE_REG+1);//id
		i2c0ReadNByte(TOFSENSE_IIC_ADDRESS,SYSTIME_REG,&f_buf[1],13);

		
		TOFSense_F_Data.pre_last_distance=TOFSense_F_Data.last_distance;//���ϴθ߶�
		TOFSense_F_Data.last_distance=TOFSense_F_Data.distance;//�ϴθ߶�
		TOFSense_F_Data.last_vel=TOFSense_F_Data.vel;//�ϴ��ٶ�
		TOFSense_F_Data.last_system_time=TOFSense_F_Data.system_time;
		//���ݸ���
		TOFSense_F_Data.id=f_buf[0];
		TOFSense_F_Data.system_time=f_buf[1]|(f_buf[2]<<8)|(f_buf[3]<<16)|(f_buf[4]<<24);
		TOFSense_F_Data.dis=f_buf[5]|(f_buf[6]<<8)|(f_buf[7]<<16)|(f_buf[8]<<24);
		TOFSense_F_Data.dis_status=f_buf[9];
		TOFSense_F_Data.signal_strength=f_buf[11]|(f_buf[12]<<8);
		TOFSense_F_Data.range_precision=f_buf[13];
		
		TOFSense_F_Data.distance=TOFSense_F_Data.dis/10.0f;//cm
		TOFSense_F_Data.vel=(TOFSense_F_Data.distance-TOFSense_F_Data.last_distance)/0.05f; //�۲��ٶ�
		TOFSense_F_Data.acc=(TOFSense_F_Data.vel-TOFSense_F_Data.last_vel)/0.05f;					  //�۲���ٶ�		
	}
#endif
}
	

























//						uint8_t id=0;
//						
//						id=COM7_Rx_Buf.Ring_Buff[i+3];
//						
//						tofdata[id].pre_last_distance=tofdata[id].last_distance;//���ϴθ߶�
//						tofdata[id].last_distance=tofdata[id].distance;//�ϴθ߶�
//						tofdata[id].last_vel=tofdata[id].vel;//�ϴ��ٶ�
//						tofdata[id].last_system_time=tofdata[id].system_time;
//						
//						
//						tofdata[id].id=COM7_Rx_Buf.Ring_Buff[i+3];
//						tofdata[id].system_time=COM7_Rx_Buf.Ring_Buff[i+4]|(COM7_Rx_Buf.Ring_Buff[i+5]<<8)|(COM7_Rx_Buf.Ring_Buff[i+6]<<16)|(COM7_Rx_Buf.Ring_Buff[i+7]<<24);
//					
//  					//if(tofdata.last_system_time==tofdata.system_time)//����δ���� 
//			
//						Get_Systime(&tofsensor);
//						
//						tofdata[id].dis=(int32_t)(COM7_Rx_Buf.Ring_Buff[8]<< 8|COM7_Rx_Buf.Ring_Buff[9]<<16|COM7_Rx_Buf.Ring_Buff[10]<<24)/256;
//						tofdata[id].dis_status=COM7_Rx_Buf.Ring_Buff[i+11];
//						tofdata[id].signal_strength=COM7_Rx_Buf.Ring_Buff[i+12]|(COM7_Rx_Buf.Ring_Buff[i+13]<<8);

//				
//						tofdata[id].distance=tofdata[id].dis/10.0f;//cm
//						tofdata[id].vel=(tofdata[id].distance-tofdata[id].last_distance)/0.1f; //�۲��ٶ�
//						tofdata[id].acc=(tofdata[id].vel-tofdata[id].last_vel)/0.1f;					  //�۲���ٶ�

