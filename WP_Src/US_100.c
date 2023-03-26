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
#include "US_100.h"




float GD_Distance=0,GD_Distance_Div=0,GD_Distance_Acc=0,Last_GD_Distance=0,Last_GD_Distance_Div=0;
uint16_t Start_Tail=0,Last_Start_Tail;
/***********************************************************
@函数名：US_100_Distance
@入口参数：uint8 MSB,uint8 LSB
@出口参数：无
@功能描述：超声波高度数据合成
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
float US_100_Distance(uint8 MSB,uint8 LSB)
{
  return (256*(MSB)+(LSB))/10.0f;//单位cm
}

/***********************************************************
@函数名：US_100_Temperature
@入口参数：uint8 data
@出口参数：无
@功能描述：超声波温度数据合成
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
float US_100_Temperature(uint8 data)
{
  return (data-45)/1.0;
}

/***********************************************************
@函数名：US_100_Start
@入口参数：无
@出口参数：无
@功能描述：超声波高度数据触发
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void US_100_Start(void)
{	
  UARTCharPut(UART7_BASE, US_100_Distance_CMD);
}


uint8_t US_100_Update_Flag=0;
uint8 US_100_Cnt=0;



us100_data us100;


/***********************************************************
@函数名：US_100_Statemachine
@入口参数：无
@出口参数：无
@功能描述：超声波数据解析状态机
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void US_100_Statemachine(void)
{
	static uint8_t fault_flag=0;
  US_100_Cnt++;
  if(US_100_Cnt>=20)//100ms
  {
    US_100_Start();//超声波触发，接收在外部中断里面处理
    US_100_Cnt=0;
    Start_Tail=COM7_Rx_Buf.Tail;
    if(ABS(Start_Tail-Last_Start_Tail)==2)
    {
			us100.pre_last_distance=us100.last_distance;//上上次高度
			us100.last_distance=us100.distance;//上次高度
						
      if(Start_Tail==0)
      {
        us100.distance=US_100_Distance(COM7_Rx_Buf.Ring_Buff[2],
                                    COM7_Rx_Buf.Ring_Buff[3]);
        US_100_Update_Flag=1;
      }
      else if(Start_Tail==1)
      {
        us100.distance=US_100_Distance(COM7_Rx_Buf.Ring_Buff[3],
                                    COM7_Rx_Buf.Ring_Buff[0]);
        US_100_Update_Flag=1;
      }
      else if(Start_Tail==2)
      {
        us100.distance=US_100_Distance(COM7_Rx_Buf.Ring_Buff[0],
                                    COM7_Rx_Buf.Ring_Buff[1]);
        US_100_Update_Flag=1;
      }
      else if(Start_Tail==3)
      {
        us100.distance=US_100_Distance(COM7_Rx_Buf.Ring_Buff[1],
                                    COM7_Rx_Buf.Ring_Buff[2]);
        US_100_Update_Flag=1;
      }
			
			us100.last_vel=us100.vel;
			us100.vel=(us100.distance-us100.last_distance)/0.1f;
			us100.acc=(us100.vel-us100.last_vel)/0.1f;
			if(ABS(us100.vel)<1000)
			{
				if(fault_flag==1&&us100.distance==us100.pre_last_distance)//本次=上上次
				{
					fault_flag=1;
				}			
				else if(fault_flag==1&&us100.distance==us100.last_distance)//本次=上次
				{
					fault_flag=1;
				}
				else if(fault_flag==1&&us100.last_distance==us100.pre_last_distance)//上次=上上次
				{
					fault_flag=1;		
				}
				//以上为传感器异常处理
				else
				{
					GD_Distance=us100.distance;
					GD_Distance_Div=us100.vel;
					GD_Distance_Acc=us100.acc;		
				}
			}
			else
			{
				fault_flag=1;		
			}	
			WP_Sensor.us100_updtate_flag=1;
    }
    Last_Start_Tail=Start_Tail;
  }		
	
  if(GD_Distance<=User_Height_Max&&GD_Distance>0)  Sensor_Flag.Ground_Health=1;
  else  Sensor_Flag.Ground_Health=0; 
}


