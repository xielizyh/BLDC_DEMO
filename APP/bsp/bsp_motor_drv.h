/**
  ******************************************************************************
  * @file			bsp_motor_drv.h
  * @brief			bsp_motor_drv header file
  * @author			Xli
  * @email			xieliyzh@163.com
  * @version		1.0.0
  * @date			2020-04-29
  * @copyright		2020, XIELI Co.,Ltd. All rights reserved
  ******************************************************************************
**/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_MOTOR_DRV_H_
#define __BSP_MOTOR_DRV_H_

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

#ifdef __cplusplus
extern "C"{
#endif

/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported typedef ----------------------------------------------------------*/
typedef void (*bsp_motor_it_update_callback)(void);

/* Exported variables ------------------------------------------------------- */
/* Exported functions ------------------------------------------------------- */
void bsp_motor_MOS_Q51PWM(uint8_t duty);
void bsp_motor_MOS_Q16PWM(uint8_t duty);
void bsp_motor_MOS_Q62PWM(uint8_t duty);
void bsp_motor_MOS_Q24PWM(uint8_t duty);
void bsp_motor_MOS_Q43PWM(uint8_t duty);
void bsp_motor_MOS_Q35PWM(uint8_t duty);
void bsp_motor_start(void);
void bsp_motor_stop(void);
void bsp_motor_drv_init(bsp_motor_it_update_callback cb);

#ifdef __cplusplus
}
#endif

#endif  /* __BSP_MOTOR_DRV_H_ */
