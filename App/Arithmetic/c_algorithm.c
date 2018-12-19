#include "c_algorithm.h"


/**
*	@brief	计算指针字符串的长度
*/
int CalculateStringlength(uint8_t *s)
{
    int len=0;
	
    while(*s != '\0')
    {
        len++;
        s++;
    }
    return len;
}
