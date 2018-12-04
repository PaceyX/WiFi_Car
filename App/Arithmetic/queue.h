/**
******************************************************************************
* @file    queue.h
* @author  Jalon
* @date    2016/08/19
* @brief   循环队列
* @attention Copyright (C) 2016 Inmotion Corporation
******************************************************************************
*/
#ifndef __QUEUE_H__
#define __QUEUE_H__

#include  "mcu_lib.h"

typedef struct QueueTypeStruct
{
    u32 front;  //队列头指针
    u32 rear;   //队列尾指针
    u32 itemSize; //队列大小
    u32 itemCount; //队列数据个数
    u8* pool;
}QueueType;

bool Queue_Init(QueueType* q, void* buffer, u32 buffer_size, u32 item_size); //队列初始化
bool Queue_Put(QueueType* q, void* pdata); //出队
bool Queue_Get(QueueType* q, void* pdata); //入队
bool Queue_Query(QueueType* q, void* pdata); //

#endif
