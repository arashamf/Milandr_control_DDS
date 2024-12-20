
// Includes----------------------------------------------------------------------------------//
#include "pins.h"
#include "HW_Profile.h"
#include "systick.h"
#include "timers.h"

//DEFINES -----------------------------------------------------------------------------------//

//TYPEDEFS ----------------------------------------------------------------------------------//

//STATIC PROTOTYPES--------------------------------------------------------------------------//
static void init_pin( const TPortPin *PortPin );
static void init_DDS_pins(void);
static void InitBiLED( const TBiLED *pBiLed );

//VARIABLES----------------------------------------------------------------------------------//
const TPortPin pins[] = 
{
	{ SPI_SCK_PORT , 	SPI_SCK_PIN  },
	{ SPI_MOSI_PORT, 	SPI_MOSI_PIN },
	{ SPI_CS_PORT  , 	SPI_CS_PIN   },
	{ DDS_SE_PORT	 , 	DDS_SE_PIN   },
	{ DDS_PD_PORT	 , 	DDS_PD_PIN   },
	{ DDS_S0_PORT	 , 	DDS_S0_PIN   },
	{ DDS_S1_PORT  , 	DDS_S1_PIN   }
};


//-----------------------------активация режима включения светодиода-----------------------------//
void SetBiLED( const TBiLED *pBiLed, TBiLEDColor Color )
{
	switch( Color )
	{
		case LED_BLACK: //бесцветный
		//	PORT_ResetBits( pBiLed->Red.PORTx, pBiLed->Red.PORT_Pin );
		//	PORT_ResetBits( pBiLed->Green.PORTx, pBiLed->Green.PORT_Pin );
			break;

		case LED_GREEN: //зелёный
		//	PORT_SetBits( pBiLed->Red.PORTx, pBiLed->Red.PORT_Pin );
		//	PORT_ResetBits( pBiLed->Green.PORTx, pBiLed->Green.PORT_Pin );
			break;

		case LED_RED: //красный
		//	PORT_ResetBits( pBiLed->Red.PORTx, pBiLed->Red.PORT_Pin );
		//	PORT_SetBits( pBiLed->Green.PORTx, pBiLed->Green.PORT_Pin );
			break;

		case LED_YELLOW: //жёлтый
		//	PORT_SetBits( pBiLed->Red.PORTx, pBiLed->Red.PORT_Pin );
		//	PORT_SetBits( pBiLed->Green.PORTx, pBiLed->Green.PORT_Pin );
			break;
	}
}


//---------------------------------------включение пина SPI---------------------------------------//
void Set_SPI_GPIO(const TPortPin *PortPin, uint8_t number_pin)
{
	PORT_SetBits( pins[number_pin].PORTx, pins[number_pin].PORT_Pin );
}

//---------------------------------------включение пина SPI---------------------------------------//
void Reset_SPI_GPIO(const  TPortPin *PortPin, uint8_t number_pin)
{
	PORT_ResetBits( pins[number_pin].PORTx, pins[number_pin].PORT_Pin );
}

//---------------------------инициализация пина управления светодиодом---------------------------//
static void init_pin( const TPortPin *PortPin )  
{
  PORT_InitTypeDef port_init_struct;
	RST_CLK_PCLKcmd( PCLK_BIT(PortPin->PORTx), ENABLE ); // Enable peripheral clocks for PORT
	PORT_StructInit( &port_init_struct ); // Configure pin

	port_init_struct.PORT_Pin   = PortPin->PORT_Pin;
	port_init_struct.PORT_OE    = PORT_OE_OUT; //пин на выход
	port_init_struct.PORT_FUNC  = PORT_FUNC_PORT; //функция вывода
	port_init_struct.PORT_MODE  = PORT_MODE_DIGITAL; //цифровой режим пина
	
	port_init_struct.PORT_SPEED = PORT_SPEED_FAST;

	PORT_Init( PortPin->PORTx, &port_init_struct );
}

//---------------------------инициализация пинов управления светодиодом---------------------------//
static void InitBiLED( const TBiLED *pBiLed )  
{
	init_pin( &pBiLed->Green_left );
	init_pin( &pBiLed->Green_center );
	init_pin( &pBiLed->Green_right );
}

//--------------------------инициализация пинов связи с DDS-генератором--------------------------//
static void init_DDS_pins(void)
{
	uint8_t number_pins = 0;
	uint8_t count = 0;
	
	number_pins = sizeof(pins)/sizeof (pins[0]);
	
	for(count = 0 ; count < number_pins; count++ )
	{	init_pin(&pins[count]);	}	
	
	CS(OFF);
}

//------------------------------------------------------------------------------------------------//
void Func_GPIO_Init(void)
{
	init_DDS_pins();
	InitBiLED(&m_Led);
	SetBiLED(&m_Led, LED_YELLOW); //установка режима светодиода
}

//-----------------------------активация режима включения светодиода-----------------------------//
uint8_t check_pins( uint8_t count )
{
	uint8_t number_pins = 0;
	number_pins = sizeof(pins)/sizeof (pins[0]);
	
	if (count == 0)
	{
		PORT_ResetBits( pins[number_pins-1].PORTx, pins[number_pins-1].PORT_Pin );
	}	
	else
	{	
		PORT_ResetBits( pins[count-1].PORTx, pins[count-1].PORT_Pin );
	}
	PORT_SetBits( pins[count].PORTx, pins[count].PORT_Pin );
	return number_pins;
}

//------------------------------------------------------------------------------------------------//

