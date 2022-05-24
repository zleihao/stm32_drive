/**
 * @brief:              RTC 时钟驱动
 * @author：            点灯大师
 * @Affiliated unit：   黄河科技学院
 * @mail：              im_leiaho@163.com
 * @gitee:              https://github.com/zleihao/stm32_drive.git
 * @note                
 */

#include "bsp_SysTick.h"
#include "bsp_debug_usart.h"
#include "rtc.h" 		  

_calendar_obj calendar;//时钟结构体 

/**
 * @brief  配置 rtc 中断 
 * @param  null
 * @return null 
 */   
static void RTC_nvic_config(void) {
	NVIC_InitTypeDef NVIC_InitStruct;

	//配置中断分组
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	NVIC_InitStruct.NVIC_IRQChannel = RTC_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&NVIC_InitStruct);
}

/**
 * @brief   RTC 初始化
 * @param   null
 * @return  0：初始化成功  
 *          1：初始化失败，晶振有问题
 */   
uint8_t RTC_Init(void) {
	//检查是不是第一次配置时钟
	uint8_t temp = 0;

	if(BKP->DR1 != 0x5050) {	 //第一次配置
	  	RCC->APB1ENR |= 1<<28;     //使能电源时钟	    
		RCC->APB1ENR |= 1<<27;     //使能备份时钟	    
		PWR->CR |= 1<<8;           //取消备份区写保护
		RCC->BDCR |= 1<<16;        //备份区域软复位	   
		RCC->BDCR &= ~(1<<16);     //备份区域软复位结束	  	 
	    RCC->BDCR |= 1<<0;         //开启外部低速振荡器 

	    while((!(RCC->BDCR & 0x02)) && temp < 250) { //等待外部时钟就绪
			temp++;
			Delay_us(10000);
		}
		
		if (temp >= 250) {
			return 1;//初始化时钟失败,晶振有问题	   
		}

		RCC->BDCR |= 1<<8; //LSI作为RTC时钟 	    
		RCC->BDCR |= 1<<15;//RTC时钟使能	

	  	while(!(RTC->CRL & (1<<5)));//等待RTC寄存器操作完成	 
    	while(!(RTC->CRL & (1<<3)));//等待RTC寄存器同步  
    	
		RTC->CRH |= 0x01;  		  //允许秒中断
    	RTC->CRH |= 0x02;  		  //允许闹钟中断
    	while(!(RTC->CRL&(1<<5)));//等待RTC寄存器操作完成	 
		
		RTC->CRL |= 1<<4;           //允许配置	 
		RTC->PRLH = 0x0000;
		RTC->PRLL = 32767;          //时钟周期设置(有待观察,看是否跑慢了?)理论值：32767	
		
		RTC_Set(2015,1,14,19,11,55); //设置时间	  

		RTC->CRL &= ~(1<<4);           //配置更新
		while(!(RTC->CRL&(1<<5)));   //等待RTC寄存器操作完成		 									  
	
		BKP->DR1 = 0x5050;  
	 	printf("FIRST TIME\n");
	}
	else {                              //系统继续计时
    	while(!(RTC->CRL&(1<<3)));      //等待RTC寄存器同步  
    	RTC->CRH |= 0x01;  		        //允许秒中断
    	while( !(RTC->CRL & (1<<5)) );  //等待RTC寄存器操作完成

		printf("OK\n");
	}		    				  
	RTC_nvic_config();//优先级设置

	RTC_Get();//更新时间 
	
	return 0; //ok
}	

/**
 * @brief  RTC时钟中断,每秒触发一次  
 * @param  NULL
 * @return NULL 
 */
void RTC_IRQHandler(void) {		 
	if(RTC->CRL & 0x0001)	{ //秒钟中断
		RTC_Get();				//更新时间   
		//printf("sec:%d\r\n",calendar.sec);
 	}
	if(RTC->CRL&0x0002) { //闹钟中断
		RTC->CRL &= ~(0x0002);	//清闹钟中断	  
		RTC_Get();				//更新时间   
  		printf("Alarm Time:%d-%d-%d %d:%d:%d\n",calendar.w_year,calendar.w_month,calendar.w_date,calendar.hour,calendar.min,calendar.sec);//输出闹铃时间	   
  	} 				  								 
    RTC->CRL &= 0x0FFA;         	//清除溢出，秒钟中断标志

	while(!(RTC->CRL&(1<<5)));	//等待RTC寄存器操作完成	  					 	   	 
}

