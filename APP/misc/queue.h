/**
  ******************************************************************************
  * @file			queue.h
  * @brief			queue header file
  * @author			Xli
  * @email			xieliyzh@163.com
  * @version		1.0.0
  * @date			2020-05-06
  * @copyright		2020, XLI Co.,Ltd. All rights reserved
  ******************************************************************************
**/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __QUEUE_H_
#define __QUEUE_H_

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

#ifdef __cplusplus
extern "C"{
#endif

/* Exported constants --------------------------------------------------------*/
#define QUEUE_LENGTH    4    /*!< 队列长度 */

/* Exported macros -----------------------------------------------------------*/
/* Exported typedef ----------------------------------------------------------*/
/**
 * @brief 队列元素的数据类型
 * 
 */
typedef uint32_t queue_elemtype_t;

/**
* @brief 队列定义
*/
typedef struct {
    queue_elemtype_t data[QUEUE_LENGTH];    /*!< 数组作为队列的存储空间 */
    int front,rear; /*!< 队列头尾指针 */
}queue_t;

/* Exported variables ------------------------------------------------------- */
/* Exported functions ------------------------------------------------------- */
queue_t* queue_create(void);
int queue_send(queue_t *queue, const void *item);
int queue_receive(queue_t *queue, void *buffer);

#ifdef __cplusplus
}
#endif

#endif  /* __QUEUE_H_ */
