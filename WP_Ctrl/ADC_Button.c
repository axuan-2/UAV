#include "Headfile.h"
#include "ADC_Button.h"

#define  ADC_Button_Control_Enable  0//使能按键控制时设置为1，反之设置为0


#define ADC_LONG_PRESS_LIMIT  1500//1500ms
#define ADC_IN_PRESS_LIMIT  	500//250ms


#define OLD_BEFORE_20210701 0
#define NEW_AFTER_20210701 1
#define ADC_KEY_VERSION NEW_AFTER_20210701//ADC按键版本



button_state ADC_Button1,ADC_Button2,ADC_Button3,ADC_Button4;
uint16_t ADC_PPM_Databuf[10]={0};

void ADC_Button_Init()
{
	ADC_Button1.port=GPIO_PORTD_BASE;
	ADC_Button1.pin=GPIO_PIN_3;
	ADC_Button1.value=1;
	ADC_Button1.last_value=1;

	ADC_Button2.port=GPIO_PORTD_BASE;
	ADC_Button2.pin=GPIO_PIN_3;
	ADC_Button2.value=1;
	ADC_Button2.last_value=1;

	ADC_Button3.port=GPIO_PORTD_BASE;
	ADC_Button3.pin=GPIO_PIN_3;
	ADC_Button3.value=1;
	ADC_Button3.last_value=1;

	ADC_Button4.port=GPIO_PORTD_BASE;
	ADC_Button4.pin=GPIO_PIN_3;
	ADC_Button4.value=1;
	ADC_Button4.last_value=1;
}




uint8_t Key_Value=0;
void ADC_Button_Read(void)
{		
	Button_ADCResult=adc_value[1]*3.3f/4095.f;
	
#if (ADC_KEY_VERSION==OLD_BEFORE_20210701)
	if(Button_ADCResult>=3.1f)  		 Key_Value=0x00;//没有按键按下，上拉10K，输入3.3V
	else if(Button_ADCResult>=2.9f)  Key_Value=0x01;//按键K1按下，下接100K， 输入3.3V/11=3.0V
	else if(Button_ADCResult>=1.5f)  Key_Value=0x02;//按键K3按下，下接10K，  输入3.3V/2=1.65V
	else if(Button_ADCResult>=1.0f)  Key_Value=0x03;//按键K2按下，下接5K，   输入3.3V/3=1.10V
	else if(Button_ADCResult>=0.5f)  Key_Value=0x04;//按键K4按下，下接2.2K， 输入3.3V/3=0.595V
	else Key_Value=0x00;
#endif

#if (ADC_KEY_VERSION==NEW_AFTER_20210701)
	if(Button_ADCResult<=0.5f)  		  Key_Value=0x00;//没有按键按下，下拉5.1K，输入0V
	else if(Button_ADCResult<=1.2f)   Key_Value=0x04;//按键K4按下，	 上接20K， 输入5.0V*5.1/25.1=1.016V
	else if(Button_ADCResult<=1.85f)  Key_Value=0x03;//按键K2按下，  上接10K， 输入5.0V*5.1/15.1=1.64V
	else if(Button_ADCResult<=2.65f)  Key_Value=0x02;//按键K3按下，  上接5.1K，输入5.0V*5.1/10.2=2.50V
	else if(Button_ADCResult<=3.25f)  Key_Value=0x01;//按键K1按下，  上接3.3K，输入5.0V*5.1/8.4=3.04V
	else Key_Value=0x00;
#endif
	
	
  switch(Key_Value)
	{
		case 0://没有按键按下
		{
			ADC_Button1.value=1;
			ADC_Button2.value=1;
			ADC_Button3.value=1;
			ADC_Button4.value=1;
		}
		break;
		case 1://按键1按下
		{
			ADC_Button1.value=0;
			ADC_Button2.value=1;
			ADC_Button3.value=1;
			ADC_Button4.value=1;		
		}
		break;		
		case 2://按键2按下
		{
			ADC_Button1.value=1;
			ADC_Button2.value=0;
			ADC_Button3.value=1;
			ADC_Button4.value=1;		
		}
		break;		
		case 3://按键3按下
		{
			ADC_Button1.value=1;
			ADC_Button2.value=1;
			ADC_Button3.value=0;
			ADC_Button4.value=1;		
		}
		break;	
		case 4://按键4按下
		{
			ADC_Button1.value=1;
			ADC_Button2.value=1;
			ADC_Button3.value=1;
			ADC_Button4.value=0;		
		}
		break;	
	  default://没有按键按下
		{
			ADC_Button1.value=1;
			ADC_Button2.value=1;
			ADC_Button3.value=1;
			ADC_Button4.value=1;				
		}
		break;	
	}
}


