/**
  ******************************************************************************
  * @file			app_motor.h
  * @brief			app_motor header file
  * @author			Xli
  * @email			xieliyzh@163.com
  * @version		1.0.0
  * @date			2020-05-08
  * @copyright		2020, XIELI Co.,Ltd. All rights reserved
  ******************************************************************************
**/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __APP_MOTOR_H_
#define __APP_MOTOR_H_

/* Includes ------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C"{
#endif

/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported typedef ----------------------------------------------------------*/
/* Exported variables ------------------------------------------------------- */
/* Exported functions ------------------------------------------------------- */
void app_motor_task(void *arg);
void app_motor_init(void);

#ifdef __cplusplus
}
#endif

#endif  /* __APP_MOTOR_H_ */
