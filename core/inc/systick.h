#ifndef __SYSTICK_H
#define __SYSTICK_H

#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*SysTick_CALLBACK)(uint32_t);

//void SysTick_Init(SysTick_CALLBACK CallbackFunction); // ������������� SysTick � �������� 1 ��
void SysTick_Init(void);
uint32_t Get_SysTick(void);	// ���������� �������� SysTick
void delay_ms (uint32_t ms);	// �������� � ��

#ifdef __cplusplus
}
#endif

#endif