void Read_ADC_Button_State_One(button_state *button)
{
  //button->value=GPIOPinRead(button->port,button->pin);
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
			if(millis()-button->in_time>ADC_IN_PRESS_LIMIT)//持续按下
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
			
			if(button->release_time-button->press_time>ADC_LONG_PRESS_LIMIT)
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


void Read_ADC_Button_State_All(void)
{
	Read_ADC_Button_State_One(&ADC_Button1);
	Read_ADC_Button_State_One(&ADC_Button2);
	Read_ADC_Button_State_One(&ADC_Button3);
	Read_ADC_Button_State_One(&ADC_Button4);
}




uint8_t adc_ppm_update_flag=0;

void ADC_Button_Scan(void)
{
	ADC_Button_Read();
	Read_ADC_Button_State_All();

#if (ADC_Button_Control_Enable==1)
	
	//以下为解锁按键事件处理逻辑
	if(ADC_Button1.press==IN_PRESS)
	{
		//以下为该按键实现的任务
		adc_ppm_update_flag=1;//控制权限给ADC按键设置的PPM
		ADC_PPM_Databuf[0]=1500;//俯仰杆位于中位
		ADC_PPM_Databuf[1]=1500;//横滚杆位于中位
		ADC_PPM_Databuf[2]=1000;//油门杆位于下侧
		ADC_PPM_Databuf[3]=2000;//偏航杆位于右侧
		
		ADC_PPM_Databuf[4]=2000;//第五通道高位，处于定高模式
		ADC_PPM_Databuf[5]=1000;//第六通道低位
		ADC_PPM_Databuf[6]=1000;//第七通道低位
		ADC_PPM_Databuf[7]=1000;//第八通道低位
		Bling_Set(&Light_3,5000,1000,0.5,0,GPIO_PORTF_BASE,GPIO_PIN_3,1);
	}
	else if(ADC_Button1.press==LONG_PRESS)//解锁按键长按
	{
		//以下为该按键实现的任务
		adc_ppm_update_flag=1;//控制权限给ADC按键设置的PPM
		if(Controler_State==Unlock_Controler)//如果处于解锁状态，遥控器油门回中准备执行SDK
		{
			ADC_PPM_Databuf[0]=1500;//俯仰杆位于中位
			ADC_PPM_Databuf[1]=1500;//横滚杆位于中位
			ADC_PPM_Databuf[2]=1000;//油门杆位于下侧
			ADC_PPM_Databuf[3]=1500;//偏航杆位于中位
			
			ADC_PPM_Databuf[4]=2000;//第五通道高位，处于定高模式
			ADC_PPM_Databuf[5]=1000;//第六通道低位
			ADC_PPM_Databuf[6]=1000;//第七通道低位
			ADC_PPM_Databuf[7]=1000;//第八通道高位
			Bling_Set(&Light_3,5000,1000,0.5,0,GPIO_PORTF_BASE,GPIO_PIN_3,1);
		}
	}
	else if(ADC_Button1.press==SHORT_PRESS)//解锁按键短按
	{
		//以下为该按键实现的任务
		adc_ppm_update_flag=0;//控制权限给释放给其它渠道获取的PPM：PPM接收机、地面站虚拟遥控器
													//故按键启动后，此时短按下按键，遥控器控制权限瞬间会释放给接收机
													//遥控器接管后，可以手动控制飞机安全降落
		
		Bling_Set(&Light_3,5000,1000,0.5,0,GPIO_PORTF_BASE,GPIO_PIN_3,0);
	}
	
	
	//以下为上锁按键事件处理逻辑
	if(ADC_Button2.press==IN_PRESS)
	{
		//以下为该按键实现的任务
	}
	else if(ADC_Button2.press==LONG_PRESS)//上锁按键长按
	{
		//以下为该按键实现的任务

	}
	else if(ADC_Button2.press==SHORT_PRESS)//上锁按键短按
	{
		//以下为该按键实现的任务
		adc_ppm_update_flag=0;//控制权限给释放给其它渠道获取的PPM：PPM接收机、地面站虚拟遥控器
													//故按键启动后，此时短按下按键，遥控器控制权限瞬间会释放给接收机
													//遥控器接管后，可以手动控制飞机安全降落
		
		if(Controler_State==Unlock_Controler)//如果处于解锁状态
		{
			Controler_State=Lock_Controler;//直接上锁
			Bling_Set(&Light_3,5000,1000,0.5,0,GPIO_PORTF_BASE,GPIO_PIN_3,0);
		}	
	}
	
	
	
	
	//以下为SDK按键事件处理逻辑
	if(ADC_Button3.press==IN_PRESS)
	{
		//以下为该按键实现的任务
	}
	else if(ADC_Button3.press==LONG_PRESS)//SDK按键长按
	{
		//以下为该按键实现的任务
		adc_ppm_update_flag=1;//控制权限给ADC按键设置的PPM
		if(Controler_State==Unlock_Controler)//如果处于解锁状态，执行SDK
		{		
			ADC_PPM_Databuf[0]=1500;//俯仰杆位于中位
			ADC_PPM_Databuf[1]=1500;//横滚杆位于中位
			ADC_PPM_Databuf[2]=1500;//油门杆位于中侧
			ADC_PPM_Databuf[3]=1500;//偏航杆位于中位

			ADC_PPM_Databuf[4]=2000;//第五通道高位，处于定高模式
			ADC_PPM_Databuf[5]=2000;//第六通道高位，处于SDK模式
			ADC_PPM_Databuf[6]=1000;//第七通道低位
			ADC_PPM_Databuf[7]=2000;//第八通道高位，处于光流定点模式
			
			Bling_Set(&Light_3,5000,1000,0.5,0,GPIO_PORTF_BASE,GPIO_PIN_3,1);
		}
	}
	else if(ADC_Button3.press==SHORT_PRESS)//SDK按键短按
	{
		//以下为该按键实现的任务
		adc_ppm_update_flag=1;//控制权限给ADC按键设置的PPM
		if(Controler_State==Unlock_Controler)//如果处于解锁状态，退出SDK模式保持原地悬停
		{
			ADC_PPM_Databuf[0]=1500;//俯仰杆位于中位
			ADC_PPM_Databuf[1]=1500;//横滚杆位于中位
			ADC_PPM_Databuf[2]=1500;//油门杆位于中侧
			ADC_PPM_Databuf[3]=1500;//偏航杆位于中位

			ADC_PPM_Databuf[4]=2000;//第五通道高位，处于定高模式
			ADC_PPM_Databuf[5]=1000;//第六通道高位，退出SDK模式
			ADC_PPM_Databuf[6]=1000;//第七通道低位
			ADC_PPM_Databuf[7]=2000;//第八通道高位，处于光流定点模式
			Bling_Set(&Light_3,5000,1000,0.5,0,GPIO_PORTF_BASE,GPIO_PIN_3,1);
		}
	}
	
	
	
	//以下为一键降落事件处理逻辑
	if(ADC_Button4.press==IN_PRESS)
	{
		//以下为该按键实现的任务
	}
	else if(ADC_Button4.press==LONG_PRESS)//一键降落按键长按
	{
		//以下为该按键实现的任务

	}
	else if(ADC_Button4.press==SHORT_PRESS)//一键降落按键短按
	{
		//以下为该按键实现的任务
		adc_ppm_update_flag=1;//控制权限给ADC按键设置的PPM
		if(Controler_State==Unlock_Controler)//如果处于解锁状态，退出SDK模式保持原地悬停
		{
			ADC_PPM_Databuf[0]=1500;
			ADC_PPM_Databuf[1]=1500;
			ADC_PPM_Databuf[2]=1500;
			ADC_PPM_Databuf[3]=1500;//前四个通道处于中位
			
			ADC_PPM_Databuf[4]=2000;//第五通道高位，处于定高模式
			ADC_PPM_Databuf[5]=1000;//第六通道低位
			ADC_PPM_Databuf[6]=2000;//第七通道高位，处于一键降落模式
			ADC_PPM_Databuf[7]=2000;//第八通道高位，处于光流定点模式
			Bling_Set(&Light_3,5000,1000,0.5,0,GPIO_PORTF_BASE,GPIO_PIN_3,1);
		}
	}
#endif
}




