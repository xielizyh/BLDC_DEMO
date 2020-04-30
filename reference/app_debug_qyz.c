/** @copyright SiHan Communication Tech Limited. All rights reserved. 2017.
  * @file      App_Debug.c
  * @author    qiuyongzheng
  * @version   V2.0.0
  * @date      08/04/2017
  * @brief     用户Debug接口
  */ 
#include "includes.h"
#include "queue.h"
#include "delay.h"
#include "malloc.h"
#include "hw_uart_dma_driver.h"
#include "app_bsp_init.h"
#include "app_debug.h"

	
#define DEBUG_UART_PORT USART6


/**
  * @brief 发送缓冲区的长度
 */
#define buffer_size 3000

/**
  * @brief 任务句柄
  */
static TaskHandle_t debug_thread_handler;

static rtu_log_level_t debug_level = RTU_LOG_VERBOSE;

/**
  * @brief 串口结构体
 */
typedef struct 
{
    /** @brief 要发送的缓冲区 */
    char buffer[buffer_size];
    /** @brief 要发送的数据长度 */
    int size;
}usart_memory_struct;

/**
  * @brief 串口内存块大小
 */
#define usart_memory_blk_size sizeof(usart_memory_struct)


/**
  * @brief 串口发送队列长度
 */
#define queue_number 20

/**
  * @brief 队列指针
 */
static QueueHandle_t pqueue = NULL;


static uint8_t is_app_running = 0;
/**
  * @brief 调试串口
 */
static Uart_data_t terminal_usart;


static uart_rx_buffer_t uart_rx_buffer[buffer_size];
static void app_debug_thread(void *pvParameters);
static void _debug_uart_rx_callback(uart_rx_buffer_t *rx_data,uint16_t size, void *param);

/**
  * @brief    创建Debug通讯任务 
 */
void app_debug_init(void)
{
    xTaskCreate((TaskFunction_t )app_debug_thread,                         
                (const char*    )"app_debug_thread",                     
                (uint16_t       )stack_size_debug,                
                (void*          )NULL,                                    
                (UBaseType_t    )prio_task_debug,                
                (TaskHandle_t*  )&debug_thread_handler);        
}

/**
  * @brief     client通讯任务 
  * @param pdata 无用
 */
static void app_debug_thread(void *pvParameters) 
{
    uint32_t pbuf;
    usart_memory_struct *p = NULL;
    pqueue = xQueueCreate(queue_number , sizeof(uint32_t));
    assert_param(pqueue);
	
    terminal_usart.port = DEBUG_UART_PORT;
    terminal_usart.baudrate = 115200;
    terminal_usart.format = UART_N_8_1;
    terminal_usart.u485en.en = 0;
    terminal_usart.rx_cb = _debug_uart_rx_callback;
    terminal_usart.using_dma_tx = 1;
    MX_UARTx_Init(&terminal_usart, uart_rx_buffer, buffer_size);	
    
    is_app_running = 1;
    while(is_app_running)
    {				
        if(xQueueReceive(pqueue,&pbuf,10))
        {
            p = (usart_memory_struct*)pbuf;/*将pbuf的值转换为指向pakcet的指针！*/

	    uart_transmit(&terminal_usart,(uint8_t*)p->buffer, p->size);/*发送到调试串口*/					

            myfree(SRAMEX,p);
            p = NULL;
        }		
    }
    is_app_running = 1;
    vQueueDelete(pqueue);
    pqueue = NULL;
    vTaskDelete(debug_thread_handler);
}

static void _debug_uart_rx_callback(uart_rx_buffer_t *rx_data,uint16_t size, void *param)
{
    
}


static uint8_t hex_to_string(uint8_t hex, uint8_t *pbuffer)
{
    uint8_t temp;

    temp = (hex & 0xf0) >> 4;
    if(temp <= 9)
        *pbuffer++ = temp + '0';
    else
        *pbuffer++ = temp - 10 + 'A';

    temp = (hex & 0x0f) >> 0;
    
    if(temp <= 9)
        *pbuffer++ = temp + '0';
    else
        *pbuffer++ = temp    - 10 + 'A';

    *pbuffer++ = ' ';

     return 3;
}

/**
  * @brief 重定向printf
  * @retval 无
 */
int fputc(int ch, FILE *f)
{
    HAL_UART_Transmit(&terminal_usart.uart,(uint8_t *)&ch,1,1000);
    return ch;
}

