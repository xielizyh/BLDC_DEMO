/*
 * queue.h
 *
 *  Created on: Feb 4, 2018
 *      Author: qyz
 */

#ifndef QUEUE_H_
#define QUEUE_H_

#include "stdio.h"


#define queue_lenth 3  /*���г���*/
#define queue_size  64 /*�������ݴ�С*/


typedef struct 
{
    /** @brief Ҫ���͵Ļ����� */
    char buffer[queue_size];
    /** @brief Ҫ���͵����ݳ��� */
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
