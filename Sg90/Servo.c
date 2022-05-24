/**
 * @brief                   Servo.c
 * @Date:                   2022.3.27
 * @Revision:               V1.0
 * @Writer:                 点灯大师
 * @Affiliated unit：       黄河科技学院
 * @Email:                  im_leihao@163.com
 * @github:              https://github.com/zleihao/stm32_drive.git
 * @note:                   舵机Sg90驱动
 */

#include "Servo.h"

/**
 * @brief   TIM_GPIO_Config
 * @param   无
 * @return  无
 * @note    初始化PWM输出通道，输出模式配置成复用推挽
 */
static void TIM_GPIO_Config(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
	//打开时钟
	RCC_APB2PeriphClockCmd(PWM_CHENNAL_GPIO_CLK | RCC_APB2Periph_AFIO,ENABLE);
	
	//通道IO口初始化
	GPIO_InitStructure.GPIO_Pin = PWM_CHENNAL_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(PWM_CHENNAL_GPIO_PORT,&GPIO_InitStructure);
}

/**
 * @brief  TIM_Mode_Config
 * @param  无
 * @return 无
 * @note   PWM的输出方式
 */
static void TIM_Mode_Config() {
	
	
	TIM_TimeBaseInitTypeDef TIM_InitStructure;
	TIM_OCInitTypeDef PWM_InitStructure;
	
	//打开定时器时钟
	TIMx_APBxPeriphClock_FUN(TIMx_APBxPeriphClock,ENABLE);
	
	/* 周期计算方法：T = Period*Prescaler / 72M */
	TIM_InitStructure.TIM_Period = TIMx_PERIOD;
	TIM_InitStructure.TIM_Prescaler = TIMx_PRESCALER;
	
	TIM_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_InitStructure.TIM_RepetitionCounter = DISABLE;
	TIM_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIMx,&TIM_InitStructure);
	//PWM输出模式
	PWM_InitStructure.TIM_OCMode = TIM_OCMode_PWM2;
	PWM_InitStructure.TIM_OutputState = TIM_OutputState_Enable;
	PWM_InitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	//初始状态占空比为0
	PWM_InitStructure.TIM_Pulse = 0;
	//对应通道初始化
	PWM_CHENNALx_INIT(TIMx,&PWM_InitStructure);
	//使能定时器
	TIM_Cmd(TIMx,ENABLE);

}

/**
 * @brief  Servo_Config
 * @param  无
 * @return 无
 * @note   SG90初始化
 */
void Servo_Config(void) {
    TIM_GPIO_Config();
  	TIM_Mode_Config();
}

/**
 * @brief  Servo_Angle
 * @param  angle：取值范围 0~180
 * @return 无
 * @note   SG90转动相应的角度
 */
void Servo_Angle(uint16_t angle) {
	
	static uint8_t i = 0;
	i = (1.0 / 9.0) * angle + 5.0;//寄存器值 = 1/9 * 角度 + 5
	PWM_CHENNALx_SET_DUTY(TIMx,i);
}
