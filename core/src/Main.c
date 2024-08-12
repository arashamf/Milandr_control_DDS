//Includes --------------------------------------------------------------------------------//

#include "main.h"
#include "HW_Profile.h"
#include "systick.h"
#include "pins.h"
#include "typedef.h"
#include "timers.h"
#include "uart_func.h"
#include "dds1367.h"
#include "dds_math.h"

//Private typedef ----------------------------------------------------------------------//

//Private define -----------------------------------------------------------------------//

//Private macro ------------------------------------------------------------------------//

//Variables ----------------------------------------------------------------------------//

//Private variables -------------------------------------------------------------------//
uint8_t i;
MSG_CTRL_DDS_t msg_DDS = {0};
MONO_SIGNAL_t set_monosignal = {0};

//Private function prototypes ----------------------------------------------------------//
static void CPUClk80MHz_Init(void);
bool ClockConfigure (void);
void InitWatchDog(void);
void StartUpDelay( void );
void test_CLK (void);

//Private functions -------------------------------------------------------------------//
#ifdef __USE_DBG
	/*#define ITM_STIM_U32 (*(volatile unsigned int*)0xE0000000)    // Stimulus Port Register word acces
	#define ITM_STIM_U8  (*(volatile         char*)0xE0000000)    // Stimulus Port Register byte acces
	#define ITM_ENA      (*(volatile unsigned int*)0xE0000E00)    // Trace Enable Ports Register
	#define ITM_TCR      (*(volatile unsigned int*)0xE0000E80)    // Trace control register

int fputc( int c, FILE *f ) 
{
	if( (ITM_TCR & 1) && (ITM_ENA & 1) ) 	// Check if ITM_TCR.ITMENA is set, Check if stimulus port is enabled
	{ 		
		while ((ITM_STIM_U8 & 1) == 0) {}; //Wait until STIMx is ready,
		ITM_STIM_U8 = (char)c; // then send data
	}
	return( c );
}*/
#endif

//-------------------------------------------------------------------------------------//
int main( void )
{
	CPUClk80MHz_Init(); //������������� PLL
	SysTick_Init();
	Func_GPIO_Init();
	SET_RED_LED() ;
	timers_ini ();
	
	registr_activated (addr10);
	dds_init_registr0 ();
	
	registr_activated (addr14);
	dds_init_registr0 ();
	
	registr_activated (addr18);
	dds_init_registr0 ();
	
	registr_activated (addr1C);
	dds_init_registr0 ();
	
	#ifdef __USE_DBG
	DBG_LoLevel_Init(DBG_TX, DBG_TX_CLOCK, DBG_BAUD_RATE);
	snprintf(DBG_buffer, BUF_SIZE,"%llu_%llu_%llu_%llu_%llu\r\n", set_monosignal.increment1, set_monosignal.increment2,
	set_monosignal.f_clk, set_monosignal.f_out1, set_monosignal.f_out2); 
	DBG_PutString (DBG_buffer);
	#endif
	
	
	monosignal_init (&set_monosignal);
	
	#ifdef __USE_DBG
	snprintf(DBG_buffer, BUF_SIZE,"%llu_%llu_%llu_%llu_%llu\r\n", set_monosignal.increment1, set_monosignal.increment2,
	set_monosignal.f_clk, set_monosignal.f_out1, set_monosignal.f_out2); 
	DBG_PutString (DBG_buffer);
	#endif
	
	//dds_init_registr12 ();
	
/*	dds_init_registr (addr10, set_monosignal.increment1);
	registr_activated (addr10);
	delay_ms (500);
	
	dds_init_registr (addr14, set_monosignal.increment2);
	registr_activated (addr14);
	delay_ms (500);
	
	dds_init_registr (addr18, set_monosignal.increment3);
	registr_activated (addr18);
	delay_ms (500);
	
	dds_init_registr (addr1C, set_monosignal.increment4);
	registr_activated (addr1C);
	delay_ms (500);*/
	
	
	
	#ifdef __USE_IWDG
		InitWatchDog(); //������������� ����������� �������
	#endif
	
	
	while(1)
	{

		dds_init_registr (addr10, set_monosignal.increment1);
		registr_activated (addr10);
//		delay_ms (100);
		delay_us (7143);

		dds_init_registr (addr10, set_monosignal.increment2);
		registr_activated (addr10);
//		delay_ms (100);
		delay_us (7143);
		
		TOOGLE_RED_LED();
		TOOGLE_GREEN_LED();
		
		#ifdef __USE_IWDG	
			IWDG_ReloadCounter(); //������������ ����������� �������
		#endif
	}
}

//-------------------------------------------------------------------------------------//
static void CPUClk80MHz_Init(void)
{
	uint8_t n = 0;
	ErrorStatus ret;
	
  RST_CLK_HSEconfig(RST_CLK_HSE_ON); // Enable HSE 
  while ( n < HSE_ON_ATTEMPTS ) //�������� ���������� HSE
  {
		ret = RST_CLK_HSEstatus(); 		
		if ( ret == SUCCESS ) 
			{break;}	
		else
			{n++;}
  }
	if ( ret != SUCCESS ) 
		{SET_RED_LED();}

  /* CPU_C1_SEL = HSE */
  RST_CLK_CPU_PLLconfig(RST_CLK_CPU_PLLsrcHSEdiv1, RST_CLK_CPU_PLLmul10); //Select HSE clock as CPU_PLL input clock source & set PLL multiplier
 
	RST_CLK_CPU_PLLcmd(ENABLE); //enable CPU_PLL
  while (RST_CLK_CPU_PLLstatus() != SUCCESS) {}; //�������� ���������� CPU_PLL 

	RST_CLK_PCLKcmd(RST_CLK_PCLK_EEPROM, ENABLE); 	// Enables the RST_CLK_PCLK_EEPROM 

  EEPROM_SetLatency(EEPROM_Latency_3);   // Sets the code latency value 

  RST_CLK_CPUclkPrescaler(RST_CLK_CPUclkDIV1);   // CPU_C3_SEL = CPU_C2_SEL 

  RST_CLK_CPU_PLLuse(ENABLE);   // CPU_C2_SEL = PLL 

  RST_CLK_CPUclkSelection(RST_CLK_CPUclkCPU_C3);   // HCLK_SEL = CPU_C3_SEL 
}

