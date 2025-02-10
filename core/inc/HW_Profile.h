#ifndef _HW_Profile_H_
#define _HW_Profile_H_

/* Includes ------------------------------------------------------------------*/
#include "MDR32F9Qx_config.h"

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/
#define CPU_CLOCK_VALUE				(80000000UL)	// Частота контроллера 
#define TICKS_PER_SECOND			1000 
//#define TICKS_PER_SECOND  (TICKS_PER_MILLISECOND*1000)	

//-----------------------------------------------------
#define LED_GREEN_LEFT_PIN					PORT_Pin_1
#define LED_GREEN_LEFT_PORT					MDR_PORTC

#define LED_GREEN_CENTER_PIN				PORT_Pin_2
#define LED_GREEN_CENTER_PORT				MDR_PORTC

#define LED_GREEN_RIGHT_PIN					PORT_Pin_7
#define LED_GREEN_RIGHT_PORT				MDR_PORTA

//------------------------------------------------------
#define		SPI_SCK_PORT					MDR_PORTA
#define		SPI_SCK_PIN						PORT_Pin_0

#define		SPI_MOSI_PORT					MDR_PORTA
#define		SPI_MOSI_PIN					PORT_Pin_5

#define		SPI_CS_PORT						MDR_PORTA
#define		SPI_CS_PIN						PORT_Pin_4

#define		DDS_SE_PORT						MDR_PORTB
#define		DDS_SE_PIN						PORT_Pin_0

#define		DDS_PD_PORT						MDR_PORTB
#define		DDS_PD_PIN						PORT_Pin_10

#define		DDS_S0_PORT						MDR_PORTB
#define		DDS_S0_PIN						PORT_Pin_2

#define		DDS_S1_PORT						MDR_PORTB
#define		DDS_S1_PIN						PORT_Pin_1


//UART для отладки--------------------------------------

#define DBG_TX									MDR_UART2
#define DBG_TX_CLOCK 						RST_CLK_PCLK_UART2
	
#define DBG_BAUD_RATE						115200
	
#define DBG_CLOCK_Pin_TX 				RST_CLK_PCLK_PORTF
	
#define DBG_PORT_TX							MDR_PORTF
#define DBG_PORT_PinTX					PORT_Pin_1
#define DBG_PORT_FuncTX  				PORT_FUNC_OVERRID	         

/* Exported macro ------------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */
	
#endif
