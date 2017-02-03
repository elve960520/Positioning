#include "key.h"
#include "delay.h"
								    
void KEY_Init(void)
{
	RCC->APB2ENR|=1<<2;     
	RCC->APB2ENR|=1<<3;    
	GPIOA->CRL&=0X00FFFF00;	  
	GPIOA->CRL|=0X88000088; 
	GPIOB->CRL&=0X00FFFFFF;  
	GPIOB->CRL|=0X88000000; 				   
} 



