//-------------------------------------------------------------------------------------//
bool ClockConfigure (void)
{
  uint32_t cntr = 0;

	cntr = 0;
	RST_CLK_HSEconfig(RST_CLK_HSE_ON); //switch on HSE clock generator
  while(RST_CLK_HSEstatus() != SUCCESS && cntr++ < 0x40000) {};//�������� ���������� ���������� HSE

  if(RST_CLK_HSEstatus() != SUCCESS) //��������� ������� ���������� HSE
	{	return false;	}
	
	#define PLL_MULL_VALUE (CPU_CLOCK_VALUE / HSE_Value - 1) //80���/8���=10
	RST_CLK_CPU_PLLconfig (RST_CLK_CPU_PLLsrcHSEdiv1, PLL_MULL_VALUE ); //Select HSE clock as CPU_PLL input clock source & set PLL multiplier

	RST_CLK_CPU_PLLcmd(ENABLE); //enable CPU_PLL
	
	while(RST_CLK_CPU_PLLstatus() != SUCCESS && cntr++ < 0x40000) {};//�������� ���������� CPU_PLL 

  if(RST_CLK_CPU_PLLstatus() != SUCCESS) //��������� ������� CPU_PLL 
	{	return false;	}

	RST_CLK_PCLKcmd(RST_CLK_PCLK_EEPROM, ENABLE); //enable the RST_CLK_PCLK_EEPROM 

	// Set the code latency value
	#if CPU_CLOCK_VALUE < 25000000UL					// Freqency < 25MHz
		#define EEPROM_LATENCY_VALUE EEPROM_Latency_0
	#elif CPU_CLOCK_VALUE < 50000000UL					// 25MHz <= Freqency < 50MHz
		#define EEPROM_LATENCY_VALUE EEPROM_Latency_1				
	#elif CPU_CLOCK_VALUE < 75000000UL					// 50MHz <= Freqency < 75MHz
		#define EEPROM_LATENCY_VALUE EEPROM_Latency_2				
	#elif CPU_CLOCK_VALUE < 100000000UL					// 75MHz <= Freqency < 100MHz
		#define EEPROM_LATENCY_VALUE EEPROM_Latency_3				
	#elif CPU_CLOCK_VALUE < 125000000UL					// 100MHz <= Freqency < 125MHz
		#define EEPROM_LATENCY_VALUE EEPROM_Latency_4				
	#elif CPU_CLOCK_VALUE < 150000000UL					// 125MHz <= Freqency < 150MHz
		#define EEPROM_LATENCY_VALUE EEPROM_Latency_5				
	#else												// 150MHz <= Freqency
		#define EEPROM_LATENCY_VALUE EEPROM_Latency_7
	#endif
	
	EEPROM_SetLatency(EEPROM_LATENCY_VALUE); //sets the code latency value

	RST_CLK_CPUclkPrescaler(RST_CLK_CPUclkDIV1); //set CPU_C3_prescaler to 1

	RST_CLK_CPU_PLLuse(ENABLE); 	//set CPU_C2_SEL to CPU_PLL output instead of CPU_C1 clock
	
	RST_CLK_CPUclkSelection(RST_CLK_CPUclkCPU_C3); //select CPU_C3 clock on the CPU clock MUX 

	SystemCoreClockUpdate(); //get core clock frequency  
	
	return true;
}

//-------------------------------------------------------------------------------------//
#ifdef __USE_IWDG
void InitWatchDog( void )
{
	RST_CLK_PCLKcmd(RST_CLK_PCLK_IWDG,ENABLE);
	IWDG_WriteAccessEnable();
	IWDG_SetPrescaler(IWDG_Prescaler_64);	// 40000/64=625 ��
	while( IWDG_GetFlagStatus( IWDG_FLAG_PVU ) != 1 ){}
	IWDG_SetReload (2500);	// 2500 / 652 = 4 ���
	IWDG_Enable();
	IWDG_ReloadCounter();
}
#endif


//-------------------------------------------------------------------------------------//
void StartUpDelay( void )
{
	// �������� ~1.5 �������
	//
	SysTick->LOAD = HSI_Value * 1.5 ;
	SysTick->CTRL = SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_CLKSOURCE_Msk;
	while( !(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) );
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

//-------------------------------------------------------------------------------------//
void test_CLK (void)
{
	{
		CLK(ON);
		delay_us (6);
		CLK(OFF);
		delay_us (6);
	}
}

//-------------------------------------------------------------------------------------------------//
#if (USE_ASSERT_INFO == 1)
void assert_failed(uint32_t file_id, uint32_t line)
{
	while (1)
	{
	}
}
#elif (USE_ASSERT_INFO == 2)
void assert_failed(uint32_t file_id, uint32_t line, const uint8_t* expr)
{
	while (1)
	{
	}
}
#endif /* USE_ASSERT_INFO */

