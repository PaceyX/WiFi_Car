#include "stm32f4xx_hal.h"




/**
  * @brief  Read an amount of data in blocking mode from a specific memory address
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @param  DevAddress Target device address
  * @param  MemAddress Internal memory address
  * @param  MemAddSize Size of internal memory address
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @param  Timeout Timeout duration
  * @retval HAL status
  * @note   此函数与HAL_I2C_Mem_Read函数的使用方法基本一致，但是加入对指定的i2c标志是否设置的判断
            并进行了超时时间的判断
  */
HAL_StatusTypeDef hIicMemoryRead(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, 
        uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
    HAL_StatusTypeDef ret;
    uint32_t tick = 0;
    
    while(__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_BUSY) == SET)
    {
        HAL_Delay(1);
        tick++;
        if(tick > Timeout)
        {
            HAL_I2C_ErrorCallback(hi2c);
            return HAL_BUSY;
        }
    }
    
    ret = HAL_I2C_Mem_Read(hi2c, DevAddress, MemAddress, MemAddSize, pData, Size, Timeout);
    if(ret == HAL_BUSY)
    {
        HAL_I2C_ErrorCallback(hi2c);
    }
    return ret;
}


/**
  * @brief  Write an amount of data in blocking mode to a specific memory address
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @param  DevAddress Target device address
  * @param  MemAddress Internal memory address
  * @param  MemAddSize Size of internal memory address
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @param  Timeout Timeout duration
  * @retval HAL status
  * @note   此函数与HAL_I2C_Mem_Write函数的使用方法基本一致，但是加入对指定的i2c标志是否设置的判断
            并进行了超时时间的判断
  */
HAL_StatusTypeDef hIicMemoryWrite(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, 
        uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
    HAL_StatusTypeDef ret;
    uint32_t tick = 0;
    
    while(__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_BUSY) == SET)
    {
        HAL_Delay(1);
        tick++;
        if(tick > Timeout)
        {
            HAL_I2C_ErrorCallback(hi2c);
            return HAL_BUSY;
        }
    }
    
    ret = HAL_I2C_Mem_Write(hi2c, DevAddress, MemAddress, MemAddSize, pData, Size, Timeout);
    if(ret == HAL_BUSY)
    {
        HAL_I2C_ErrorCallback(hi2c);
    }
    return ret;
}


HAL_StatusTypeDef hIicMemoryWriteByDma(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size)
{
    HAL_StatusTypeDef ret;
    DMA_HandleTypeDef *hdma = hi2c->hdmatx;
    
    ret = HAL_I2C_Mem_Write_DMA(hi2c, DevAddress, MemAddress, MemAddSize, pData, Size);

    if(ret == HAL_OK)
    {
        while(__HAL_DMA_GET_FLAG(hdma, __HAL_DMA_GET_TC_FLAG_INDEX(hdma)) == RESET)
        {
            /* Transfer Error Interrupt management ***************************************/
            if(__HAL_DMA_GET_FLAG(hdma, __HAL_DMA_GET_TE_FLAG_INDEX(hdma)) != RESET)
            {
                /* Clear the transfer error flag */
                __HAL_DMA_CLEAR_FLAG(hdma, __HAL_DMA_GET_TE_FLAG_INDEX(hdma));

                /* Update error code */
                SET_BIT(hdma->ErrorCode, HAL_DMA_ERROR_TE);

                /* Change the DMA state */
                hdma->State = HAL_DMA_STATE_ERROR;

                /* Process Unlocked */
                __HAL_UNLOCK(hdma);
                
                if (hdma->XferErrorCallback != NULL)
                {
                    /* Transfer error callback */
                    hdma->XferErrorCallback(hdma);
                }
                
                return HAL_ERROR;
            }
        }
            
        /* Clear the transfer complete flag */
        __HAL_DMA_CLEAR_FLAG(hdma, __HAL_DMA_GET_TC_FLAG_INDEX(hdma));

        /* Update error code */
        SET_BIT(hdma->ErrorCode, HAL_DMA_ERROR_NONE);

        /* Change the DMA state */
        hdma->State = HAL_DMA_STATE_READY;

        /* Process Unlocked */
        __HAL_UNLOCK(hdma);
            
        if(hdma->XferCpltCallback != NULL)
        {       
            /* Transfer complete callback */
            hdma->XferCpltCallback(hdma);
        }
    }
    else if(ret == HAL_BUSY)
    {
        HAL_I2C_ErrorCallback(hi2c);
    }
    
    return ret;
}


HAL_StatusTypeDef hIicMemoryReadByDma(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size)
{
    HAL_StatusTypeDef ret;
    DMA_HandleTypeDef *hdma = hi2c->hdmarx;
    
    ret = HAL_I2C_Mem_Read_DMA(hi2c, DevAddress, MemAddress, MemAddSize, pData, Size);
    
    if(ret == HAL_OK)
    {
        while(__HAL_DMA_GET_FLAG(hdma, __HAL_DMA_GET_TC_FLAG_INDEX(hdma)) == RESET)
        {
            /* Transfer Error Interrupt management ***************************************/
            if(__HAL_DMA_GET_FLAG(hdma, __HAL_DMA_GET_TE_FLAG_INDEX(hdma)) != RESET)
            {
                /* Clear the transfer error flag */
                __HAL_DMA_CLEAR_FLAG(hdma, __HAL_DMA_GET_TE_FLAG_INDEX(hdma));

                /* Update error code */
                SET_BIT(hdma->ErrorCode, HAL_DMA_ERROR_TE);

                /* Change the DMA state */
                hdma->State = HAL_DMA_STATE_ERROR;

                /* Process Unlocked */
                __HAL_UNLOCK(hdma);
                
                if (hdma->XferErrorCallback != NULL)
                {
                    /* Transfer error callback */
                    hdma->XferErrorCallback(hdma);
                }
                
                return HAL_ERROR;
            }
        }
            
        /* Clear the transfer complete flag */
        __HAL_DMA_CLEAR_FLAG(hdma, __HAL_DMA_GET_TC_FLAG_INDEX(hdma));

        /* Update error code */
        SET_BIT(hdma->ErrorCode, HAL_DMA_ERROR_NONE);

        /* Change the DMA state */
        hdma->State = HAL_DMA_STATE_READY;

        /* Process Unlocked */
        __HAL_UNLOCK(hdma);
            
        if(hdma->XferCpltCallback != NULL)
        {       
            /* Transfer complete callback */
            hdma->XferCpltCallback(hdma);
        }
    }
    else if(ret == HAL_BUSY)
    {
        HAL_I2C_ErrorCallback(hi2c);
    }
    
    return ret;
}

