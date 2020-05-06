/**
  ******************************************************************************
  * @file			app_debug_log.c
  * @brief			app_debug_log function
  * @author			Xli
  * @email			xieliyzh@163.com
  * @version		1.0.0
  * @date			2020-04-29
  * @copyright		2020, EVECCA Co.,Ltd. All rights reserved
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "app_debug.h"
#include "bsp_debug_uart.h"

#include "queue.h"

/* Private constants ---------------------------------------------------------*/
#define RTU_OUTPUT_DATA_MAX_SIZE UART_BUFFER_MAX_SIZE    /*!< 终端输出最大缓冲长度 */
#define RTU_INPUT_DATA_MAX_SIZE UART_BUFFER_MAX_SIZE     /*!< 终端输入最大缓冲长度 */

/* Private macro -------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/**
 * @brief 输出到终端
 * 
 */
typedef struct {
    uint8_t buffer[RTU_OUTPUT_DATA_MAX_SIZE];
    int size;
}rtu_output_t;

/**
 * @brief 从终端输入
 * 
 */
typedef struct {
    uint8_t buffer[RTU_INPUT_DATA_MAX_SIZE];
    int size;
}rtu_input_t;

/* Private variables ---------------------------------------------------------*/
static rtu_log_level_t log_local_level = RTU_LOG_VERBOSE;
static rtu_input_t rtu_input;
static queue_t *log_queue = NULL;

/* Private function ----------------------------------------------------------*/
/**=============================================================================
 * @brief           数据打印
 *
 * @param[in]       none
 *
 * @return          none
 *============================================================================*/
void rtu_log_printf(const char *fmt, ...)
{
    rtu_output_t *p = (rtu_output_t*)malloc(sizeof(rtu_output_t));
    if (!p) return;
    
    va_list arg;
    va_start(arg, fmt);
    p->size = vsnprintf((char*)p->buffer, RTU_OUTPUT_DATA_MAX_SIZE, fmt, arg);
    va_end(arg);
    //assert_param(p->size < RTU_OUTPUT_DATA_MAX_SIZE);

    if (p->size)
    {
        //bsp_debug_uart_send((uint8_t*)p->buffer, p->size);
        uint32_t pbuf = 0;
        pbuf = (uint32_t)p;
        if (queue_send(log_queue, &pbuf) != 0)   goto exit;/*!< 发送失败，释放内存 */
        return;
    }
exit:   
    free(p);
    p = NULL;
}

/**=============================================================================
 * @brief           Get log level
 *
 * @param[in]       none
 *
 * @return          level: log level
 *============================================================================*/
rtu_log_level_t rtu_log_get_level(void)
{
    return log_local_level;
}

/**=============================================================================
 * @brief           Set log level
 *
 * @param[in]       level: log level
 *
 * @return          none
 *============================================================================*/
void rtu_log_level_set(rtu_log_level_t level)
{
   log_local_level = level;
}

/**=============================================================================
 * @brief           时间戳
 *
 * @param[in]       none
 *
 * @return          none
 *============================================================================*/
uint32_t rtu_log_timestamp(void)
{
    return 0;
}

/**=============================================================================
 * @brief           
 *
 * @param[in]       none
 *
 * @return          none
 *============================================================================*/
void app_debug_task(void *arg)
{
    uint32_t pbuf = 0;
    rtu_output_t *p = NULL;
    
    if (queue_receive(log_queue, &pbuf) == 0)   /*!< 接收成功 */
    {
        p = (rtu_output_t*)pbuf;
        bsp_debug_uart_send((uint8_t*)p->buffer, p->size);
        free(p);
        p = NULL;
    }
}

/**=============================================================================
 * @brief           中断输入回调
 *
 * @param[in]       none
 *
 * @return          none
 *============================================================================*/
static void rtu_input_callback(uint8_t *pbuf, uint16_t size)
{
    RTU_LOGI("info", "input data size = %d", size);
}

/**=============================================================================
 * @brief           调试初始化
 *
 * @param[in]       none
 *
 * @return          none
 *============================================================================*/
void app_debug_init(void)
{
    log_queue = queue_create();
    bsp_debug_uart_init(rtu_input_callback);
}
