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
*               修改日期:2020/03/24                   
*               版本：躺赢者PRO——CarryPilot_V3.0.1
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
#include "uart.h"
#include "Usart.h"
#include "Ringbuf.h"
#include "DIY_OP.h"

void UART6_IRQHandler(void);


//串口循环队列缓冲数据定义
RingBuff_t COM0_Rx_Buf,COM1_Rx_Buf,COM2_Rx_Buf,COM3_Rx_Buf,COM4_Rx_Buf,COM5_Rx_Buf,COM6_Rx_Buf,COM7_Rx_Buf;
/***********************************************************
@函数名：UART0_IRQHandler
@入口参数：无
@出口参数：无
功能描述：串口0数据接收
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void UART0_IRQHandler(void)//UART0中断函数
{	
  //获取中断标志 原始中断状态 不屏蔽中断标志		
  uint32_t flag = UARTIntStatus(UART0_BASE,1);
  //清除中断标志	
  UARTIntClear(UART0_BASE,flag);		
  //判断FIFO是否还有数据		
  while(UARTCharsAvail(UART0_BASE))		
  {			
		RingBuf_Write(UARTCharGet(UART0_BASE),&COM0_Rx_Buf,SDK_Target_Length*9);//往环形队列里面写数据
		if(COM0_Rx_Buf.Ring_Buff[0]!=0xFF)
		{
			COM0_Rx_Buf.Head=1;
			COM0_Rx_Buf.Tail=0; 
		}	
		
  }
}

/***********************************************************
@函数名：ConfigureUART0
@入口参数：无
@出口参数：无
功能描述：串口0配置
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void ConfigureUART0(void)//串口0初始化
{
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);//使能GPIO外设		
  SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);//使能UART外设
  GPIOPinConfigure(GPIO_PA0_U0RX);//GPIO模式配置 PA0--RX PA1--TX 
  GPIOPinConfigure(GPIO_PA1_U0TX);
  GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);//GPIO的UART模式配置
  UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);
  UARTStdioConfig(0, 921600, 16000000);
  //UART协议配置 波特率115200 8位 1停止位  无校验位	
  //UART禁用FIFO 默认FIFO Level为4/8 寄存器满8字节后产生中断	//禁用后接收1位就产生中断	
  UARTFIFODisable(UART0_BASE);//使能UART0中断	IntEnable(INT_UART0);	
  UARTIntEnable(UART0_BASE,UART_INT_RX);//使能UART0接收中断		
  UARTIntRegister(UART0_BASE,UART0_IRQHandler);//UART中断地址注册	
  IntPrioritySet(INT_UART0, USER_INT1);
}

/***********************************************************
@函数名：USART0_Send
@入口参数：uint8_t *pui8Buffer, uint32_t ui32Count
@出口参数：无
功能描述：串口0发送N个字节数据
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void USART0_Send(uint8_t *pui8Buffer, uint32_t ui32Count)//发送N个字节长度的数据
{
  while(ui32Count--)
  {
    UARTCharPut(UART0_BASE, *pui8Buffer++);
  }
}
/***********************************************************
@函数名：wust_sendware
@入口参数：unsigned char *wareaddr, int16_t waresize
@出口参数：无
功能描述：山外虚拟示波器发送
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void wust_sendware(unsigned char *wareaddr, int16_t waresize)//山外发送波形
{
#define CMD_WARE     3
  uint8 cmdf[2] = {CMD_WARE, ~CMD_WARE};//帧头
  uint8 cmdr[2] = {~CMD_WARE, CMD_WARE};//帧尾
  USART0_Send(cmdf, sizeof(cmdf));
  USART0_Send(wareaddr, waresize);
  USART0_Send(cmdr, sizeof(cmdr));
}

/***********************************************************
@函数名：UART1_IRQHandler
@入口参数：无
@出口参数：无
功能描述：串口1数据接收
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void UART1_IRQHandler(void)//UART1中断函数
{				
  uint32_t flag = UARTIntStatus(UART1_BASE,1);//获取中断标志 原始中断状态 屏蔽中断标志	
  UARTIntClear(UART1_BASE,flag);//清除中断标志	
  while(UARTCharsAvail(UART1_BASE))//判断FIFO是否还有数据		
  {			
    //RingBuf_Write(UARTCharGet(UART1_BASE),&COM1_Rx_Buf,100);//往环形队列里面写数据	
    NCLink_Data_Prase_Prepare(UARTCharGet(UART1_BASE)); 		
  }
}


/***********************************************************
@函数名：USART1_Send
@入口参数：uint8_t *pui8Buffer, uint32_t ui32Count
@出口参数：无
功能描述：串口1发送N个字节数据
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void USART1_Send(uint8_t *pui8Buffer, uint32_t ui32Count)//发送N个字节长度的数据
{
  while(ui32Count--)
  {
    UARTCharPut(UART1_BASE, *pui8Buffer++);
  }
}
/***********************************************************
@函数名：ConfigureUART1
@入口参数：无
@出口参数：无
功能描述：串口1配置
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void ConfigureUART1(void)//串口1初始化
{
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);//使能GPIO外设		
  SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);//使能UART外设
  GPIOPinConfigure(GPIO_PB0_U1RX);//GPIO模式配置 PB0--RX PB1--TX 
  GPIOPinConfigure(GPIO_PB1_U1TX);
  GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);//GPIO的UART模式配置
  UARTClockSourceSet(UART1_BASE, UART_CLOCK_PIOSC);
  UARTStdioConfig(1, 115200, 16000000);//115200
  UARTFIFODisable(UART1_BASE);//使能UART1中断	
  UARTIntEnable(UART1_BASE,UART_INT_RX);//使能UART1接收中断		
  UARTIntRegister(UART1_BASE,UART1_IRQHandler);//UART1中断地址注册	
  IntPrioritySet(INT_UART1, USER_INT4);
}


/***********************************************************
@函数名：UART2_IRQHandler
@入口参数：无
@出口参数：无
功能描述：串口2数据接收
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void UART2_IRQHandler(void)
{
  uint32_t flag = UARTIntStatus(UART2_BASE,1);//获取中断标志 原始中断状态 屏蔽中断标志		
  UARTIntClear(UART2_BASE,flag);//清除中断标志	
  while(UARTCharsAvail(UART2_BASE))//判断FIFO是否还有数据				
  {		
		RingBuf_Write(UARTCharGet(UART2_BASE),&COM2_Rx_Buf,FLOW_BUF_CNT);//往环形队列里面写数据				
  }
}

/***********************************************************
@函数名：USART2_Send
@入口参数：uint8_t *pui8Buffer, uint32_t ui32Count
@出口参数：无
功能描述：串口2发送N个字节数据
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void USART2_Send(uint8_t *pui8Buffer, uint32_t ui32Count)//发送N个字节长度的数据
{
  while(ui32Count--)
  {
    UARTCharPut(UART2_BASE, *pui8Buffer++);
  }
}
/***********************************************************
@函数名：ConfigureUART2
@入口参数：无
@出口参数：无
功能描述：串口2配置
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void ConfigureUART2(void)//串口6初始化
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);//使能GPIO外设		
  SysCtlPeripheralEnable(SYSCTL_PERIPH_UART2);//使能UART外设
  
  HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;//解锁PD6
  HWREG(GPIO_PORTD_BASE + GPIO_O_CR) |= 0x80;//确认
  HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = 0;//重新锁定
  
  GPIOPinConfigure(GPIO_PD6_U2RX);//GPIO模式配置 PD6--RX PD7--TX 
  GPIOPinConfigure(GPIO_PD7_U2TX);
  GPIOPinTypeUART(GPIO_PORTD_BASE, GPIO_PIN_6 | GPIO_PIN_7);//GPIO的UART模式配置
  UARTConfigSetExpClk(UART2_BASE, SysCtlClockGet(), 19200,
                      (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                       UART_CONFIG_PAR_NONE));
  UARTFIFODisable(UART2_BASE);//使能UART2中断	

  UARTIntEnable(UART2_BASE,UART_INT_RX);//使能UART6接收中断		
  OpticalFlow_Init();//光流滤波参数初始化
  OpticalFlow_Is_Work=Config_Init_Uart();//光流传感器初始化
	
  UARTIntRegister(UART2_BASE,UART2_IRQHandler);//UART中断地址注册	
  IntPrioritySet(INT_UART2, USER_INT5);
}

/***********************************************************
@函数名：UART3_IRQHandler
@入口参数：无
@出口参数：无
功能描述：串口3数据接收
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void UART3_IRQHandler(void)
{		
  uint32_t flag = UARTIntStatus(UART3_BASE,1);//获取中断标志 原始中断状态 屏蔽中断标志		
  UARTIntClear(UART3_BASE,flag);//清除中断标志			
  while(UARTCharsAvail(UART3_BASE))//判断FIFO是否还有数据		
  {			
		RingBuf_Write(UARTCharGet(UART3_BASE),&COM3_Rx_Buf,SDK_Target_Length*2);//往环形队列里面写数据
		if(COM3_Rx_Buf.Ring_Buff[0]!='*')
		{
			COM3_Rx_Buf.Head=1;
			COM3_Rx_Buf.Tail=0; 
		}	
	}	
}
uint8_t uart3_buff[10],uart3_data,start_receive_flag,i,mode,flag_inter;
int temp_1,x,y,sysle_flag=0;
uint8_t uart3_buff_circle[22],head=0,tail=0,len_mx=12;
//void UART3_IRQHandler(void)
//{		
//  uint32_t flag = UARTIntStatus(UART3_BASE,1);//?????? ?????? ??????		
//  UARTIntClear(UART3_BASE,flag);//??????			
//  while(UARTCharsAvail(UART3_BASE))//??FIFO??????		
//  {			
//			
//		uart3_data=UARTCharGet(UART3_BASE);
//		if(sysle_flag!=1)
//		{
//			uart3_buff_circle[tail]=uart3_data;
//	    tail++;
//		}
//	    
//	    if(tail>=len_mx) 
//		{
//			tail=0;
//		}
//		if(tail==head)
//		{
//			head++;
//		if(head>=len_mx) 
//		{
//			head=0;
//		}
//		}
////		if(uart3_buff_circle[tail-1]==';' || uart3_buff_circle[tail-1]==':' || uart3_buff_circle[tail-1]=='#')
////		{
////			while(head!=tail-1)
////			{
////				if(uart3_buff_circle[head]=='*')
////				{
////					start_receive_flag=1;
////				}
////				else if(start_receive_flag==1)
////				{
////					temp_1=(uart3_buff_circle[head]-'0')*pow(10,3-start_receive_flag);
////					start_receive_flag++;
////				}
////				head++;
////				if(head>=len_mx) 
////				{
////					head=0;
////				}
////			}
////			if(uart3_buff_circle[tail-1]==';')
////			{openmv.x_target=temp_1-360;}
////			if(uart3_buff_circle[tail-1]==':')
////			{openmv.y_target=temp_1-360;}
////			if(uart3_buff_circle[tail-1]=='#')
////			{
////				openmv.state=(int)temp_1/100;
////			}
////			start_receive_flag=0;
////		}
////	  
////		flag_inter=!flag_inter;
////		if(uart3_data == ';' && i>=3)//????
////		{		
////			temp_1=0;

////			for(uint8_t k=0;k<i;k++)
////			temp_1+=(uart3_buff[k]-'0')*pow(10,i-k-1);
////			openmv.x_target=temp_1-360;
////			start_receive_flag = 0;
////			i = 0;
////		}
////		else if(uart3_data == ':' && i>=3)
////		{
////			temp_1=0;

////			for(uint8_t k=0;k<i;k++)
////			temp_1+=(uart3_buff[k]-'0')*pow(10,i-k-1);
////			openmv.y_target=temp_1-360;
////			start_receive_flag = 0;
////			i = 0;			
////		}
////		else if(uart3_data == '#')
////		{
////			mode=uart3_buff[0]-'0';
////			openmv.state=mode;
////		//	memset(uart3_buff,0,sizeof(uart3_buff));
////			start_receive_flag = 0;
////			i = 0;
////			//0 起飞
////			//1 悬停
////			//2 降落
////		}
////		 if(start_receive_flag == 1)   //??????
////		{ 
////			uart3_buff[i]=uart3_data;
////			i++;
////		}
////		if(uart3_data == '*')//????
////		{
////			start_receive_flag = 1;
////			i=0;
////	//		memset(uart3_buff,0,sizeof(uart3_buff));
////		}
//	}	
//}
char uart3_buff_circle_copy[22];
void usaaaa(void)
{
	temp_1=0;
	//sysle_flag=1;
	 for(int j=0;j<len_mx;j++)
	 {
		uart3_buff_circle_copy[j]=uart3_buff_circle[j];
	 }
			while(head!=tail-1 && tail!=0)
			{
				if(uart3_buff_circle_copy[head]=='*')
				{
					start_receive_flag=1;
				}
				else if(start_receive_flag>=1)
				{
					if((uart3_buff_circle_copy[head]-'0')>=0 &&(uart3_buff_circle_copy[head]-'0')<=9)
					{
						temp_1+=(uart3_buff_circle_copy[head]-'0')*pow(10,3-start_receive_flag);
					 start_receive_flag++;
					}
//					else 
//					{
//						temp_1=0;
//					 start_receive_flag=0;
//					}
				}
				head++;
				if(head>=len_mx) 
				{
					head=0;
				}
				if(uart3_buff_circle_copy[head]==';' || uart3_buff_circle_copy[head]==':' || uart3_buff_circle_copy[head]=='#')
				{
					if(uart3_buff_circle_copy[head]=='#')
			    {
			    	openmv.state=(int)temp_1/100;
						temp_1=0;
						
			    }
					if(start_receive_flag==0 || start_receive_flag<4)
						continue;
					if(uart3_buff_circle_copy[head]==';')
			    {
						openmv.last_x_target=openmv.x_target;
						openmv.x_target=temp_1-360;
						if(abs(openmv.last_x_target-openmv.x_target)>100&&openmv.x_target!=0)
							openmv.x_target=openmv.last_x_target;
					}
			    if(uart3_buff_circle_copy[head]==':')
			    {
						 openmv.last_y_target=openmv.y_target;
					   openmv.y_target=temp_1-360;
						if(abs(openmv.last_y_target-openmv.y_target)>100&&openmv.y_target!=0)
							openmv.y_target=openmv.last_y_target;
					}
			    temp_1=0;
			   start_receive_flag=0;
				}
			}
			//sysle_flag=0;

//			for(int j=0;j<len_mx;j++)
//	{
//	  	if(uart3_buff_circle[j]=='*')
//				{
//					start_receive_flag=1;
//				}
//				else if(start_receive_flag>=1)
//				{
//					temp_1+=(uart3_buff_circle[j]-'0')*pow(10,3-start_receive_flag);
//					start_receive_flag++;
//				}
//				if(uart3_buff_circle[j]==';' || uart3_buff_circle[j]==':' || uart3_buff_circle[j]=='#')
//				{
//					
//					if(start_receive_flag==0)
//						continue;
//					if(uart3_buff_circle[j]==';')
//			    {openmv.x_target=temp_1-360;}
//			    if(uart3_buff_circle[j]==':')
//			    {openmv.y_target=temp_1-360;}
//			    if(uart3_buff_circle[j]=='#')
//			    {
//			    	openmv.state=(int)temp_1/100;
//			    }temp_1=0;
//			   start_receive_flag=0;
//				}
//			}
}

void json_analysis(void)
{
	for(int j=0;j<len_mx;j++)
	{
		uart3_buff_circle_copy[j]=uart3_buff_circle[j];
	}
	cJSON *json_receive = NULL;
	cJSON *json_x = NULL;
	cJSON *json_y = NULL;
	cJSON *json_mode = NULL;
	
	json_receive = cJSON_Parse(uart3_buff_circle_copy);
	json_x = cJSON_GetObjectItem( json_receive , "x" );
	json_y = cJSON_GetObjectItem( json_receive , "y" );
	json_mode = cJSON_GetObjectItem( json_receive , "m" );
	
	cJSON_Delete(json_receive);
	cJSON_Delete(json_x);
	cJSON_Delete(json_y);
	cJSON_Delete(json_mode);
}
/***********************************************************
@函数名：USART3_Send
@入口参数：uint8_t *pui8Buffer, uint32_t ui32Count
@出口参数：无
功能描述：串口3发送N个字节数据
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void USART3_Send(uint8_t *pui8Buffer, uint32_t ui32Count)//发送N个字节长度的数据
{
  while(ui32Count--)
  {
    UARTCharPut(UART3_BASE, *pui8Buffer++);
  }
}

/***********************************************************
@函数名：ConfigureUART3
@入口参数：无
@出口参数：无
功能描述：串口3配置
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void ConfigureUART3(void)//串口3初始化
{
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);//使能GPIO外设		
  SysCtlPeripheralEnable(SYSCTL_PERIPH_UART3);//使能UART外设
  GPIOPinConfigure(GPIO_PC6_U3RX);//GPIO模式配置 PC6--RX PC7--TX 
  GPIOPinConfigure(GPIO_PC7_U3TX);
  GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_6 | GPIO_PIN_7);//GPIO的UART模式配置
  UARTConfigSetExpClk(UART3_BASE, SysCtlClockGet(), 921600,
                      (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                       UART_CONFIG_PAR_NONE));
  UARTFIFODisable(UART3_BASE);//使能UART0中断	
  UARTIntEnable(UART3_BASE,UART_INT_RX);//使能UART3接收中断		
  UARTIntRegister(UART3_BASE,UART3_IRQHandler);//UART3中断地址注册	
  IntPrioritySet(INT_UART3, USER_INT1);
}

/***********************************************************
@函数名：UART6_IRQHandler
@入口参数：无
@出口参数：无
功能描述：串口6数据接收
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
uint16_t UART6_LEN=0;
void UART6_IRQHandler(void)
{		
  uint32_t flag = UARTIntStatus(UART6_BASE,1);//获取中断标志 原始中断状态 屏蔽中断标志		
  UARTIntClear(UART6_BASE,flag);//清除中断标志	
  while(UARTCharsAvail(UART6_BASE))//判断FIFO是否还有数据		
  {	
#if FRONT_RANGE_FINDER
			RingBuf_Write(UARTCharGet(UART6_BASE),&COM6_Rx_Buf,UART6_LEN);//往环形队列里面写数据
#else
			RingBuf_Write(UARTCharGet(UART6_BASE),&COM6_Rx_Buf,UART6_LEN);//往环形队列里面写数据
			if(COM6_Rx_Buf.Ring_Buff[0]!=0XB5)
			{
				COM6_Rx_Buf.Head=1;
				COM6_Rx_Buf.Tail=0; 
			}		
#endif		
  }
}

/***********************************************************
@函数名：USART6_Send
@入口参数：uint8_t *pui8Buffer, uint32_t ui32Count
@出口参数：无
功能描述：串口6发送N个字节数据
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void USART6_Send(uint8_t *pui8Buffer, uint32_t ui32Count)//发送N个字节长度的数据
{
  while(ui32Count--)
  {
    UARTCharPut(UART6_BASE, *pui8Buffer++);
  }
}


/***********************************************************
@函数名：ConfigureUART6
@入口参数：无
@出口参数：无
功能描述：串口6配置
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void ConfigureUART6(unsigned long bound,uint16_t len)
{
	UART6_LEN=len;
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);//使能GPIO外设		
  SysCtlPeripheralEnable(SYSCTL_PERIPH_UART6);//使能UART外设
  
	HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;//解锁PD6
	HWREG(GPIO_PORTD_BASE + GPIO_O_CR) |= 0x80;//确认
	HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = 0;//重新锁定
	
	GPIOPinConfigure(GPIO_PD4_U6RX);//GPIO模式配置 PD4--RX PD5--TX 
  GPIOPinConfigure(GPIO_PD5_U6TX);
  GPIOPinTypeUART(GPIO_PORTD_BASE, GPIO_PIN_4 | GPIO_PIN_5);//GPIO的UART模式配置
  UARTConfigSetExpClk(UART6_BASE, SysCtlClockGet(), bound,
                      (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                       UART_CONFIG_PAR_NONE));
  UARTFIFODisable(UART6_BASE);//使能UART0中断	
  UARTIntEnable(UART6_BASE,UART_INT_RX);//使能UART6接收中断		
	
  UARTIntRegister(UART6_BASE,UART6_IRQHandler);//UART6中断地址注册	
  IntPrioritySet(INT_UART6, USER_INT2);
}





/***********************************************************
@函数名：UART7_IRQHandler
@入口参数：无
@出口参数：无
功能描述：串口7数据接收
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void UART7_IRQHandler(void)//UART2中断函数
{		
  uint32_t flag = UARTIntStatus(UART7_BASE,1);//获取中断标志 原始中断状态 屏蔽中断标志	
  UARTIntClear(UART7_BASE,flag);//清除中断标志		
  while(UARTCharsAvail(UART7_BASE))//判断FIFO是否还有数据			
  {			
    //输出得到的数据			
		if(Ground_Sensor_Now_Mode==US100)					
		{
			RingBuf_Write(UARTCharGet(UART7_BASE),&COM7_Rx_Buf,4);//往环形队列里面写数据
		}
		else	if(Ground_Sensor_Now_Mode==TFMINI)	
		{
			RingBuf_Write(UARTCharGet(UART7_BASE),&COM7_Rx_Buf,18);//往环形队列里面写数据
			if(COM7_Rx_Buf.Ring_Buff[0]!=0x59)
			{
				COM7_Rx_Buf.Head=1;
				COM7_Rx_Buf.Tail=0; 
			}	
		}
		else	if(Ground_Sensor_Now_Mode==TOFSENSOR)	
		{
			uint8_t data=UARTCharGet(UART7_BASE);
			RingBuf_Write(data,&COM7_Rx_Buf,16*TOFSENSE_CURRENT_WORK);//往环形队列里面写数据  32
			TOF_Sense_Prase(data);
			if(COM7_Rx_Buf.Ring_Buff[0]!=0x57)
			{
				COM7_Rx_Buf.Head=1;
				COM7_Rx_Buf.Tail=0; 
			}				
		}			
  }
}

/***********************************************************
@函数名：USART7_Send
@入口参数：uint8_t *pui8Buffer, uint32_t ui32Count
@出口参数：无
功能描述：串口7发送N个字节数据
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void USART7_Send(uint8_t *pui8Buffer, uint32_t ui32Count)//发送N个字节长度的数据
{
  while(ui32Count--)
  {
    UARTCharPut(UART7_BASE, *pui8Buffer++);
  }
}

/***********************************************************
@函数名：ConfigureUART7
@入口参数：无
@出口参数：无
功能描述：串口7配置
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void ConfigureUART7(void)//串口7初始化
{
	static uint16_t state_init_cnt=0;
	ReadFlashParameterOne(GROUND_DISTANCE_DEFAULT,&ground_sensor_default);
  if(isnan(ground_sensor_default)==0)
  {
    Ground_Sensor_Now_Mode=(uint8_t)(ground_sensor_default);
    if(Ground_Sensor_Now_Mode==US100)	
		{  
			SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);//使能GPIO外设		
			SysCtlPeripheralEnable(SYSCTL_PERIPH_UART7);//使能UART外设
			GPIOPinConfigure(GPIO_PE0_U7RX);//GPIO模式配置 PE0--RX PE1--TX 
			GPIOPinConfigure(GPIO_PE1_U7TX);
			GPIOPinTypeUART(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1);//GPIO的UART模式配置
			UARTConfigSetExpClk(UART7_BASE, SysCtlClockGet(), 9600,
												(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
												 UART_CONFIG_PAR_NONE));
			RingBuff_Init(&COM7_Rx_Buf); 	
			UARTFIFODisable(UART7_BASE);//使能UART0中断	
			UARTIntEnable(UART7_BASE,UART_INT_RX);//使能UART0接收中断		
			UARTIntRegister(UART7_BASE,UART7_IRQHandler);//UART中断地址注册	
			IntPrioritySet(INT_UART7, USER_INT3);
			state_init_cnt=1;
		}
		else if(Ground_Sensor_Now_Mode==TFMINI)	
		{
			SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);//使能GPIO外设		
			SysCtlPeripheralEnable(SYSCTL_PERIPH_UART7);//使能UART外设
			GPIOPinConfigure(GPIO_PE0_U7RX);//GPIO模式配置 PE0--RX PE1--TX 
			GPIOPinConfigure(GPIO_PE1_U7TX);
			GPIOPinTypeUART(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1);//GPIO的UART模式配置
			UARTConfigSetExpClk(UART7_BASE, SysCtlClockGet(), 115200,
													(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
													 UART_CONFIG_PAR_NONE));	
			RingBuff_Init(&COM7_Rx_Buf); 	
			UARTFIFODisable(UART7_BASE);//使能UART0中断	
			UARTIntEnable(UART7_BASE,UART_INT_RX);//使能UART0接收中断		
			UARTIntRegister(UART7_BASE,UART7_IRQHandler);//UART中断地址注册	
			IntPrioritySet(INT_UART7, USER_INT3);
			state_init_cnt=1;
		}
		else if(Ground_Sensor_Now_Mode==TOFSENSOR)	
		{
			SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);//使能GPIO外设		
			SysCtlPeripheralEnable(SYSCTL_PERIPH_UART7);//使能UART外设
			GPIOPinConfigure(GPIO_PE0_U7RX);//GPIO模式配置 PE0--RX PE1--TX 
			GPIOPinConfigure(GPIO_PE1_U7TX);
			GPIOPinTypeUART(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1);//GPIO的UART模式配置
			UARTConfigSetExpClk(UART7_BASE, SysCtlClockGet(), 921600,
													(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
													 UART_CONFIG_PAR_NONE));	
			RingBuff_Init(&COM7_Rx_Buf); 	
			UARTFIFODisable(UART7_BASE);//使能UART0中断	
			UARTIntEnable(UART7_BASE,UART_INT_RX);//使能UART0接收中断		
			UARTIntRegister(UART7_BASE,UART7_IRQHandler);//UART中断地址注册	
			IntPrioritySet(INT_UART7, USER_INT3);
			state_init_cnt=1;
		}
		else if(Ground_Sensor_Now_Mode==VL53L1X)
		{
      if(state_init_cnt==0)	 VL53L1Init(&VL53);
		}			
  }
	else//默认采用US100
	{
			SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);//使能GPIO外设		
			SysCtlPeripheralEnable(SYSCTL_PERIPH_UART7);//使能UART外设
			GPIOPinConfigure(GPIO_PE0_U7RX);//GPIO模式配置 PE0--RX PE1--TX 
			GPIOPinConfigure(GPIO_PE1_U7TX);
			GPIOPinTypeUART(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1);//GPIO的UART模式配置
			UARTConfigSetExpClk(UART7_BASE, SysCtlClockGet(), 9600,
												(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
												 UART_CONFIG_PAR_NONE));
			RingBuff_Init(&COM7_Rx_Buf); 	
			UARTFIFODisable(UART7_BASE);//使能UART0中断	
			UARTIntEnable(UART7_BASE,UART_INT_RX);//使能UART0接收中断		
			UARTIntRegister(UART7_BASE,UART7_IRQHandler);//UART中断地址注册	
			IntPrioritySet(INT_UART7, USER_INT3);
			state_init_cnt=1;
	}

}



/***********************************************************
@函数名：Vcan_Send
@入口参数：无
@出口参数：无
功能描述：山外数据发送函数，默认发送8个通道，数据类型为float
，每个通道数据可以自己定义
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void Vcan_Send(void)//山外地面站发送
{
  static float DataBuf[8];	

	DataBuf[0]=Pitch;
  DataBuf[1]=Roll;
  DataBuf[2]=Yaw;
  DataBuf[3]=Pitch_Observation;
  DataBuf[4]=Roll_Observation;
  DataBuf[5]=Yaw_Observation;
  DataBuf[6]=0;
  DataBuf[7]=0;


//  DataBuf[0]=NamelessQuad.Position[_YAW];//惯导高度
//  DataBuf[1]=NamelessQuad.Speed[_YAW];//惯导速度
//  DataBuf[2]=Observation_Altitude;//NamelessQuad.Acceleration[_YAW];;//惯导加速度
//  DataBuf[3]=NamelessQuad.Acceleration[_YAW];//气压计高度
//	DataBuf[4]=Origion_NamelessQuad.Acceleration[_YAW];
//  DataBuf[5]=NamelessQuad.Acce_Bias[_YAW];//acc_correction[_YAW];
//  DataBuf[6]=vel_correction[_YAW];
//	DataBuf[7]=vl52l1x_result_data.RangeMilliMeter/10.0;
	
/*
//	DataBuf[0]=Accel_X_Butter_Filter.output[Accel_X_Butter_Filter.N-1];;
//  DataBuf[1]=Accel_Y_Butter_Filter.output[Accel_Y_Butter_Filter.N-1];;
//  DataBuf[2]=Accel_Z_Butter_Filter.output[Accel_Z_Butter_Filter.N-1];;
//  DataBuf[3]=accel.x;
//	DataBuf[4]=accel.y;
//  DataBuf[5]=accel.z;
	
	
  
  DataBuf[0]=GPS_Vel_Div.E;//惯导高度
  DataBuf[1]=GPS_Vel_Div.N;//惯导速度
  DataBuf[2]=NamelessQuad.Acceleration[_PITCH];;//惯导加速度
  DataBuf[3]=NamelessQuad.Acceleration[_ROLL];;//惯导加速度
  DataBuf[4]=0;//惯导高度;
  DataBuf[5]=0;
  DataBuf[6]=0;
  DataBuf[7]=0;   
  */
  //2      
  /*
	DataBuf[0]=opt_gyro_filter_data.x;
	DataBuf[1]=opt_gyro_filter_data.y;
	DataBuf[2]=0;
	DataBuf[3]=OpticalFlow_SINS.Speed[_PITCH];
	DataBuf[4]=OpticalFlow_SINS.Position[_PITCH];
	DataBuf[5]=OpticalFlow_Speed.x;
	DataBuf[6]=OpticalFlow_Position.x;
	DataBuf[7]=OpticalFlow_SINS.Acceleration[_PITCH];   
  
	
  DataBuf[0]=gyro_filter_data.x;//Pitch X
  DataBuf[1]=gyro_filter_data.y;//Roll R
  DataBuf[2]=opt_gyro_data.x;
  DataBuf[3]=opt_gyro_data.y;
  DataBuf[4]=opt_filter_data.x;
  DataBuf[5]=opt_filter_data.y;
  DataBuf[6]=opt_gyro_filter_data.x;
  DataBuf[7]=opt_gyro_filter_data.y;
  

  DataBuf[0]=PPM_Databuf[0];
  DataBuf[1]=PPM_Databuf[1];
  DataBuf[2]=PPM_Databuf[2];
  DataBuf[3]=PPM_Databuf[3];
  DataBuf[4]=PPM_LPF_Databuf[0];
  DataBuf[5]=PPM_LPF_Databuf[1];
  DataBuf[6]=Total_Controller.Pitch_Gyro_Control.Dis_Err;
  DataBuf[7]=Total_Controller.Pitch_Gyro_Control.Dis_Error_History[0];
  */
  //3
   /*
  DataBuf[0]=NamelessQuad.Position[_PITCH];
  DataBuf[1]=NamelessQuad.Speed[_PITCH];
  DataBuf[2]=GPS_Vel.E;
  DataBuf[3]=Earth_Frame_To_XYZ.E;
  DataBuf[4]=NamelessQuad.Position[_ROLL];
  DataBuf[5]=NamelessQuad.Speed[_ROLL];
  DataBuf[6]=GPS_Vel.N;
  DataBuf[7]=Earth_Frame_To_XYZ.N;
 
  DataBuf[0]=NamelessQuad.Position[_PITCH];
  DataBuf[1]=0;
  DataBuf[2]=NamelessQuad.Position[_ROLL];
  DataBuf[3]=0;
  DataBuf[4]=GPS_Ground_Speed;
  DataBuf[5]=sqrt(NamelessQuad.Speed[_PITCH]*NamelessQuad.Speed[_PITCH]+NamelessQuad.Speed[_ROLL]*NamelessQuad.Speed[_ROLL]);
  DataBuf[6]=0;
  DataBuf[7]=NamelessQuad.Position[_YAW];//惯导高度

  DataBuf[0]=PPM_Databuf[0];
	DataBuf[1]=PPM_Databuf[1];
	DataBuf[2]=PPM_Databuf[2];
	DataBuf[3]=PPM_Databuf[3];
	DataBuf[4]=PPM_LPF_Databuf[0];
	DataBuf[5]=PPM_LPF_Databuf[1];
	DataBuf[6]=PPM_LPF_Databuf[2];
	DataBuf[7]=PPM_LPF_Databuf[3];
	 */
  wust_sendware((unsigned char *)DataBuf,sizeof(DataBuf));
}




