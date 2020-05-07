/**
  ******************************************************************************
  * @file			app_task.h
  * @brief			app_task header file
  * @author			Xli
  * @email			xieliyzh@163.com
  * @version		1.0.0
  * @date			2020-05-07
  * @copyright		2020, XIELI Co.,Ltd. All rights reserved
  ******************************************************************************
**/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __APP_TASK_H_
#define __APP_TASK_H_

/* Includes ------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C"{
#endif

/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported typedef ----------------------------------------------------------*/
/* Exported variables ------------------------------------------------------- */
/* Exported functions ------------------------------------------------------- */
void systick_init(void);
void app_task_poll(void);
void TEST_CASE_TASK(void *arg);
#ifdef __cplusplus
}
#endif

#endif  /* __APP_TASK_H_ */
