/**
******************************************************************************
* @file    navipack_transport_layer.c
* @author  *
* @date    *
* @brief   通讯协议传输层解包封包等相关函数
* @attention Copyright (C) 
******************************************************************************
*/

/** @addtogroup Navipack_MCU_SDK
* @{
*/

/** @defgroup Transport_Layer Transport layer
* @brief Navipack Transport layer
* @{
*/

#include "navipack_transport_layer.h"

//传输层协议标志
#define FRAMECTRL 0xA5
#define FRAMEHEAD 0xAA
#define FRAMETAIL 0x55

/**
* @brief  传输层解包函数
* @param  pframe : 数据帧对象
* @param  buffer : 解包结果存储缓冲区
* @param  size   : 解包缓冲区尺寸
* @param  data   : 接收的数据，单 byte
* @retval 是否成功解包
*/
bool TransportUnpacking(TransportFrame_Type *pframe, u8* buffer, u16 size, u8 data)
{
    if(data == FRAMEHEAD && pframe->lastByte == FRAMEHEAD)
    {
        pframe->offset = 0;
        pframe->checkSum = 0;
        pframe->recvFlag = true;
        return false;
    }

    if(pframe->recvFlag)
    {
        // 收到结束符
        if(data == FRAMETAIL && pframe->lastByte == FRAMETAIL)
        { 
            pframe->recvFlag = false;
            
            if(pframe->offset < 3)
            {
                pframe->errorCount++;
                return false;
            }
            
            pframe->offset -= 2;
            pframe->checkSum -= (FRAMETAIL + buffer[pframe->offset]);

            if(pframe->checkSum == buffer[pframe->offset])
            {
                return true;
            }
            else
            {
				if(pframe->offset <= 0x12){// the wifi board bootloader will add 0x0d after 0x0a, and will make error checksum
					uint8_t *ptr = buffer + 1;
					uint8_t v1 = buffer[0];
					int32_t i = 1, need_to_calc = 0;
					
					if (buffer[pframe->offset] == 0x0d && buffer[pframe->offset - 1] == 0x0a)
					{
						pframe->offset--;
						need_to_calc = 1;
					}
					
					for(;i < pframe->offset ; i ++){
						uint8_t v2 = buffer[i];
						if(v1 == 0x0a && v2 == 0x0d){
							need_to_calc = 1;
						}else{
							*ptr++ = v2;
						}
						v1 = v2;						
					}
					if(need_to_calc){
						uint8_t * cur_ptr = buffer;
						uint8_t chc = 0;
						while(cur_ptr < ptr){
							chc += *cur_ptr ++;
						}
						if(chc == buffer[pframe->offset]){
							pframe->offset = ptr - buffer;
							return true;
						}
					}
				}
				
                pframe->errorCount++;
                return false;
            }
        }

        // 控制字
        if(pframe->ctrlFlag)
        {
            pframe->ctrlFlag = false;

            if(data == FRAMEHEAD || data == FRAMETAIL || data == FRAMECTRL)
            {
                buffer[pframe->offset++] = data;
                pframe->checkSum += data;
                data = FRAMECTRL;
            }
            else
            {
                // 复位
                pframe->recvFlag = false;
                pframe->errorCount++;
            }
        }
        else
        {
            if(data == FRAMECTRL)
            {
                pframe->ctrlFlag = true;
            }
            else
            {
                buffer[pframe->offset++] = data;
                pframe->checkSum += data;
            }
        }

        // 数据长度超过 SIZE
        if(pframe->offset >= size)
        {
            // 复位
            pframe->recvFlag = false;
            pframe->errorCount++;
        }
    }

    pframe->lastByte = data;

    return false;
}

/**
* @brief  传输层打包函数
* @param  pframe    : 数据帧对象
* @param  buffer    : 打包结果存储缓冲区
* @param  size      : 打包缓冲区尺寸
* @param  in_buf    : 打包数据指针
* @param  len       : 打包数据长度
* @param  pack_flag : 打包模式 @ref PACK_FLAG_define 按 bit 设置
* @retval 打包错误则返回 false
*/
bool TransportPacking(TransportFrame_Type *pframe, u8* buffer, u16 size, u8 *in_buf, u16 len, u8 pack_flag)
{
    u16 i;

    if((pack_flag & PACK_FLAG_BEGIN) != 0)
    {
        pframe->offset = 0;
        pframe->checkSum = 0;
        
        if(len > size - 5)      //当发送的数据长度超过发送SIZE - 包头字节数 - 包尾字节数 - 校验位字节数
            return false;
        
        buffer[pframe->offset++] = FRAMEHEAD;      //加头
        buffer[pframe->offset++] = FRAMEHEAD;
        
    }
    else
    {
        if(len + pframe->offset > size - 3)      //当发送的数据长度超过发送SIZE - 包尾字节数 - 校验位字节数
            return false;
    }

    for (i = 0; i < len; i++)
    {
        if( (*in_buf == FRAMECTRL) || (*in_buf == FRAMEHEAD)|| (*in_buf == FRAMETAIL) )
        {
            buffer[pframe->offset++] = FRAMECTRL;
            if((len - i) + pframe->offset > size - 3)    //溢出判断
                return false;
        }
        buffer[pframe->offset++] = *in_buf;
        pframe->checkSum += *in_buf;
        in_buf++;
    }

    if((pack_flag & PACK_FLAG_END) != 0)
    {
        //校验和
        if ( (pframe->checkSum == FRAMECTRL) || (pframe->checkSum == FRAMEHEAD) || (pframe->checkSum == FRAMETAIL) )
        {
            buffer[pframe->offset++] = FRAMECTRL;
            if(pframe->offset > size - 3)    //溢出判断
                return false;
        }
        buffer[pframe->offset++] = pframe->checkSum;

        buffer[pframe->offset++] = FRAMETAIL;      //加尾
        buffer[pframe->offset++] = FRAMETAIL;
    }

    return true;
}

/**
* @}
*/

/**
* @}
*/
