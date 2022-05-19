#ifndef _DS18B20_H
#define _DS18B20_H

#include "stm32f10x.h"

//DQ_GPIO
#define DQ_GPIO_CLK   RCC_APB2Periph_GPIOD
#define DQ_GPIO_Port  GPIOD
#define DQ_Pin        GPIO_Pin_6

/***************************** 延时函数 ************************************************************/

#include "core_delay.h"                            //包含内核延时头文件
#define  Delay_Us(us)  CPU_TS_Tmr_Delay_US(us)

/****************************************************************************************************/

//总线的电平
typedef enum 
{
	DQ_LEVEL_LOW = 0,
	DQ_LEVEL_HIGHT
}DQ_LEVEL_Type;

/* 引脚写高低电平 */
#define DQ_Write(Level)     if(Level) \
										GPIO_SetBits(DQ_GPIO_Port,DQ_Pin); \
						    else  \
										GPIO_ResetBits(DQ_GPIO_Port,DQ_Pin); 
//读取总线电平
#define DQ_Read        GPIO_ReadInputDataBit(DQ_GPIO_Port,DQ_Pin)												

//ROM指令
#define SKIP_ROM         0xCC     //跳过 ROM
#define CONVERT_TEMP     0x44    //转换温度
#define READ_SCRATCHPAD  0xBE   //读取暂存器内容
												
/*************************相关函数************************************************/	

void DQ_GPIO_Init(void);								
uint8_t Ds18b20_Init();
float Ds18b20_Get_Temp(void);												

#endif /* _DS18B20_H */
