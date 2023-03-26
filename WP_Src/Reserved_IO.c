#include "Headfile.h"
#include "Reserved_IO.h"



//LaunchPad�汾�ɿ�ʱ�����´����ӣ�https://www.bilibili.com/read/cv12740760
//TIVA LaunchPad�ɿ���չ��Ԥ����IO1����PD0��IO2����PD1����ֱ����
//��Ҫ�õ�IO1��IO2ʱ������ȡ��launchpad�ϵ�R9��R10����


#define RESERVED_IO_ENABLE 0
//ʹ��ǰ�����������ȡ��R9/R10������ɿ�PWM����᲻�������ᵼ��ը��



void Reserved_IO_Init(void)
{
#if RESERVED_IO_ENABLE
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
  GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2);
	GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_0,GPIO_PIN_0);
	GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_1,GPIO_PIN_1);
  GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_2,GPIO_PIN_2);
#endif
}


void Reserved_IO_Test(void)
{
#if RESERVED_IO_ENABLE
	static uint16_t _cnt=0;
	_cnt++;
	if(_cnt==50)
	{
		GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_0,GPIO_PIN_0);
		GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_1,0);
		GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_2,0);
	}
	else if(_cnt==100)
	{
		GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_0,0);
		GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_1,GPIO_PIN_1);
		GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_2,0);
	}
	else if(_cnt==150)
	{
		GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_0,0);
		GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_1,0);
		GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_2,GPIO_PIN_2);
	}
	else if(_cnt==200)
	{
		_cnt=0;
		GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_0,GPIO_PIN_0);
		GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_1,GPIO_PIN_1);
		GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_2,GPIO_PIN_2);
	}
#endif
}

