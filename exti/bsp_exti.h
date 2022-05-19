#ifndef __EXTI_H
#define	__EXTI_H


#include "stm32f10x.h"

/**
 *	外部中断,边沿电平选择
 *  1: 上升沿触发
 *  0: 
 */
#define EDGE_TRIGGERED      1

//引脚定义
#define Mod_INT_GPIO_PORT         GPIOA
#define Mod_INT_GPIO_CLK          (RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO)
#define Mod_INT_GPIO_PIN          GPIO_Pin_0
//外部中断端口
#define Mod_INT_EXTI_PORTSOURCE   GPIO_PortSourceGPIOA //与GPIO端口一致
#define Mod_INT_EXTI_PINSOURCE    GPIO_PinSource0      //与GPIO引脚号一致
#define Mod_INT_EXTI_LINE         EXTI_Line0           //与GPIO引脚号一致

/**
 * 中断端口分为三类：
 * 1、EXTIx_IRQn     其中x = 0、1、2、3、4，对应GPIO的引脚
 * 2、EXTI9_5_IRQn   包括GPIO引脚的5~9
 * 3、EXTI15_10_IRQn  包括GPIO引脚的10~15
 */
#define Mod_INT_EXTI_IRQ          EXTI0_IRQn          //与GPIO引脚一致
/**
 * 中断函数入口分为三类：
 * 1、EXTIx_IRQHandler     其中x = 0、1、2、3、4，对应GPIO的引脚
 * 2、EXTI9_5_IRQHandler   包括GPIO引脚的5~9
 * 3、EXTI15_10_IRQHandler 包括GPIO引脚的10~15
 */
#define Mod_IRQHandler            EXTI0_IRQHandler    //与GPIO引脚一致


void EXTI_Mod_Config(void);


#endif /* __EXTI_H */