/**
  * @brief 打印数据接口
  * @details 可变参数
  * @param    pstring 字符参数
  * @retval 无
 */
void rtu_printf(const char *pstring, ...)
{
    if(pqueue == NULL)
        return;
    
    usart_memory_struct *p;
    uint32_t pbuf;
    va_list args;
                            
    p = mymalloc(SRAMEX,usart_memory_blk_size);
    if(!p) 
        return;

    va_start(args, pstring);
    p->size = vsnprintf(p->buffer, usart_memory_blk_size, pstring, args);
    va_end(args);
    assert_param(p->size < usart_memory_blk_size);

    if(!p->size)
    {
        myfree(SRAMEX,p);
        p = NULL;
        return;
    }     
    pbuf = (uint32_t)p;
    if(pdPASS == xQueueSend(pqueue,&pbuf,0))
    {
        return;             /*成功入栈，则不释放内存*/
    }
    myfree(SRAMEX,p);
    p = NULL;        
}

void rtu_print_buffer(const uint8_t *buffer, uint16_t size)
{
    if(pqueue == NULL)
        return;

    uint32_t pbuf;
    usart_memory_struct *p;
    int i;
            
    if(!size)
        return;

    p = mymalloc(SRAMEX,usart_memory_blk_size);
    if(!p) 
        return;
    
    p->size = 0;
    for(i = 0; i < size; ++i)
    {
        if(buffer_size - p->size < 6)
        {
            /* not enough memory to fill the buffer, fix the EOF */
            p->buffer[p->size] = 0;
            p->buffer[p->size - 1] = '\n';
            p->buffer[p->size - 2] = '\r';
            p->buffer[p->size - 3] = '.';
            p->buffer[p->size - 4] = '.';
            p->buffer[p->size - 5] = '.';
            break;    
        }
        p->size += hex_to_string(buffer[i], (uint8_t *)&p->buffer[p->size]);
    }
    /* all buffer filled to memory */
    if(i == size)
    {
        p->buffer[p->size++] = '\r';
        p->buffer[p->size++] = '\n';
        p->buffer[p->size++] = 0;
    }
    if(!p->size)
    {
        myfree(SRAMEX,p);
        p = NULL;
        return;
    }     
    pbuf = (uint32_t)p;
    if(pdPASS == xQueueSend(pqueue,&pbuf,100))
    {
        return;/*成功入栈，则不释放内存*/
    }
    myfree(SRAMEX,p);
    p = NULL;     
}

void rtu_print_buffer_isr(const uint8_t *buffer, uint16_t size)
{
    if(pqueue == NULL)
        return;

    uint32_t pbuf;
    usart_memory_struct *p;
    int i;
            
    if(!size)
        return;

    p = mymalloc(SRAMEX,usart_memory_blk_size);
    if(!p) 
        return;
    
    p->size = 0;
    for(i = 0; i < size; ++i)
    {
        if(buffer_size - p->size < 6)
        {
            /* not enough memory to fill the buffer, fix the EOF */
            p->buffer[p->size] = 0;
            p->buffer[p->size - 1] = '\n';
            p->buffer[p->size - 2] = '\r';
            p->buffer[p->size - 3] = '.';
            p->buffer[p->size - 4] = '.';
            p->buffer[p->size - 5] = '.';
            break;    
        }
        p->size += hex_to_string(buffer[i], (uint8_t *)&p->buffer[p->size]);
    }
    /* all buffer filled to memory */
    if(i == size)
    {
        p->buffer[p->size++] = '\r';
        p->buffer[p->size++] = '\n';
        p->buffer[p->size++] = 0;
    }
    if(!p->size)
    {
        myfree(SRAMEX,p);
        p = NULL;
        return;
    }     
    pbuf = (uint32_t)p;
    if(pdTRUE == xQueueSendFromISR(pqueue,&pbuf,pdFALSE))
    {
        return;/*成功入栈，则不释放内存*/
    }
    myfree(SRAMEX,p);
    p = NULL;     
}


uint32_t rtu_log_timestamp(void) 
{
    return HAL_GetTick();
}

void debug_set_level(rtu_log_level_t level)
{
    debug_level = level;
}

rtu_log_level_t debug_get_level(void)
{
    return debug_level;
}


void DMA2_Stream6_IRQHandler(void)
{
    HAL_DMA_IRQHandler(terminal_usart.uart.hdmatx); 
}

void USART6_IRQHandler(void)
{
    _hw_uart_isr(&terminal_usart);
}
