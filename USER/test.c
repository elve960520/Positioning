#include "sys.h"
#include "delay.h"
#include "usart.h" 
#include "led.h" 
#include "key.h"
#include "timer.h" 

extern u8  TIM5CH1_CAPTURE_STA;		//输入捕获状态		    				
extern u16	TIM5CH1_CAPTURE_VAL;	//输入捕获值
extern u8  TIM3CH1_CAPTURE_STA;		//输入捕获状态		    				
extern u16	TIM3CH1_CAPTURE_VAL;	//输入捕获值
int main(void)
{					   
	u32 temp=0;  	
	Stm32_Clock_Init(9);	//系统时钟设置
	uart_init(72,115200);	//串口初始化为115200
	delay_init(72);	   	 	//延时初始化
	
	LED_Init();		  		//初始化与LED连接的硬件接口
	KEY_Init();

 	TIM5_Cap_Init(0XFFFF,72-1);	//以1Mhz的频率计数 
	TIM3_Cap_Init(0XFFFF,72-1);	//以1Mhz的频率计数 
	
  while(1)
	{	 

 		if(TIM5CH1_CAPTURE_STA&0X80)//成功捕获到了一次高电平
		{
			temp=TIM5CH1_CAPTURE_STA&0X3F;
			temp*=65536;					//溢出时间总和
			temp+=TIM5CH1_CAPTURE_VAL;		//得到总的高电平时间
			printf("HIGH5:%d us\r\n",temp);	//打印总的高点平时间
			TIM5CH1_CAPTURE_STA=0;			//开启下一次捕获
		}
		else if(TIM3CH1_CAPTURE_STA&0X80)//成功捕获到了一次高电平
		{
			temp=TIM3CH1_CAPTURE_STA&0X3F;
			temp*=65536;					//溢出时间总和
			temp+=TIM3CH1_CAPTURE_VAL;		//得到总的高电平时间
			printf("HIGH3:%d us\r\n",temp);	//打印总的高点平时间
			TIM3CH1_CAPTURE_STA=0;			//开启下一次捕获
		}
	}		
}


