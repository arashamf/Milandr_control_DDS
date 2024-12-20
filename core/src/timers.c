#include "main.h"
#include "timers.h"

//DEFINES -----------------------------------------------------------------------------------//

//TYPEDEFS ----------------------------------------------------------------------------------//

//VARIABLES----------------------------------------------------------------------------------//
TIMER_CntInitTypeDef TIMER_CntInitStructure;

//STATIC PROTOTYPES--------------------------------------------------------------------------//
static void init_timer_delay (void);

//------------------------------------------------------------------------------------------//
static void init_timer_delay (void)
{

	RST_CLK_PCLKcmd(RST_CLK_PCLK_TIMER2, ENABLE); //������������ �������

	TIMER_DeInit(MDR_TIMER2); //����� �������� �������

	TIMER_CntStructInit(&TIMER_CntInitStructure);

	TIMER_BRGInit(MDR_TIMER2, TIMER_HCLKdiv1);	//������������� �������� ���� ������� � �������������

	TIMER_CntInitStructure.TIMER_Prescaler				= 4 - 1; // 10 ���
	TIMER_CntInitStructure.TIMER_Period						= 8-1;	// TIMERx->ARR
	TIMER_CntInitStructure.TIMER_CounterMode			= TIMER_CntMode_ClkFixedDir;
	TIMER_CntInitStructure.TIMER_CounterDirection	= TIMER_CntDir_Up;	 
	TIMER_CntInitStructure.TIMER_ARR_UpdateMode		= TIMER_ARR_Update_Immediately;   
	TIMER_CntInitStructure.TIMER_IniCounter 			= 0;

	TIMER_CntInit(MDR_TIMER2, &TIMER_CntInitStructure);
}

//------------------------------------------------------------------------------------------//
void delay_us (uint16_t delay)
{
	/*TIMER_CntInitTypeDef TIMER_CntInitStructure;
//	TIMER_DeInit(MDR_TIMER2); //����� �������� �������
	TIMER_CntStructInit(&TIMER_CntInitStructure);

//	TIMER_BRGInit(MDR_TIMER2, TIMER_HCLKdiv1);	//������������� �������� ���� ������� � �������������

	TIMER_CntInitStructure.TIMER_Prescaler				= 80 - 1; // 10 ���
	TIMER_CntInitStructure.TIMER_Period						= (delay-1);	// TIMERx->ARR
	TIMER_CntInitStructure.TIMER_CounterMode			= TIMER_CntMode_ClkFixedDir;
	TIMER_CntInitStructure.TIMER_CounterDirection	= TIMER_CntDir_Up;	 
	TIMER_CntInitStructure.TIMER_ARR_UpdateMode		= TIMER_ARR_Update_Immediately;   
	TIMER_CntInitStructure.TIMER_IniCounter 			= 0;

	TIMER_CntInit(MDR_TIMER2, &TIMER_CntInitStructure);
	
	TIMER_ClearFlag(MDR_TIMER2, TIMER_STATUS_CNT_ARR); // ����� ����� ����������	*/
	TIMER_SetCntAutoreload(MDR_TIMER2, (delay*20-1));
	
	TIMER_SetCounter (MDR_TIMER2, 0);
	TIMER_Cmd(MDR_TIMER2, ENABLE);
	
	while (!(TIMER_GetFlagStatus (MDR_TIMER2, TIMER_STATUS_CNT_ARR))) {}
	TIMER_Cmd(MDR_TIMER2, DISABLE);
	TIMER_ClearFlag(MDR_TIMER2, TIMER_STATUS_CNT_ARR); // ����� ����� ����������	
}


//---------------------������������� ���������� � ����������� ��������---------------------//
void timers_ini (void)
{
	init_timer_delay ();
}


