/**
  ******************************************************************************
  * @file			app_task.c
  * @brief			app_task function
  * @author			Xli
  * @email			xieliyzh@163.com
  * @version		1.0.0
  * @date			2020-05-07
  * @copyright		2020, EVECCA Co.,Ltd. All rights reserved
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "app_task.h"
#include "bsp_systick.h"
#include "app_debug.h"
#include "app_key.h"

/* Private constants ---------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define _ARRAY_SIZE(x) 	(sizeof(x)/sizeof((x)[0]))

/* Private typedef -----------------------------------------------------------*/
/**
* @brief 任务块
*/
typedef struct {
    uint8_t is_run;		/*!< 是否运行 */
    uint16_t slice_cnt;  /*!< 任务时间片计数 */
    uint16_t peroid;    /*!< 任务周期 */
    void (*task_handle)(void *arg);    /*!< 任务函数指针 */
}_task_blk_t;

/* Private variables ---------------------------------------------------------*/
/**
 * @brief 任务列表
 * 
 */
static _task_blk_t _task_list[] = {
    {0, 0, 1, app_debug_task},
    {0, 0, 10, app_key_task},
};

/* Private function ----------------------------------------------------------*/

/**=============================================================================
 * @brief           系统时基回调(滴答时间)
 *
 * @param[in]       none
 *
 * @return          none
 *============================================================================*/
static void systick_callback(void)
{
    //RTU_LOGI(LOG_TAG, "sys now time = %d", sys_time_get());
    for (uint8_t i = 0; i < _ARRAY_SIZE(_task_list); i++)
    {
        _task_blk_t *task = &_task_list[i];
        if (++task->slice_cnt >= task->peroid)
        {
            task->slice_cnt = 0;
            task->is_run = 1;
        }
    }
    
}

/**=============================================================================
 * @brief           系统时基初始化
 *
 * @param[in]       none
 *
 * @return          none
 *============================================================================*/
void systick_init(void)
{
    bsp_systick_init(systick_callback);
}

/**=============================================================================
 * @brief           任务轮询
 *
 * @param[in]       none
 *
 * @return          none
 *============================================================================*/
void app_task_poll(void)
{
    for (uint8_t i = 0; i < _ARRAY_SIZE(_task_list); i++)
    {
        _task_blk_t *task = &_task_list[i];
        if (task->is_run)
        {
            task->task_handle(NULL);
            task->is_run = 0;
        }
    }
}


/**=============================================================================
 * @brief           任务测试
 *
 * @param[in]       none
 *
 * @return          none
 *============================================================================*/
void TEST_CASE_TASK(void *arg)
{
    RTU_LOGE(LOG_TAG, "log error");
    RTU_LOGW(LOG_TAG, "log warning");
    RTU_LOGI(LOG_TAG, "log info");
    RTU_LOGD(LOG_TAG, "log debug");
    RTU_LOGV(LOG_TAG, "log verbose");
}
