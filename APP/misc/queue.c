/*
 * queue.c
 *
 *  Created on: Feb 4, 2018
 *      Author: qyz
 */

#include "queue.h"
#include "stdlib.h"
#include "string.h"

#define queue_memory_blk_size sizeof(queue_struct)


void QueueCreate(Queue_t *queue)  
{  
	(void)memset(queue,0,sizeof(Queue_t));  
	queue->front = -1;  
	queue->rear = -1;  
	queue->size = 0; 
}

void QueueDestroy(Queue_t *queue)  
{  
    if (NULL != queue)   
    {  
    	if ( NULL != queue->data )
            free(queue->data);
    	free(queue);
    } 
}


void QueueClear(Queue_t *queue)  
{
    if (NULL == queue)
    	return;  
      
    queue->front = -1;  
	queue->rear = -1;  
	queue->size = 0;
    
    return;
} 


int QueueIsEmpty(Queue_t *queue)  
{  
    return (queue->size == 0); 
}

int QueueIsFull(Queue_t *queue)  
{  
	return (queue->size == queue_lenth);  
} 

int QueueGetLenth(Queue_t *queue)
{ 
    return queue->size;
}

int QueueSend(Queue_t *queue,data_t *ItemToQueue)
{    
    if (1 == QueueIsFull(queue)) 
    	return -1; /* full */
  
    queue->rear++;  
    queue->rear %= queue_lenth;  
    queue->size++;  

    ( void ) memcpy( ( void * ) &queue->data[queue->rear] , (void*)ItemToQueue, queue_memory_blk_size );
  
    return 0;     
} 

int QueueReceive(Queue_t *queue,data_t *pvBuffer)
{  
    if (1 == QueueIsEmpty(queue)) return -1; /* empty */  
  
    queue->front++;  
    queue->front %= queue_lenth;
    queue->size--; 

    if (NULL != pvBuffer)
    {
    	( void ) memcpy( ( void * ) pvBuffer, &queue->data[queue->front], queue_memory_blk_size);
    	(void)memset(&queue->data[queue->front],0,queue_memory_blk_size);  
    }  
    
    return 0;   
} 
