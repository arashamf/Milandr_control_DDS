#include "systick.h"
#include "HW_Profile.h"

//static variables------------------------------------------------------------------------//
static uint32_t TicksCounter = 0;
static SysTick_CALLBACK SysTick_CallbackFunction = NULL;

//������������� SysTick � �������� 1 ��---------------------------------------------------//
//void SysTick_Init(SysTick_CALLBACK CallbackFunction)
void SysTick_Init(void)
{
	SysTick->CTRL = 0;
	SysTick->LOAD = (uint32_t)(CPU_CLOCK_VALUE / TICKS_PER_SECOND) - 1;
	SysTick->CTRL = SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk | \
                  SysTick_CTRL_CLKSOURCE_Msk;
	
	//SysTick_CallbackFunction = CallbackFunction;
}

// ����������� �������� SysTick ��� ������������ ����������-------------------------------//
void SysTick_Handler(void)
{
    TicksCounter++;
	/*	if ( SysTick_CallbackFunction != NULL ) 
			{SysTick_CallbackFunction(TicksCounter);}*/
}

// ���������� �������� SysTick------------------------------------------------------------//
uint32_t Get_SysTick(void)
{
	return TicksCounter;
}
 
// �������� � ��--------------------------------------------------------------------------//
void delay_ms(uint32_t ms)
{	
  uint32_t ticks;

	ticks = Get_SysTick();
	while( ( Get_SysTick() - ticks ) < ms )
	{
		__NOP();
	}
}
