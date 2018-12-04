#ifndef __APP_TIM_H__
#define __APP_TIM_H__



typedef struct{
	uint32_t ms1Cnt;
	uint32_t ms3Cnt;
    uint32_t ms4Cnt;
	uint32_t ms20Cnt;
	uint32_t ms80Cnt;
    uint32_t ms100Cnt;
    uint32_t ms250Cnt;
    uint32_t ms500Cnt;
    uint32_t ms1000Cnt;
    uint8_t ms1;
	uint8_t ms3;
    uint8_t ms4;
    uint8_t ms20;
	uint8_t ms80;
    uint8_t ms100;
    uint8_t ms250;
    uint8_t ms500;
    uint8_t ms1000;
}SYSFlagType;
extern SYSFlagType RunFlag;

typedef struct{
    uint32_t capture;
    uint32_t count;
}IntervalTimeType;

void RunFlagInit(void);
void RunFlagHandleTask(void);




#endif
