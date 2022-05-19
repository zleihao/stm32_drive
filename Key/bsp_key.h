#ifndef __KEY_H
#define	__KEY_H

#include "stm32f10x.h"

//  引脚定义
#define    KEY1_GPIO_CLK     RCC_APB2Periph_GPIOA
#define    KEY1_GPIO_PORT    GPIOA			   
#define    KEY1_GPIO_PIN		 GPIO_Pin_0

#define    KEY2_GPIO_CLK     RCC_APB2Periph_GPIOC
#define    KEY2_GPIO_PORT    GPIOC		   
#define    KEY2_GPIO_PIN		 GPIO_Pin_13



 /** 
	*  按键按下为高电平，设置 KEY_LEVEL = 1
	*  按键按下为低电平，设置 KEY_LEVEL = 0
	*/
#define KEY_LEVEL 0


#if (KEY_LEVEL == 1)
	  #define KEY_ON	1
	  #define KEY_OFF	0
#else
	  #define KEY_ON	0
	  #define KEY_OFF	1
#endif


void Key_GPIO_Config(void);
uint8_t Key_Scan(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin);


#endif /* __KEY_H */

