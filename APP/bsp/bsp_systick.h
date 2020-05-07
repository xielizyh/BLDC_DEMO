/**
  ******************************************************************************
  * @file			bsp_systick.h
  * @brief			bsp_systick header file
  * @author			Xli
  * @email			xieliyzh@163.com
  * @version		1.0.0
  * @date			2020-05-07
  * @copyright		2020, XIELI Co.,Ltd. All rights reserved
  ******************************************************************************
**/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_SYSTICK_H_
#define __BSP_SYSTICK_H_

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

#ifdef __cplusplus
extern "C"{
#endif

/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported typedef ----------------------------------------------------------*/
/**
 * @brief 系统滴答定时器回调
 * 
 */
typedef void (*bsp_systick_callback)(void);

/* Exported variables ------------------------------------------------------- */
/* Exported functions ------------------------------------------------------- */
void systick_handler(void);
uint32_t sys_time_get(void);
void bsp_systick_init(bsp_systick_callback cb);

#ifdef __cplusplus
}
#endif

#endif  /* __BSP_SYSTICK_H_ */
