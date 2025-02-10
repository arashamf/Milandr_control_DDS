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
//#define PLL_MULL_VALUE (CPU_CLOCK_VALUE / HSE_Value - 1) //80МГц/8МГц=10
#define PLL_MULL_VALUE (CPU_CLOCK_VALUE / 10 - 1) //80МГц/8МГц=10

//Private macro ------------------------------------------------------------------------//

//Variables ----------------------------------------------------------------------------//

//Private variables -------------------------------------------------------------------//
uint8_t i = 0;
MSG_CTRL_DDS_t msg_DDS = {0};
MONO_SIGNAL_t set_monosignal = {0};
uint8_t flag = 0;

//Private function prototypes ------------------------------------------------------//
static bool HSI_80MHz_Init(void);
static bool ClockConfigure (void);
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
	flag = HSI_80MHz_Init(); //инициализация PLL
//	flag = ClockConfigure ();
	SysTick_Init();
	Func_GPIO_Init();
	LEFT_GREEN_LED(OFF);
	RIGHT_GREEN_LED(OFF);
	timers_ini ();
	
	#ifdef __USE_DBG
		DBG_LoLevel_Init(DBG_TX, DBG_TX_CLOCK, DBG_BAUD_RATE);
	#endif
	
	if (flag == 0)
	{
		RIGHT_GREEN_LED(ON);
		while(1) {}
	}
	
	registr_activated (addr10);
	reg0_data_init (&msg_DDS);
	dds_init_registr (addr0, msg_DDS.RAW_data);
	monosignal_init (&set_monosignal);	
	dds_init_registr (addr10, set_monosignal.increment1);
	reg11_data_init (&msg_DDS);
	dds_init_registr (addr11, msg_DDS.RAW_data);
	reg12_data_init (&msg_DDS);
	dds_init_registr (addr12, msg_DDS.RAW_data);
	delay_us (1000);
	
	registr_activated (addr14);
	dds_init_registr (addr10, set_monosignal.increment2);	
	reg11_data_init (&msg_DDS);
	dds_init_registr (addr15, msg_DDS.RAW_data);
	reg12_data_init (&msg_DDS);
	dds_init_registr (addr16, msg_DDS.RAW_data);
	delay_us (1000);
	
	LEFT_GREEN_LED(OFF);
	RIGHT_GREEN_LED(OFF);
	#ifdef __USE_IWDG
		InitWatchDog(); //инициализация сторожевого таймера
	#endif

	
	while(1)
	{

		dds_init_registr (addr10, set_monosignal.increment1);
		registr_activated (addr10);
		delay_us (7143);

		dds_init_registr (addr10, set_monosignal.increment2);
		registr_activated (addr10);
		delay_us (7143);

	/*	dds_init_registr (addr14, set_monosignal.increment1);
		registr_activated (addr14);
		delay_us (7143);*/
	//	TOOGLE_CENTER_GREEN_LED();
		
		#ifdef __USE_IWDG	
			IWDG_ReloadCounter(); //перезагрузка сторожевого таймера
		#endif
	}
}

//-------------------------------------------------------------------------------------//
static bool HSI_80MHz_Init(void)
{
	uint32_t cntr = 0;
	ErrorStatus ret;
	
/*  RST_CLK_HSEconfig(RST_CLK_HSE_ON); // Enable HSE 
  while ( cntr < HSE_ON_ATTEMPTS ) //ожидание готовности HSE
  {
		ret = RST_CLK_HSEstatus(); 		
		if ( ret == SUCCESS ) 
		{	break;	}	
		else
		{	cntr++;	}
  }
	if ( ret != SUCCESS ) 
	{	
		//SET_RED_LED();			
		return false;
	}*/

  /* CPU_C1_SEL = HSI */
  RST_CLK_CPU_PLLconfig(RST_CLK_CPU_PLLsrcHSIdiv1 , RST_CLK_CPU_PLLmul8 ); //Select HSI clock as CPU_PLL input clock source & set PLL multiplier
 
	RST_CLK_CPU_PLLcmd(ENABLE); //enable CPU_PLL
  while ((RST_CLK_CPU_PLLstatus() != SUCCESS )&& (cntr++ < 0x40000)) {}; //ожидание готовности CPU_PLL 
  if(RST_CLK_CPU_PLLstatus() != SUCCESS) //получение статуса CPU_PLL 
	{	return false;	}
		
	RST_CLK_PCLKcmd(RST_CLK_PCLK_EEPROM, ENABLE); 	// Enables the RST_CLK_PCLK_EEPROM 
  EEPROM_SetLatency(EEPROM_Latency_3);   // Sets the code latency value 

	RST_CLK_CPU_PLLuse(ENABLE);   								// CPU_C2_SEL = PLL 
  RST_CLK_CPUclkPrescaler(RST_CLK_CPUclkDIV1);   // CPU_C3_SEL = CPU_C2_SEL 
  RST_CLK_CPUclkSelection(RST_CLK_CPUclkCPU_C3);   // HCLK_SEL = CPU_C3_SEL 
	return true;
}

//-------------------------------------------------------------------------------------//
static bool ClockConfigure (void)
{
  uint32_t cntr = 0;

	RST_CLK_HSEconfig(RST_CLK_HSE_ON); //switch on HSE clock generator
  while(RST_CLK_HSEstatus() != SUCCESS && cntr++ < 0x40000) {};//ожидание готовности генератора HSE

  if(RST_CLK_HSEstatus() != SUCCESS) //получение статуса генератора HSE
	{	return false;	}
	
	RST_CLK_CPU_PLLconfig (RST_CLK_CPU_PLLsrcHSEdiv1, PLL_MULL_VALUE ); //Select HSE clock as CPU_PLL input clock source & set PLL multiplier

	RST_CLK_CPU_PLLcmd(ENABLE); //enable CPU_PLL	
	while(RST_CLK_CPU_PLLstatus() != SUCCESS && cntr++ < 0x40000) {};//ожидание готовности CPU_PLL 
  if(RST_CLK_CPU_PLLstatus() != SUCCESS) //получение статуса CPU_PLL 
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
	IWDG_SetPrescaler(IWDG_Prescaler_64);	// 40000/64=625 Гц
	while( IWDG_GetFlagStatus( IWDG_FLAG_PVU ) != 1 ){}
	IWDG_SetReload (2500);	// 2500 / 652 = 4 сек
	IWDG_Enable();
	IWDG_ReloadCounter();
}
#endif


//-------------------------------------------------------------------------------------//
void StartUpDelay( void )
{
	// Задержка ~1.5 секунды
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

