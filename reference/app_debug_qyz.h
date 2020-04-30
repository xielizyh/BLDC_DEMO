#ifndef _app_debug_h_include_
#define _app_debug_h_include_
#include <stdint.h>

typedef enum {
    RTU_LOG_NONE,       /*!< No log output */
    RTU_LOG_ERROR,      /*!< Critical errors, software module can not recover on its own */
    RTU_LOG_WARN,       /*!< Error conditions from which recovery measures have been taken */
    RTU_LOG_INFO,       /*!< Information messages which describe normal flow of events */
    RTU_LOG_DEBUG,      /*!< Extra information which is not necessary for normal use (values, pointers, sizes, etc). */
    RTU_LOG_VERBOSE     /*!< Bigger chunks of debugging information, or frequent messages which can potentially flood the output. */
} rtu_log_level_t;

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
#define LOG_COLOR_D       LOG_COLOR(LOG_COLOR_CYAN)
#define LOG_COLOR_V


#define LOG_FORMAT(letter, format)  LOG_COLOR_ ## letter #letter " (%d) %s: " format LOG_RESET_COLOR "\r\n"


#define RTU_EARLY_LOGE( tag, format, ... ) RTU_LOG_EARLY_IMPL(tag, format, RTU_LOG_ERROR,   E, ##__VA_ARGS__)
#define RTU_EARLY_LOGW( tag, format, ... ) RTU_LOG_EARLY_IMPL(tag, format, RTU_LOG_WARN,    W, ##__VA_ARGS__)
#define RTU_EARLY_LOGI( tag, format, ... ) RTU_LOG_EARLY_IMPL(tag, format, RTU_LOG_INFO,    I, ##__VA_ARGS__)
#define RTU_EARLY_LOGD( tag, format, ... ) RTU_LOG_EARLY_IMPL(tag, format, RTU_LOG_DEBUG,   D, ##__VA_ARGS__)
#define RTU_EARLY_LOGV( tag, format, ... ) RTU_LOG_EARLY_IMPL(tag, format, RTU_LOG_VERBOSE, V, ##__VA_ARGS__)

#define RTU_LOG_EARLY_IMPL(tag, format, log_level, log_tag_letter, ...) do {                         \
        if (debug_get_level() >= log_level) {                                                          \
            rtu_printf(LOG_FORMAT(log_tag_letter, format), rtu_log_timestamp(), tag, ##__VA_ARGS__); \
        }} while(0)

#define RTU_LOGE( tag, format, ... )  RTU_EARLY_LOGE(tag, format, ##__VA_ARGS__)
#define RTU_LOGW( tag, format, ... )  RTU_EARLY_LOGW(tag, format, ##__VA_ARGS__)
#define RTU_LOGI( tag, format, ... )  RTU_EARLY_LOGI(tag, format, ##__VA_ARGS__)
#define RTU_LOGD( tag, format, ... )  RTU_EARLY_LOGD(tag, format, ##__VA_ARGS__)
#define RTU_LOGV( tag, format, ... )  RTU_EARLY_LOGV(tag, format, ##__VA_ARGS__)
    
void app_debug_init(void);
rtu_log_level_t debug_get_level(void);
void debug_set_level(rtu_log_level_t level);
uint32_t rtu_log_timestamp(void);

void rtu_printf(const char *pstring, ...);
void rtu_print_buffer(const uint8_t *buffer, uint16_t size);
void rtu_print_buffer_isr(const uint8_t *buffer, uint16_t size);

#endif /* _app_debug_h_include_ */
