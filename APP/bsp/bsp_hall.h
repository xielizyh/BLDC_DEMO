/**
  ******************************************************************************
  * @file			bsp_hall.h
  * @brief			bsp_hall header file
  * @author			Xli
  * @email			xieliyzh@163.com
  * @version		1.0.0
  * @date			2020-05-08
  * @copyright		2020, XIELI Co.,Ltd. All rights reserved
  ******************************************************************************
**/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_HALL_H_
#define __BSP_HALL_H_

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

#ifdef __cplusplus
extern "C"{
#endif

/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported typedef ----------------------------------------------------------*/
/**
 * @brief 按键索引定义
 */
typedef enum{
    BSP_HALL_A,
    BSP_HALL_B,
    BSP_HALL_C,
    BSP_HALL_MAX,
}bsp_hall_num_t;

/* Exported variables ------------------------------------------------------- */
/* Exported functions ------------------------------------------------------- */
uint8_t bsp_hall_get_level(uint8_t idx);
void bsp_hall_init(void);

#ifdef __cplusplus
}
#endif

#endif  /* __BSP_HALL_H_ */
