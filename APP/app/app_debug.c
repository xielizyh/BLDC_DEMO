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

/* Private constants ---------------------------------------------------------*/
#define LOG_DATA_MAX_SIZE UART_BUFFER_MAX_SIZE

/* Private macro -------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/**
 * @brief 输出到终端
 * 
 */
typedef struct {
    uint8_t buffer[LOG_DATA_MAX_SIZE];
    uint16_t size;
}rtu_output_t;

/**
 * @brief 从终端输入
 * 
 */
typedef struct {
    uint8_t buffer[LOG_DATA_MAX_SIZE];
    uint16_t size;
}rtu_input_t;

/* Private variables ---------------------------------------------------------*/
static rtu_log_level_t log_local_level = RTU_LOG_VERBOSE;
static rtu_input_t rtu_input;

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
    p->size = vsnprintf((char*)p->buffer, sizeof(rtu_output_t), fmt, arg);
    va_end(arg);

    if (p->size)
    {
        bsp_debug_uart_send((uint8_t*)p->buffer, p->size);
    }
    
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
void app_debug_task(void *pvParameters)
{

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
    bsp_debug_uart_init(rtu_input.buffer);
}