/**
 * @brief   判断某年是否是闰年函数
 * @param   yaer: 年份
 * @return  1：是闰年  0：不是闰年
 */
uint8_t Is_Leap_Year(uint16_t year) {			  
	if(year%4 == 0) { //必须能被4整除
		if(year%100 == 0) { 
			if(year%400 == 0) {
				return 1;//如果以00结尾,还要能被400整除 	   
			} else { 
				return 0;
			}   
		} else {
			return 1;   
		}
	} else {
		return 0;
	}	
}	 	
								 
uint8_t const table_week[12]={0,3,3,6,1,4,6,2,5,0,3,5}; //月修正数据表	  
//平年的月份日期表
const uint8_t mon_table[12]={31,28,31,30,31,30,31,31,30,31,30,31};

/**
 * @brief   设置 RTC 的年、月、日、时、分、秒 RTC_Set
 * @param   syear: 年  smon: 月  sday: 日  
 *          hour: 时   min: 分   sec: 秒
 * @return  0: 设置成功 
 *          1: 设置失败    
 */
uint8_t RTC_Set( uint16_t syear,  uint8_t smon,   uint8_t sday,
                 uint8_t hour,    uint8_t min,    uint8_t sec ) 
{
	uint16_t t;
	uint32_t seccount=0;
	if (syear<1970 || syear>2099) {
		return 1;	   
	}
	for (t=1970;t<syear;t++) { //把所有年份的秒钟相加
		if(Is_Leap_Year(t))seccount+=31622400;//闰年的秒钟数
		else seccount+=31536000;			  //平年的秒钟数
	}
	smon -= 1;

	for(t=0;t<smon;t++) {	//把前面月份的秒钟数相加
		seccount+=(uint32_t)mon_table[t]*86400;//月份秒钟数相加
		if(Is_Leap_Year(syear) && t==1) {
			seccount += 86400;//闰年2月份增加一天的秒钟数	   
		}
	}
	seccount += (uint32_t)(sday-1)*86400;	//把前面日期的秒钟数相加 
	seccount += (uint32_t)hour*3600;		//小时秒钟数
    seccount += (uint32_t)min*60;	 		//分钟秒钟数
	seccount += sec;						//最后的秒钟加上去
													    
	//设置时钟
    RCC->APB1ENR |= 1<<28;//使能电源时钟
    RCC->APB1ENR |= 1<<27;//使能备份时钟
	PWR->CR |= 1<<8;    //取消备份区写保护
	
	//上面三步是必须的!
	RTC->CRL |= 1<<4;   //允许配置 
	RTC->CNTL = seccount&0xffff;
	RTC->CNTH = seccount>>16;
	RTC->CRL &= ~(1<<4);//配置更新
	while(!(RTC->CRL&(1<<5)));//等待RTC寄存器操作完成 

	RTC_Get();//设置完之后更新一下数据 	
	
	return 0;	    
}

/**
 * @brief   设置闹钟,1970~2099年为合法年份
 * @param   syear：年  smon：月  sday：日
 *          hour：时   min：分   sec: 秒
 * @return  0: 成功
 *          其他: 错误代码.
 */
