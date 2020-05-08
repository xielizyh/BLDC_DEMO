/**
  ******************************************************************************
  * @file			app_key.h
  * @brief			app_key header file
  * @author			Xli
  * @email			xieliyzh@163.com
  * @version		1.0.0
  * @date			2019-07-18
  * @copyright		2019, EVECCA Co.,Ltd. All rights reserved
  ******************************************************************************
**/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __APP_KEY_H_
#define __APP_KEY_H_

/* Includes ------------------------------------------------------------------*/
#include "bsp_key.h"

#ifdef __cplusplus
extern "C"{
#endif

/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported typedef ----------------------------------------------------------*/
/**
* @brief 按键事件定义
*/
typedef struct{
  bsp_key_num_t num;
  bsp_key_sta_t sta;
  uint8_t       clicks;
}app_key_evt_t;

/* Exported variables --------------------------------------------------------*/
typedef uint8_t app_key_if_t; 
typedef void (*app_key_cb_t)(app_key_evt_t *event);

/* Exported functions --------------------------------------------------------*/
void app_key_init(void);
int app_key_register_callback(bsp_key_num_t key_num, app_key_cb_t callback);
void app_key_task(void *arg);

#ifdef __cplusplus
}
#endif

#endif  /* __APP_KEY_H_ */
