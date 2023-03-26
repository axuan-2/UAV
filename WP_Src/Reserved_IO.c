#include "Headfile.h"
#include "Reserved_IO.h"



//LaunchPad版本飞控时，看下此链接：https://www.bilibili.com/read/cv12740760
//TIVA LaunchPad飞控扩展版预留的IO1——PD0、IO2——PD1不能直接用
//需要用到IO1、IO2时，自行取下launchpad上的R9、R10电阻


#define RESERVED_IO_ENABLE 0
//使能前————务必取下R9/R10，否则飞控PWM输出会不正常，会导致炸鸡



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

