/**
  ******************************************************************************
  * @file			app_debug.h
  * @brief			app_debug header file
  * @author			Xli
  * @email			xieliyzh@163.com
  * @version		1.0.0
  * @date			2020-04-29
  * @copyright		2020, XIELI Co.,Ltd. All rights reserved
  ******************************************************************************
**/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __APP_DEBUG_H_
#define __APP_DEBUG_H_

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @brief 日志标签
 * 
 */
#define LOG_TAG   __func__

/**
 * @brief Log level
 *
 */
typedef enum {
    RTU_LOG_NONE,       /*!< No log output */
    RTU_LOG_ERROR,      /*!< Critical errors, software module can not recover on its own */
    RTU_LOG_WARN,       /*!< Error conditions from which recovery measures have been taken */
    RTU_LOG_INFO,       /*!< Information messages which describe normal flow of events */
    RTU_LOG_DEBUG,      /*!< Extra information which is not necessary for normal use (values, pointers, sizes, etc). */
    RTU_LOG_VERBOSE     /*!< Bigger chunks of debugging information, or frequent messages which can potentially flood the output. */
}rtu_log_level_t;

#define CONFIG_LOG_COLORS 1 /*!< 使能日志颜色 */
/**
 * @brief Log color
 *
 */
#if CONFIG_LOG_COLORS
#define LOG_COLOR_BLACK   "30"
#define LOG_COLOR_RED     "31"
#define LOG_COLOR_GREEN   "32"
#define LOG_COLOR_BROWN   "33"
#define LOG_COLOR_BLUE    "34"
#define LOG_COLOR_PURPLE  "35"
#define LOG_COLOR_CYAN    "36"
#define LOG_COLOR(COLOR)  "\033[0;" COLOR "m"
#define LOG_BOLD(COLOR)   "\033[1;" COLOR "m"
#define LOG_RESET_COLOR   "\033[0m"
#define LOG_COLOR_E       LOG_COLOR(LOG_COLOR_RED)
#define LOG_COLOR_W       LOG_COLOR(LOG_COLOR_BROWN)
#define LOG_COLOR_I       LOG_COLOR(LOG_COLOR_GREEN)
#define LOG_COLOR_D
#define LOG_COLOR_V
#else //CONFIG_LOG_COLORS
#define LOG_COLOR_E
#define LOG_COLOR_W
#define LOG_COLOR_I
#define LOG_COLOR_D
#define LOG_COLOR_V
#define LOG_RESET_COLOR
#endif //CONFIG_LOG_COLORS

/**
 * @brief Log format
 * 
 */
#define LOG_FORMAT(letter, format)  LOG_COLOR_ ## letter #letter " (%d) %s: " format LOG_RESET_COLOR "\r\n"

/**
 * @brief Log implement
 * 
 */
#define RTU_LOG_IMPL(tag, format, log_level, log_tag_letter, ...) do {                                  \
        if (rtu_log_get_level() >= log_level) {                                                         \
            rtu_log_printf(LOG_FORMAT(log_tag_letter, format), rtu_log_timestamp(), tag, ##__VA_ARGS__);\
        }} while(0)

/**
 * macro to output logs at RTU_LOG level.
 *
 * @param tag tag of the log, which can be used to change the log level by ``rtu_log_level_set`` at runtime.
 *
 * @see ``printf``
 */
#define RTU_LOGE( tag, format, ... )  RTU_LOG_IMPL(tag, format, RTU_LOG_ERROR,   E, ##__VA_ARGS__)
#define RTU_LOGW( tag, format, ... )  RTU_LOG_IMPL(tag, format, RTU_LOG_WARN,    W, ##__VA_ARGS__)
#define RTU_LOGI( tag, format, ... )  RTU_LOG_IMPL(tag, format, RTU_LOG_INFO,    I, ##__VA_ARGS__)
#define RTU_LOGD( tag, format, ... )  RTU_LOG_IMPL(tag, format, RTU_LOG_DEBUG,   D, ##__VA_ARGS__)
#define RTU_LOGV( tag, format, ... )  RTU_LOG_IMPL(tag, format, RTU_LOG_VERBOSE, V, ##__VA_ARGS__)

/**
 * @brief Get log level
 * 
 * @return log level 
 */
rtu_log_level_t rtu_log_get_level(void);

/**
 * @brief Set log level
 * 
 * @param level log level
 */
void rtu_log_level_set(rtu_log_level_t level);

/**
 * @brief Get log timestamp
 * 
 * @return timestamp 
 */
uint32_t rtu_log_timestamp(void);

/**
 * @brief 打印数据接口
 * 
 * @param fmt: 可变参数
 */
void rtu_log_printf(const char *fmt, ...);

/**
 * @brief 调试初始化
 * 
 */
void app_debug_init(void);
void app_debug_task(void *arg);

#ifdef __cplusplus
}
#endif

#endif  /* __APP_DEBUG_H_ */
