#include "MY_anonymous.h"
#include "Headfile.h"//用户自己库函数头文件
#include "DIY_OP.h"
int data_to_send[18];

/*------------------------------------------------------------------------------------------------------
【函    数】老梁的蓝牙传输函数(两个函数组合）
【功    能】                
【参    数】
【返 回 值】
【注意事项】                         
-------------------------------------------------------------------------------------------------------*/

void Bluetooth_transmission(void)
{
  data_to_send[0]=(int)openmv.x_target;
  data_to_send[1]=(int)NamelessQuad.Position[_YAW];
	data_to_send[2]=(int)Total_Controller.High_Position_Control.Control_OutPut;//OpticalFlow_Pos_Ctrl_Expect.x*10
  data_to_send[3]=(int)Total_Controller.High_Position_Control.Expect;//OpticalFlow_Pos_Ctrl_Expect.y;
	data_to_send[4]=(int)SDK1_Mode_Setup;
  data_to_send[5]=(int)SDK_Duty_Cnt;
	data_to_send[6]=(int)GD_Distance;
	data_to_send[7]=(int)now_height;
	data_to_send[8]=(int)height_flag;//
	data_to_send[9]=(int)NamelessQuad.Position[_YAW];//
  Data_Send_F1(data_to_send,10);
}


void Data_Send_F1(int *pst, unsigned char len)
{
    unsigned char sum = 0,add=0;
    unsigned char i;
    unsigned char data_to_send[45];
    data_to_send[0] = 0xAA;
    data_to_send[1] = 0xFF;
    data_to_send[2] = 0xF1;
    data_to_send[3] = 2*len;
    for(i=0;i<len;i++)
    {
        data_to_send[2*i+4]=(unsigned char)pst[i];
        data_to_send[2*i+5]=(unsigned char)(pst[i]>>8);
    }
    for(i=0;i<2*len+4;i++)
    {
        sum += data_to_send[i];
			add+=sum;
    }
    data_to_send[2*len+4] = sum;
		data_to_send[2*len+5] = add;

		USART1_Send(data_to_send,2*len+6);//用户移植时，重写此串口发送函数
}

