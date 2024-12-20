
#ifndef __PINS_H__
#define __PINS_H__

#ifdef __cplusplus
extern "C" {
#endif

// Includes -----------------------------------------------------------------------------------//
#include "main.h"
#include "HW_Profile.h"

// Private defines-----------------------------------------------------------------------------//
#define ON 1
#define OFF 0

//Exported types ------------------------------------------------------------------------------//
typedef enum { LED_BLACK=0, LED_RED, LED_GREEN, LED_YELLOW }TBiLEDColor;

typedef enum {CLK_PIN=0, MOSI_PIN, CS_PIN, SE_PIN, PD_PIN, S0_PIN, S1_PIN} SelectPin; 

//--------------------------------
typedef struct 
{
	MDR_PORT_TypeDef *PORTx;
	uint16_t PORT_Pin;		

}TPortPin;

//--------------------------------
typedef struct 
{
	TPortPin	Green_left;		
	TPortPin 	Green_center;
	TPortPin 	Green_right;
}TBiLED;

static TBiLED m_Led = {{LED_GREEN_LEFT_PORT, LED_GREEN_LEFT_PIN}, {LED_GREEN_CENTER_PORT, 
												LED_GREEN_CENTER_PIN}, {LED_GREEN_RIGHT_PORT, LED_GREEN_RIGHT_PIN}}; 

// Function prototypes -----------------------------------------------------------------------//
void SetBiLED( const TBiLED *pBiLed, TBiLEDColor Color ); 
void Set_SPI_GPIO	(const  TPortPin * , uint8_t );
void Reset_SPI_GPIO	(const  TPortPin * , uint8_t );
void Func_GPIO_Init(void);
uint8_t check_pins( uint8_t );
uint8_t SPIBitMode0(uint8_t );

//Macro--------------------------------------------------------------------------------------//
/*#define 	SET_BLACK_LED() 		SetBiLED(&m_Led, LED_BLACK);
#define 	SET_RED_LED() 			SetBiLED(&m_Led, LED_RED);
#define 	SET_GREEN_LED() 		SetBiLED(&m_Led, LED_GREEN);
#define 	SET_YELLOW_LED() 		SetBiLED(&m_Led, LED_YELLOW);*/
#define		TOOGLE_CENTER_GREEN_LED()	((PORT_ReadInputDataBit (LED_GREEN_CENTER_PORT, LED_GREEN_CENTER_PIN)) ? (PORT_ResetBits(LED_GREEN_CENTER_PORT, LED_GREEN_CENTER_PIN)) : (PORT_SetBits(LED_GREEN_CENTER_PORT, LED_GREEN_CENTER_PIN)))
//#define		TOOGLE_RED_LED()		((PORT_ReadInputDataBit (LED_RED_PORT, LED_RED_PIN)) ? (PORT_ResetBits(LED_RED_PORT, LED_RED_PIN)) : (PORT_SetBits(LED_RED_PORT, LED_RED_PIN)))
#define		LEFT_GREEN_LED(x)	((x) ? (PORT_ResetBits(LED_GREEN_LEFT_PORT, LED_GREEN_LEFT_PIN)) : (PORT_SetBits(LED_GREEN_LEFT_PORT, LED_GREEN_LEFT_PIN)))
#define		RIGHT_GREEN_LED(x)	((x) ? (PORT_ResetBits(LED_GREEN_RIGHT_PORT, LED_GREEN_RIGHT_PIN)) : (PORT_SetBits(LED_GREEN_RIGHT_PORT, LED_GREEN_RIGHT_PIN)))
#define 	CS(x) 		((x)? (Reset_SPI_GPIO( pins, CS_PIN)) : (Set_SPI_GPIO( pins, CS_PIN)))
#define 	MOSI(x) 	((x)? (Set_SPI_GPIO( pins, MOSI_PIN)) : (Reset_SPI_GPIO( pins, MOSI_PIN)))
#define 	CLK(x)  	((x)? (Set_SPI_GPIO( pins, CLK_PIN)) : (Reset_SPI_GPIO( pins, CLK_PIN)))
#define 	SE(x)  		((x)? (Set_SPI_GPIO( pins, SE_PIN)) : (Reset_SPI_GPIO( pins, SE_PIN)))
#define 	S0(x)  		((x)? (Set_SPI_GPIO( pins, S0_PIN)) : (Reset_SPI_GPIO( pins, S0_PIN)))
#define 	S1(x)  		((x)? (Set_SPI_GPIO( pins, S1_PIN)) : (Reset_SPI_GPIO( pins, S1_PIN)))

//VARIABLES----------------------------------------------------------------------------------//
extern const TPortPin pins[];

#ifdef __cplusplus
}
#endif
#endif /*__ PINS_H__ */

