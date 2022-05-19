#ifndef __BSP_MLX90614_H__
#define __BSP_MLX90614_H__

#include "stm32f10x.h"

//----------------------------------------------------------------------------//
#define MLX90614_I2C_SCL_RCC     RCC_APB2Periph_GPIOB
#define MLX90614_I2C_SCL_PIN	   GPIO_Pin_10		/* 连接到SCL时钟线的GPIO */
#define MLX90614_I2C_SCL_PORT    GPIOB

#define MLX90614_I2C_SDA_RCC     RCC_APB2Periph_GPIOB
#define MLX90614_I2C_SDA_PIN	   GPIO_Pin_11			/* 连接到SDA数据线的GPIO */
#define MLX90614_I2C_SDA_PORT    GPIOB


/* 定义读写SCL和SDA的宏 */
#define MLX90614_I2C_SCL_1()     MLX90614_I2C_SCL_PORT->BSRR = MLX90614_I2C_SCL_PIN			/* SCL = 1 */
#define MLX90614_I2C_SCL_0()     MLX90614_I2C_SCL_PORT->BRR  = MLX90614_I2C_SCL_PIN			/* SCL = 0 */

#define MLX90614_I2C_SDA_1()     MLX90614_I2C_SDA_PORT->BSRR = MLX90614_I2C_SDA_PIN			/* SDA = 1 */
#define MLX90614_I2C_SDA_0()     MLX90614_I2C_SDA_PORT->BRR  = MLX90614_I2C_SDA_PIN			/* SDA = 0 */

#define MLX90614_I2C_SDA_READ()  ((MLX90614_I2C_SDA_PORT->IDR & MLX90614_I2C_SDA_PIN) != 0)	/* 读SDA口线状态 */
#define MLX90614_I2C_SCL_READ()  ((MLX90614_I2C_SCL_PORT->IDR & MLX90614_I2C_SCL_PIN) != 0)	/* 读SCL口线状态 */
/*******************************************************************************************************************/

#define MLX90614_TEMP_READ_ERR_CODE -99

/* MLX90614 内部寄存器地址定义 */
/* Private defines -----------------------------------------------------------*/
/* DEFAULT SLAVE ADDRESS */
#define MLX90614_ADDR       0x5A
#define MLX90614_ADDR_WR   (MLX90614_ADDR << 1)

/* OPCODE DEFINES */
#define MLX90614_OP_RAM		0x00
#define MLX90614_OP_EEPROM	0x20
#define MLX90614_OP_SLEEP	0xFF

/* RAM offsets with 16-bit data, MSB first */
#define MLX90614_RAW1		(MLX90614_OP_RAM | 0x04) /* raw data IR channel 1 */
#define MLX90614_RAW2		(MLX90614_OP_RAM | 0x05) /* raw data IR channel 2 */
#define MLX90614_TAMB 		(MLX90614_OP_RAM | 0x06) /* ambient temperature */
#define MLX90614_TOBJ1 		(MLX90614_OP_RAM | 0x07) /* object 1 temperature */
#define MLX90614_TOBJ2 		(MLX90614_OP_RAM | 0x08) /* object 2 temperature */
/* EEPROM offsets with 16-bit data, MSB first */
#define MLX90614_TOMIN 		(MLX90614_OP_EEPROM | 0x00) /* object temperature min register */
#define MLX90614_TOMAX 		(MLX90614_OP_EEPROM | 0x01) /* object temperature max register */
#define MLX90614_PWMCTRL 	(MLX90614_OP_EEPROM | 0x02) /* pwm configuration register */
#define MLX90614_TARANGE 	(MLX90614_OP_EEPROM | 0x03) /* ambient temperature register */
#define MLX90614_EMISSIVITY (MLX90614_OP_EEPROM | 0x04) /* emissivity correction register */
#define MLX90614_CFG1 		(MLX90614_OP_EEPROM | 0x05) /* configuration register */
#define MLX90614_SA 		(MLX90614_OP_EEPROM | 0x0E) /* slave address register */
#define MLX90614_ID1 		(MLX90614_OP_EEPROM | 0x1C) /*[read-only] 1 ID register */
#define MLX90614_ID2 		(MLX90614_OP_EEPROM | 0x1D) /*[read-only] 2 ID register */
#define MLX90614_ID3 		(MLX90614_OP_EEPROM | 0x1E) /*[read-only] 3 ID register */
#define MLX90614_ID4 		(MLX90614_OP_EEPROM | 0x1F) /*[read-only] 4 ID register */

#define MLX90614_DBG_OFF    0
#define MLX90614_DBG_ON     1
#define MLX90614_DBG_MSG_W  0
#define MLX90614_DBG_MSG_R  1


/* MLX90614 供外部调用函数 */
int8_t bsp_Mlx90614Init(void);
int8_t bsp_Mlx90614ScanDevice(uint8_t *pSlaveAddr);
float bsp_Mlx90614ReadTemp(uint8_t devAddr);


#endif
/***************************** (END OF FILE) **********************************/
