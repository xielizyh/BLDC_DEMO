
/**
  ******************************************************************************
  * @file			main.c
  * @brief			main function
  * @author			Xli
  * @email			xieliyzh@163.com
  * @version		1.0.0
  * @date			2020-04-29
  * @copyright		2020, EVECCA Co.,Ltd. All rights reserved
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "app_debug.h"

/* Private constants ---------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function ----------------------------------------------------------*/

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init(168);
	//uart_init(115200);
    app_debug_init();

	while (1)
	{
		delay_ms(1000);
		//printf("hello!\r\n");
        RTU_LOGI("Info", "Hello world!");
        delay_ms(1000);
        RTU_LOGW("Warning", "Hello world!");
        delay_ms(1000);
        RTU_LOGE("Error", "Hello world!");
        delay_ms(1000);
        RTU_LOGD("Debug", "Hello world!");
	}
	
	//return 0;
}
