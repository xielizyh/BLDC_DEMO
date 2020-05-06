
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

	uint16_t cnt_1s = 0;
    while (1)
	{
        app_debug_task(NULL);

		delay_ms(10);

        if (++cnt_1s >= 1000)
        {
            cnt_1s = 0;
            RTU_LOGI("Info", "Hello world!");
            // RTU_LOGW("Warning", "Hello world!");
            // RTU_LOGE("Error", "Hello world!");
            // RTU_LOGD("Debug", "Hello world!");            
        }
	}
	
	//return 0;
}
