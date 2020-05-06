/**
  ******************************************************************************
  * @file			queue.c
  * @brief			queue function
  * @author			Xli
  * @email			xieliyzh@163.com
  * @version		1.0.0
  * @date			2020-05-06
  * @copyright		2020, EVECCA Co.,Ltd. All rights reserved
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "queue.h"
#include <string.h>
#include <stdlib.h>

/* Private constants ---------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function ----------------------------------------------------------*/

/**=============================================================================
 * @brief           查找队列是否为空
 *
 * @param[in]       none
 *
 * @return          none
 *============================================================================*/
int is_queue_empty(queue_t *queue)
{
    if (queue == NULL) return -1;

    return (queue->front == queue->rear ? 1 : 0);
}

/**=============================================================================
 * @brief           查看队列是否已满
 *
 * @param[in]       none
 *
 * @return          none
 *============================================================================*/
int is_queue_full(queue_t *queue)  
{  
    if (queue == NULL) return -1;  
    
    return ((queue->rear + 1) % QUEUE_LENGTH == queue->front ? 1 : 0);  
}  

/**=============================================================================
 * @brief           队列创建
 *
 * @param[in]       none
 *
 * @return          none
 *============================================================================*/
queue_t* queue_create(void)
{
    queue_t *queue = (queue_t *)malloc(sizeof(queue_t));
    if (queue)
    {
        queue->front = queue->rear = 0;
    }

    return queue;
}

/**=============================================================================
 * @brief           队列摧毁
 *
 * @param[in]       none
 *
 * @return          none
 *============================================================================*/
int queue_destroy(queue_t *queue)
{
    if (queue != NULL) return -1;

    free(queue);

    return 0;
}

/**=============================================================================
 * @brief           队列清空
 *
 * @param[in]       none
 *
 * @return          none
 *============================================================================*/
int queue_clear(queue_t *queue)
{
    if (queue != NULL) return -1;

    queue->front = queue->rear = 0;

    return 0;
}

/**=============================================================================
 * @brief           队列发送
 *
 * @param[in]       none
 *
 * @return          none
 *============================================================================*/
int queue_send(queue_t *queue, const void *item)
{
    if (queue == NULL) return -1;  
    if (is_queue_full(queue) == 1) return -1; /* full */  
  
    queue->rear = (queue->rear + 1) % QUEUE_LENGTH;  
    ( void ) memcpy( ( void * ) &queue->data[queue->rear], item, sizeof(queue_elemtype_t) );  
  
    return 0;  
}

/**=============================================================================
 * @brief           队列发送
 *
 * @param[in]       none
 *
 * @return          none
 *============================================================================*/
int queue_receive(queue_t *queue, void *buffer)
{
    if (queue == NULL) return -1;  
    if (is_queue_empty(queue) == 1) return -1; /* empty */  

    queue->front = (queue->front + 1) % QUEUE_LENGTH;  
  
    ( void ) memcpy( ( void * ) buffer, &queue->data[queue->front], sizeof(queue_elemtype_t) );
    
    return 0;  
}
