#ifndef _PWM_H
#define _PWM_H

#include "stm32f10x.h"

//本例程所使用定时器为:TIM3，PWM输出通道为3
//若使用其他定时器或者通道，前往《STM32F103xCDE_数据手册-英文.pdf》第3章查询

//PWM通道GPIO
#define PWM_CHENNAL_GPIO_PIN	          GPIO_Pin_0
#define PWM_CHENNAL_GPIO_PORT	          GPIOB
#define PWM_CHENNAL_GPIO_CLK            RCC_APB2Periph_GPIOB

//定时器
#define TIMx                            TIM3
#define TIMx_APBxPeriphClock            RCC_APB1Periph_TIM3

#define TIMx_APBxPeriphClock_FUN        RCC_APB1PeriphClockCmd
//PWM通道选择，将下面改成对应的值即可
/*  
    通道1：TIM_OC1Init、TIM_SetCompare1
		通道2：TIM_OC2Init、TIM_SetCompare2
		通道3：TIM_OC3Init、TIM_SetCompare3
		通道4：TIM_OC4Init、TIM_SetCompare4 
*/
#define PWM_CHENNALx_INIT                TIM_OC3Init    
#define PWM_CHENNALx_SET_DUTY            TIM_SetCompare3

//舵机周期 20ms
//周期计算方法：T = Period*Prescaler / 72M
#define TIMx_PERIOD          (200-1)
#define TIMx_PRESCALER       (7200-1)

//函数接口
void Servo_Config(void);
void Servo_Angle(uint16_t angle);

#endif
