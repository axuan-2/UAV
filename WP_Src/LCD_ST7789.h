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
#ifndef __LCD_ST7789_H
#define __LCD_ST7789_H			

#define	u8 unsigned char
#define	u16 unsigned int
#define	u32 unsigned long

#define Bit_SET   1
#define Bit_RESET 0
#define SYSCTL_PERIPH_GPIO_OLED SYSCTL_PERIPH_GPIOA
#define GPIO_OLED   GPIO_PORTA_BASE 
//-----------------OLED端口定义---------------- 

#define OLED_SCLK_Clr() GPIOPinWrite(GPIO_OLED, GPIO_PIN_5,0)//CLK
#define OLED_SCLK_Set() GPIOPinWrite(GPIO_OLED, GPIO_PIN_5,GPIO_PIN_5)
#define OLED_SDIN_Clr() GPIOPinWrite(GPIO_OLED, GPIO_PIN_4,0)//DIN
#define OLED_SDIN_Set() GPIOPinWrite(GPIO_OLED, GPIO_PIN_4,GPIO_PIN_4)
#define OLED_RST_Clr() 	GPIOPinWrite(GPIO_OLED, GPIO_PIN_3,0)//RES
#define OLED_RST_Set() 	GPIOPinWrite(GPIO_OLED, GPIO_PIN_3,GPIO_PIN_3)
#define OLED_DC_Clr() 	GPIOPinWrite(GPIO_OLED, GPIO_PIN_2,0)//DC
#define OLED_DC_Set() 	GPIOPinWrite(GPIO_OLED, GPIO_PIN_2,GPIO_PIN_2)		     
#define OLED_CS_Clr() 	 ;//CS
#define OLED_CS_Set()  	 ;	     
#define OLED_BLK_Clr()   ;//BLK
#define OLED_BLK_Set()   ;

#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据


//画笔颜色
#define LCD_WHITE        0xFFFF
#define LCD_BLACK        0x0000	  
#define BLUE           	 0x001F  
#define BRED             0XF81F
#define GRED 			       0XFFE0
#define GBLUE			       0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			     0XBC40 //棕色
#define BRRED 			     0XFC07 //棕红色
#define GRAY  			     0X8430 //灰色
//GUI颜色
#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
//以上三色为PANEL的颜色 
#define LIGHTGREEN     	 0X841F //浅绿色
#define LGRAY 			     0XC618 //浅灰色(PANNEL),窗体背景色
#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)

#define USE_HORIZONTAL 2  //设置横屏或者竖屏显示 0或1为竖屏 2或3为横屏


#if USE_HORIZONTAL==0||USE_HORIZONTAL==1
#define LCD_W 135
#define LCD_H 240
#else
#define LCD_W 240
#define LCD_H 135
#endif


void LCD_Writ_Bus(u8 dat);
void LCD_WR_DATA8(u8 dat);
void LCD_WR_DATA(u16 dat);
void LCD_WR_REG(u8 dat);
void LCD_Address_Set(u16 x1,u16 y1,u16 x2,u16 y2);
void Lcd_Init(void); 
void ST7789_LCD_Clear(u16 Color);
void ST7789_LCD_ShowChinese(u16 x,u16 y,u8 index,u8 size,u16 color);
void ST7789_LCD_DrawPoint(u16 x,u16 y,u16 color);
void ST7789_LCD_DrawPoint_big(u16 x,u16 y,u16 colory);
void ST7789_LCD_Fill(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color);
void ST7789_LCD_DrawLine(u16 x1,u16 y1,u16 x2,u16 y2,u16 color);
void ST7789_LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2,u16 color);
void ST7789_Draw_Circle(u16 x0,u16 y0,u8 r,u16 color);
void ST7789_LCD_ShowChar(u16 x,u16 y,u8 num,u8 mode,u16 color);
void ST7789_LCD_ShowString(u16 x,u16 y,const u8 *p,u16 color);
u32 mypow(u8 m,u8 n);
void ST7789_LCD_ShowNum(u16 x,u16 y,u16 num,u8 len,u16 color);
void ST7789_LCD_ShowNum1(u16 x,u16 y,float num,u8 len,u16 color);
void ST7789_LCD_ShowPicture(u16 x1,u16 y1,u16 x2,u16 y2);
			

void st7789_write_6_8_string(uint8_t x,uint8_t y,uint8_t ch[]);
void st7789_write_6_8_number(unsigned char x,unsigned char y, float number);
void st7789_write_6_8_char(uint8_t x,uint8_t y,uint8_t ucData);

extern  u16 BACK_COLOR;   //背景色


#endif  
	 
	 



