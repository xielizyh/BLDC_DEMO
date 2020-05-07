/**
  ******************************************************************************
  * @file			bsp_systick.c
  * @brief			bsp_systick function
  * @author			Xli
  * @email			xieliyzh@163.com
  * @version		1.0.0
  * @date			2020-05-07
  * @copyright		2020, EVECCA Co.,Ltd. All rights reserved
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h" 
#include "bsp_systick.h"

/* Private constants ---------------------------------------------------------*/
#define CONFIG_SYSCLK_HZ     168     /*!< 系统时钟源频率：168M */
#define CONFIG_TICK_RATE_HZ  1000    /*!< 滴答时钟节拍：1K=1ms @note:reload为24位，频率不要小于约2Hz */
#define TICK_PERIOD_MS       (1000 / CONFIG_TICK_RATE_HZ)   /*!< 每个时钟节拍的时间(ms) */

/* Private macro -------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static uint32_t systick_cnt;   /*!< 系统节拍 */
static bsp_systick_callback systick_cb;

/* Private function ----------------------------------------------------------*/

/**=============================================================================
 * @brief           系统时间获取
 *
 * @param[in]       none
 *
 * @return          ms
 *============================================================================*/
uint32_t sys_time_get(void)
{
    return systick_cnt * TICK_PERIOD_MS;
}

/**=============================================================================
 * @brief           滴答定时器中断
 *
 * @param[in]       none
 *
 * @return          none
 *============================================================================*/
void systick_handler(void)
{	
	systick_cnt++;

    /* 回调 */
    systick_cb();
}

/**=============================================================================
 * @brief           系统时基初始化
 *
 * @param[in]       none
 *
 * @return          none
 *============================================================================*/
void bsp_systick_init(bsp_systick_callback cb)
{
    uint32_t reload = 0;

    /* 滴答时钟源 */
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);   /*!< 外部时钟 */

    reload = (CONFIG_SYSCLK_HZ / 8) * 1000000 / CONFIG_TICK_RATE_HZ;

	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;   	/*!< 开启SYSTICK中断 */ 
	SysTick->LOAD = reload; 				/*!< 每1/CONFIG_TICK_RATE_HZ秒中断一次 */
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; 	/* 开启SYSTICK */  

    systick_cb = cb;
}
