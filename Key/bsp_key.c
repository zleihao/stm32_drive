/**
 * @brief                   bsp_key.c
 * @Date:                   2022.3.19
 * @Revision:               V1.0
 * @Writer:                 点灯大师
 * @Affiliated unit：       黄河科技学院
 * @Email:                  im_leihao@163.com
 * @gitee:                  https://gitee.com/zleihao/st_stm32_drive.git
 * @note:                   参考野火例程
 */
  
#include "bsp_key.h"  

/**
  * @brief  Key_GPIO_Config 
  * @param  无
  * @return 无
	* @note   配置按键用到的I/O口
  */
void Key_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/*开启按键端口的时钟*/
	RCC_APB2PeriphClockCmd(KEY1_GPIO_CLK|KEY2_GPIO_CLK,ENABLE);
	
	//选择按键的引脚
	GPIO_InitStructure.GPIO_Pin = KEY1_GPIO_PIN; 
	// 设置按键的引脚为浮空输入
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	//使用结构体初始化按键
	GPIO_Init(KEY1_GPIO_PORT, &GPIO_InitStructure);
	
	//选择按键的引脚
	GPIO_InitStructure.GPIO_Pin = KEY2_GPIO_PIN; 
	//设置按键的引脚为浮空输入
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	//使用结构体初始化按键
	GPIO_Init(KEY2_GPIO_PORT, &GPIO_InitStructure);	
}

/**
 * @brief    Key_Scan
 * @param    GPIOx：x 可以是 A，B，C，D或者 E
   		       GPIO_Pin：待读取的端口位 	
 * @return   KEY_OFF(没按下按键)、KEY_ON（按下按键）
 * @note     检测是否有按键按下
 */
uint8_t Key_Scan(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin)
{			
	/*检测是否有按键按下 */
	if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON )  
	{	 
		/*等待按键释放 */
		while(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON);   
		return 	KEY_ON;	 
	}
	else
		return KEY_OFF;
}
/*********************************************END OF FILE**********************/