uint8_t RTC_Alarm_Set( uint16_t syear,  uint8_t smon,  uint8_t sday,
                       uint8_t hour,    uint8_t min,   uint8_t sec )
{
	uint16_t t;
	uint32_t seccount=0;

	if (syear<1970 || syear>2099) {
		return 1;
	}	   

	for (t=1970;t<syear;t++) {	//把所有年份的秒钟相加
		if(Is_Leap_Year(t)) {
			seccount += 31622400;//闰年的秒钟数
		} else {
			seccount += 31536000;			  //平年的秒钟数
		}
	}
	smon-=1;
	for(t=0;t<smon;t++)	   //把前面月份的秒钟数相加
	{
		seccount += (uint32_t)mon_table[t]*86400;//月份秒钟数相加
		if(Is_Leap_Year(syear)&&t==1) {
			seccount+=86400;//闰年2月份增加一天的秒钟数	   
		}
	}
	seccount += (uint32_t)(sday-1)*86400;//把前面日期的秒钟数相加 
	seccount += (uint32_t)hour*3600;//小时秒钟数
    seccount += (uint32_t)min*60;	 //分钟秒钟数
	seccount += sec;//最后的秒钟加上去 			    
	//设置时钟
    RCC->APB1ENR |= 1<<28;//使能电源时钟
    RCC->APB1ENR |= 1<<27;//使能备份时钟
	PWR->CR|=1<<8;    //取消备份区写保护
	//上面三步是必须的!
	RTC->CRL|=1<<4;   //允许配置 
	RTC->ALRL=seccount&0xffff;
	RTC->ALRH=seccount>>16;
	RTC->CRL&=~(1<<4);//配置更新
	while(!(RTC->CRL&(1<<5)));//等待RTC寄存器操作完成  
	return 0;	    
}


/**
 * @brief   得到当前的时间，结果保存在calendar结构体里面
 * @param   NULL
 * @return  0,成功  
 *          其他:错误代码.
 */
uint8_t RTC_Get(void) {
	static uint16_t daycnt=0;
	uint32_t timecount = 0; 
	uint32_t temp = 0;
	uint16_t temp1 = 0;

 	timecount = RTC->CNTH;//得到计数器中的值(秒钟数)
	timecount <<= 16;
	timecount += RTC->CNTL;			 

 	temp=timecount/86400;   //得到天数(秒钟数对应的)
	if(daycnt!=temp)//超过一天了
	{	  
		daycnt=temp;
		temp1=1970;	//从1970年开始
		while(temp>=365)
		{				 
			if(Is_Leap_Year(temp1))//是闰年
			{
				if(temp>=366)temp-=366;//闰年的秒钟数
				else break;  
			}
			else temp-=365;	  //平年 
			temp1++;  
		}   
		calendar.w_year=temp1;//得到年份
		temp1=0;
		while(temp>=28)//超过了一个月
		{
			if(Is_Leap_Year(calendar.w_year)&&temp1==1)//当年是不是闰年/2月份
			{
				if(temp>=29)temp-=29;//闰年的秒钟数
				else break; 
			}
			else 
			{
				if(temp>=mon_table[temp1])temp-=mon_table[temp1];//平年
				else break;
			}
			temp1++;  
		}
		calendar.w_month=temp1+1;	//得到月份
		calendar.w_date=temp+1;  	//得到日期 
	}
	temp=timecount%86400;     		//得到秒钟数   	   
	calendar.hour=temp/3600;     	//小时
	calendar.min=(temp%3600)/60; 	//分钟	
	calendar.sec=(temp%3600)%60; 	//秒钟
	calendar.week=RTC_Get_Week(calendar.w_year,calendar.w_month,calendar.w_date);//获取星期   
	return 0;
}	 

/**
 * @brief  输入公历日期得到星期(只允许1901-2099年)
 * @param  year: 公历年  month: 公历月  day: 公历日
 * @return 对应的星期号
 */																					 
uint8_t RTC_Get_Week(uint16_t year,uint8_t month,uint8_t day) {	
	uint16_t temp2;
	uint8_t yearH,yearL;
	
	yearH = year / 100;	
	yearL = year % 100; 
	// 如果为21世纪,年份数加100  
	if (yearH>19) {
		yearL+=100;
	}
	// 所过闰年数只算1900年之后的  
	temp2 = yearL + yearL/4;
	temp2 = temp2 % 7; 
	temp2 = temp2 + day + table_week[month-1];
	if (yearL%4 == 0 && month<3) {
		temp2--;
	}
	return(temp2%7);
}			  
















