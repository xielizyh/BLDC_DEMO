
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
#include <stdio.h>
#include "stm32f4xx.h" 
#include "app_debug.h"
#include "app_task.h"
#include "app_key.h"
#include "app_motor.h"

/* Private constants ---------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function ----------------------------------------------------------*/
#if 0
__asm void wait()
{
delay
    ldr r0, =0xFFFFFF 
delay_loop
    sub r0, r0, #1
    mov r2, #0
    cmp r0, r2 
    bne delay_loop 
    mov pc, lr
}
#endif

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    systick_init();
    app_debug_init();
    app_key_init();
    app_motor_init();

    RTU_LOGI(LOG_TAG, "system start to run...");

    while (1)
	{
        app_task_poll();
	}
	
	//return 0;
}
