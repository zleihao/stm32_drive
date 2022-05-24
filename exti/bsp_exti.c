/**
 * @brief               bsp_exti.c
 * @Date:               2022.3.19
 * @Revision:           V1.0
 * @Writer:             点灯大师
 * @Affiliated unit：   黄河科技学院
 * @Email:              im_leihao@163.com
 * @github:             https://github.com/zleihao/stm32_drive.git
 * @note:               参考野火例程
 */
#include "bsp_exti.h"

 /**
  * @brief  配置嵌套向量中断控制器NVIC
  * @param  无
  * @retval 无
  */
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* 配置NVIC为优先级组1 */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  
  /* 配置中断源： */
  NVIC_InitStructure.NVIC_IRQChannel = Mod_INT_EXTI_IRQ;
  /* 配置抢占优先级 */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  /* 配置子优先级 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  /* 使能中断通道 */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

 /**
  * @brief  配置 IO为EXTI中断口，并设置中断优先级
  * @param  无
  * @retval 无
  * @note   触发外部中断为上升沿
  */
void EXTI_Mod_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	EXTI_InitTypeDef EXTI_InitStructure;

	/*开启按键GPIO口的时钟*/
	RCC_APB2PeriphClockCmd(Mod_INT_GPIO_CLK,ENABLE);
												
	/* 配置 NVIC 中断*/
	NVIC_Configuration();
	
/*--------------------------Mod配置-----------------------------*/
	/* 选择按键用到的GPIO */	
  GPIO_InitStructure.GPIO_Pin = Mod_INT_GPIO_PIN;
  /* 配置为浮空输入 */	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(Mod_INT_GPIO_PORT, &GPIO_InitStructure);

	/* 选择EXTI的信号源 */
  GPIO_EXTILineConfig(Mod_INT_EXTI_PORTSOURCE, Mod_INT_EXTI_PINSOURCE); 
  EXTI_InitStructure.EXTI_Line = Mod_INT_EXTI_LINE;
	
	/* EXTI为中断模式 */
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	#if (EDGE_TRIGGERED == 1)
		/* 上升沿中断 */
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  #else
		/* 下降沿中断 */
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;	
	#endif
	/* 使能中断 */	
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
}

/**
 * 中断函数示例 Example
 */
#define EXAMPLE 0

#if (EXAMPLE == 1)
void Mod_IRQHandler(void)
{
  //确保是否产生了EXTI Line中断
	if(EXTI_GetITStatus(Mod_INT_EXTI_LINE) != RESET) 
	{
		/*处理相关逻辑
		     .
		     .
		     .
		*/
    //清除中断标志位
		EXTI_ClearITPendingBit(Mod_INT_EXTI_LINE);     
	}  
}
#endif

/*********************************************END OF FILE**********************/
