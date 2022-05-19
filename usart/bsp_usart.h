#ifndef __USART_H
#define	__USART_H


#include "stm32f10x.h"
#include <stdio.h>

/** 
 * 编译器选择
 * 0： MDK (Keil)
 * 1： gcc (arm-none-eabi-gcc)
 */
#define MDK_IS_GCC    0

//是否使用中断
//0: 禁止中断   1: 使用中断
#define ENABLE_INTERROUT   0

	
// USART1
#define  DEBUG_USARTx                       USART1
#define  DEBUG_USART_CLK                    RCC_APB2Periph_USART1
#define  DEBUG_USART_APBxClkCmd             RCC_APB2PeriphClockCmd
#define  DEBUG_USART_BAUDRATE               115200

// USART GPIO 引脚宏定义
#define  DEBUG_USART_GPIO_CLK               (RCC_APB2Periph_GPIOA)
#define  DEBUG_USART_GPIO_APBxClkCmd        RCC_APB2PeriphClockCmd
    
#define  DEBUG_USART_TX_GPIO_PORT           GPIOA   
#define  DEBUG_USART_TX_GPIO_PIN            GPIO_Pin_9
#define  DEBUG_USART_RX_GPIO_PORT           GPIOA
#define  DEBUG_USART_RX_GPIO_PIN            GPIO_Pin_10

#if (ENABLE_INTERROUT == 1)
	  #define  DEBUG_USART_IRQ                USART1_IRQn
	  #define  DEBUG_USART_IRQHandler         USART1_IRQHandler
#endif


void USART_Config(void);
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch);
void Usart_SendString( USART_TypeDef * pUSARTx, char *str);
void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch);

#endif /* __USART_H */
