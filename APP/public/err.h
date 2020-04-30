/**
  ******************************************************************************
  * @file			err.h
  * @brief			err header file
  * @author			Xli
  * @email			xieliyzh@163.com
  * @version		1.0.0
  * @date			2020-04-29
  * @copyright		2020, XIELI Co.,Ltd. All rights reserved
  ******************************************************************************
**/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ERR_H_
#define __ERR_H_

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

#ifdef __cplusplus
extern "C"{
#endif

/* Exported constants --------------------------------------------------------*/
/**
 * @brief 错误码定义
 */
#define ERR_NONE              (0)       /*!< 无错误 */
#define ERR_FAIL              (0x01)    /*!< 失败 */
#define ERR_NO_MEM            (0x02)    /*!< 内存溢出 */
#define ERR_INVALID_ARG       (0x03)    /*!< 变量非法 */   

/* Exported macros -----------------------------------------------------------*/
/* Exported typedef ----------------------------------------------------------*/
typedef uint16_t err_t;

/* Exported variables ------------------------------------------------------- */
/* Exported functions ------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif  /* __ERR_H_ */