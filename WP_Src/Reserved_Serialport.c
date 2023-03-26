#include "Headfile.h"
#include "Reserved_Serialport.h"


void Reserved_Serialport_Init()
{
#if FRONT_RANGE_FINDER
	ConfigureUART6(9600,10);							 //串口6初始化，前向超声波测距
#else
	Set_GPS_USART();
#endif
}




us100_data us100_front;
void us100_front_statemachine(void)
{
#if FRONT_RANGE_FINDER
	static uint16_t _cnt=0;
	_cnt++;
	if(_cnt>=20)
	{
		_cnt=0;
		us100_front.pre_last_distance=us100_front.last_distance;//上上次高度
		us100_front.last_distance=us100_front.distance;//上次高度
		us100_front.distance=US_100_Distance(COM6_Rx_Buf.Ring_Buff[0],COM6_Rx_Buf.Ring_Buff[1]);

		us100_front.last_vel=us100_front.vel;
		us100_front.vel=(us100_front.distance-us100_front.last_distance)/0.1f;
		us100_front.acc=(us100_front.vel-us100_front.last_vel)/0.1f;		
		
		COM6_Rx_Buf.Head=1;
		COM6_Rx_Buf.Tail=0; 
		UARTCharPut(UART6_BASE,US_100_Distance_CMD);
	}
#endif
}


