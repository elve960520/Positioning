#include "timer.h"
#include "led.h"
 	 
void TIM5_Cap_Init(u16 arr,u16 psc)
{		 
	RCC->APB1ENR|=1<<3;   	//TIM5 时钟使能 
 	TIM5->ARR=arr;  		//设定计数器自动重装值   
	TIM5->PSC=psc;  		//预分频器 
	TIM5->CCMR1|=1<<0;		//CC1S=01 	选择输入端 IC1映射到TI1上
 	TIM5->CCMR1|=0<<4; 		//IC1F=0000 配置输入滤波器 不滤波
 	TIM5->CCMR1|=0<<10; 	//IC2PS=00 	配置输入分频,不分频 
	TIM5->CCER|=0<<1; 		//CC1P=0	上升沿捕获
	TIM5->CCER|=1<<0; 		//CC1E=1 	允许捕获计数器的值到捕获寄存器中
	TIM5->DIER|=1<<1;   	//允许捕获中断				
	TIM5->DIER|=1<<0;   	//允许更新中断	
	TIM5->CR1|=0x01;    	//使能定时器2
	MY_NVIC_Init(2,0,TIM5_IRQn,2);//抢占2，子优先级0，组2	   
}

void TIM3_Cap_Init(u16 arr,u16 psc)
{		 
	RCC->APB1ENR|=1<<1;   	//TIM4 时钟使能   
 	TIM3->ARR=arr;  		//设定计数器自动重装值   
	TIM3->PSC=psc;  		//预分频器 
	TIM3->CCMR1|=1<<0;		//CC1S=01 	选择输入端 IC1映射到TI1上
 	TIM3->CCMR1|=0<<4; 		//IC1F=0000 配置输入滤波器 不滤波
 	TIM3->CCMR1|=0<<10; 	//IC2PS=00 	配置输入分频,不分频 
	TIM3->CCER|=0<<1; 		//CC1P=0	上升沿捕获
	TIM3->CCER|=1<<0; 		//CC1E=1 	允许捕获计数器的值到捕获寄存器中
	TIM3->DIER|=1<<1;   	//允许捕获中断				
	TIM3->DIER|=1<<0;   	//允许更新中断	
	TIM3->CR1|=0x01;    	//使能定时器2
	MY_NVIC_Init(2,2,TIM3_IRQn,2);//抢占2，子优先级0，组2	   
}

//捕获状态
//[7]:0,没有成功的捕获;1,成功捕获到一次.
//[6]:0,还没捕获到高电平;1,已经捕获到高电平了.
//[5:0]:捕获高电平后溢出的次数
u8  TIM5CH1_CAPTURE_STA=0;	//输入捕获状态		    				
u16	TIM5CH1_CAPTURE_VAL;	//输入捕获值
//定时器5中断服务程序	 
void TIM5_IRQHandler(void)  //TIM3_IRQHandler
{ 		    
	u16 tsr;
	tsr=TIM5->SR;
 	if((TIM5CH1_CAPTURE_STA&0X80)==0)//还未成功捕获	
	{
		if(tsr&0X01)//溢出
		{	    
			if(TIM5CH1_CAPTURE_STA&0X40)//已经捕获到高电平了
			{
				if((TIM5CH1_CAPTURE_STA&0X3F)==0X3F)//高电平太长了
				{
					TIM5CH1_CAPTURE_STA|=0X80;//标记成功捕获了一次
					TIM5CH1_CAPTURE_VAL=0XFFFF;
				}else TIM5CH1_CAPTURE_STA++;
			}	 
		}
		if(tsr&0x02)//捕获1发生捕获事件
		{	
			if(TIM5CH1_CAPTURE_STA&0X40)		//捕获到一个下降沿 		
			{	  			
				TIM5CH1_CAPTURE_STA|=0X80;		//标记成功捕获到一次高电平脉宽
			  TIM5CH1_CAPTURE_VAL=TIM5->CCR1;	//获取当前的捕获值.
	 			TIM5->CCER&=~(1<<1);			//CC1P=0 设置为上升沿捕获
			}else  								//还未开始,第一次捕获上升沿
			{
				TIM5CH1_CAPTURE_STA=0;			//清空
				TIM5CH1_CAPTURE_VAL=0;
				TIM5CH1_CAPTURE_STA|=0X40;		//标记捕获到了上升沿
	 			TIM5->CNT=0;					//计数器清空
	 			TIM5->CCER|=1<<1; 				//CC1P=1 设置为下降沿捕获
			}		    
		}			     	    					   
 	}
	TIM5->SR=0;//清除中断标志位 	    
}

//捕获状态
//[7]:0,没有成功的捕获;1,成功捕获到一次.
//[6]:0,还没捕获到高电平;1,已经捕获到高电平了.
//[5:0]:捕获高电平后溢出的次数
u8  TIM3CH1_CAPTURE_STA=0;	//输入捕获状态		    				
u16	TIM3CH1_CAPTURE_VAL;	//输入捕获值
//定时器5中断服务程序	 
void TIM3_IRQHandler(void)
{ 		    
	u16 tem;
	tem=TIM3->SR;
 	if((TIM3CH1_CAPTURE_STA&0X80)==0)//还未成功捕获	
	{
		if(tem&0X01)//溢出
		{	    
			if(TIM3CH1_CAPTURE_STA&0X40)//已经捕获到高电平了
			{
				if((TIM3CH1_CAPTURE_STA&0X3F)==0X3F)//高电平太长了
				{
					TIM3CH1_CAPTURE_STA|=0X80;//标记成功捕获了一次
					TIM3CH1_CAPTURE_VAL=0XFFFF;
				}else TIM3CH1_CAPTURE_STA++;
			}	 
		}
		if(tem&0x02)//捕获1发生捕获事件
		{	
			if(TIM3CH1_CAPTURE_STA&0X40)		//捕获到一个下降沿 		
			{	  			
				TIM3CH1_CAPTURE_STA|=0X80;		//标记成功捕获到一次高电平脉宽
			  TIM3CH1_CAPTURE_VAL=TIM3->CCR1;	//获取当前的捕获值.
	 			TIM3->CCER&=~(1<<1);			//CC1P=0 设置为上升沿捕获
			}else  								//还未开始,第一次捕获上升沿
			{
				TIM3CH1_CAPTURE_STA=0;			//清空
				TIM3CH1_CAPTURE_VAL=0;
				TIM3CH1_CAPTURE_STA|=0X40;		//标记捕获到了上升沿
	 			TIM3->CNT=0;					//计数器清空
	 			TIM3->CCER|=1<<1; 				//CC1P=1 设置为下降沿捕获
			}		    
		}			     	    					   
 	}
	TIM3->SR=0;//清除中断标志位 	    
}











