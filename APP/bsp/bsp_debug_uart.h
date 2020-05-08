/**
  ******************************************************************************
  * @file			bsp_debug_uart.h
  * @brief			bsp_debug_uart header file
  * @author			Xli
  * @email			xieliyzh@163.com
  * @version		1.0.0
  * @date			2020-04-30
  * @copyright		2020, XIELI Co.,Ltd. All rights reserved
  ******************************************************************************
**/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_DEBUG_UART_H_
#define __BSP_DEBUG_UART_H_

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

#ifdef __cplusplus
extern "C"{
#endif

/* Exported constants --------------------------------------------------------*/
#define UART_BUFFER_MAX_SIZE        128

/* Exported macros -----------------------------------------------------------*/
/* Exported typedef ----------------------------------------------------------*/
/**
 * @brief 串口接收数据回调
 * 
 */
typedef void (*bsp_debug_uart_rx_callback)(uint8_t *, uint16_t);

/* Exported variables ------------------------------------------------------- */
/* Exported functions ------------------------------------------------------- */
uint8_t bsp_debug_uart_is_transferring(void);
int bsp_debug_uart_send(uint8_t *pbuf, uint16_t size);
void bsp_debug_uart_init(bsp_debug_uart_rx_callback recv_cb);

#ifdef __cplusplus
}
#endif

#endif  /* __BSP_DEBUG_UART_H_ */
