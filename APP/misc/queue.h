/*
 * queue.h
 *
 *  Created on: Feb 4, 2018
 *      Author: qyz
 */

#ifndef QUEUE_H_
#define QUEUE_H_

#include "stdio.h"


#define queue_lenth 3  /*队列长度*/
#define queue_size  64 /*队列数据大小*/


typedef struct 
{
    /** @brief 要发送的缓冲区 */
    char buffer[queue_size];
    /** @brief 要发送的数据长度 */
    int size;
}queue_struct;


typedef queue_struct data_t;  

typedef struct  
{  
    data_t data[queue_lenth];
    int front,rear,size;
}Queue_t; 



void QueueCreate(Queue_t *queue); 
void QueueDestroy(Queue_t *queue);
void QueueClear(Queue_t *queue);
int QueueIsEmpty(Queue_t *queue);
int QueueIsFull(Queue_t *queue);
int QueueGetLenth(Queue_t *queue);
int QueueSend(Queue_t *queue,data_t *ItemToQueue);
int QueueReceive(Queue_t *queue,data_t *pvBuffer);


#endif /* QUEUE_H_ */
