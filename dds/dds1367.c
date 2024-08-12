// Includes----------------------------------------------------------------------------------//
#include "dds1367.h"
#include "systick.h"
#include "timers.h"
#include "uart_func.h"
#include "pins.h"

//DEFINES -----------------------------------------------------------------------------------//
#define 		INT_REF		(19UL)
#define 		LCHM			(14UL)
#define 		DIRECT		(15UL)
//TYPEDEFS ----------------------------------------------------------------------------------//

//STATIC PROTOTYPES--------------------------------------------------------------------------//
static uint8_t SPI_TX_Mode0_MSB(uint64_t , uint16_t );
static uint8_t SPI_TX_Mode0_LSB(uint64_t , uint8_t );
static void dds_put_msg (uint8_t , uint64_t );

//VARIABLES----------------------------------------------------------------------------------//

//------------------------------------------------------------------------------------------//
static uint8_t SPI_TX_Mode0_MSB(uint64_t data_byte, uint16_t number_bit)
{       
	uint8_t bit; 
	uint64_t mask = 1;
	
	if (number_bit >= 1)
	{	mask = (mask << (number_bit-1)); }	//
	else
	{	return 0;	}
	
	for (bit = 0; bit < number_bit; bit++)  // записать MOSI по спаду предыдущего тактового импульса 
	{        
		if (data_byte & mask )
		{	MOSI(ON);	}
		else
		{	MOSI(OFF);	}
		data_byte <<= 1;
       
		delay_us (3);  //ожидание половины тактового периода, перед тем как сгенерировать фронт 
		CLK(ON);
        
		delay_us (3); // ожидание половины тактового периода, перед тем как сгенерировать спад 
		CLK(OFF);
	}
	
	return data_byte;
}

//------------------------------------------------------------------------------------------//
static uint8_t SPI_TX_Mode0_LSB(uint64_t data_byte, uint8_t number_bit)
{       
	uint8_t bit;
	for (bit = 0; bit < number_bit; bit++)  // записать MOSI по спаду предыдущего тактового импульса 
	{        
		if (data_byte & 0x01)
		{	MOSI(ON);	}
		else
		{	MOSI(OFF);	}
		data_byte <<= 1;
       
		delay_us (3);  //ожидание половины тактового периода, перед тем как сгенерировать фронт 
		CLK(ON);
        
		delay_us (3); // ожидание половины тактового периода, перед тем как сгенерировать спад 
		CLK(OFF);
	}
	return data_byte;
}

//------------------------------------------------------------------------------------------//
void registr_activated (uint8_t number_registr)
{
	uint8_t count = 0;
	
	if (number_registr == addr0)
	{	return;	}
	
	
	switch (number_registr)
	{
		case addr0:
			break;
		
		case addr10:
			S1(OFF);
			S0(OFF);
			break;
		
		case addr14:
			S1(OFF);
			S0(ON);
			break;
				
		case addr18:
			S1(ON);
			S0(OFF);
			break;
						
		case addr1C:
			S1(ON);
			S0(ON);	
			break;
		
		default:
			break;
	}
	SE(ON);
	//delay_us (2);	
	for (count = 0; count < 50; count++)
	{}
	SE(OFF);
}


//------------------------------------------------------------------------------------------//
void dds_msg (MSG_CTRL_DDS_t * ptr_DDSmsg, uint8_t number_registr)
{
	char count;
	uint8_t lenght_msg = sizeof(ptr_DDSmsg->RAW_data)-1;
	
	CS(ON);	
	for (count = 0; count <= lenght_msg; count++)
	{
		if (count < (lenght_msg))
		{
			SPI_TX_Mode0_MSB(ptr_DDSmsg->RAW_data[lenght_msg-count], 8); 
		}
		else
		{
			if (count == (lenght_msg))
			{	
				SPI_TX_Mode0_MSB(ptr_DDSmsg->RAW_data[lenght_msg-count], 1);
			}	
		}
	}
	MOSI(OFF);
	CS (OFF);	
	
}

//------------------------------------------------------------------------------------------//
static void dds_put_msg (uint8_t number_registr, uint64_t tmp)
{
	CS(ON);	

	SPI_TX_Mode0_MSB(number_registr, 5); 
	SPI_TX_Mode0_MSB(tmp, 0x24); 
	
	MOSI(OFF);
	CS (OFF);	
}

//------------------------------------------------------------------------------------------//
void dds_init_registr0 (void)
{
	uint64_t tmp = 0;
	
	tmp |= (1 << INT_REF);
	tmp |= (FSC << 12);
	//tmp |= 0x1;
	dds_put_msg (addr0, tmp);
}

//------------------------------------------------------------------------------------------//
void dds_init_registr10 (MONO_SIGNAL_t * ptr)
{
	uint64_t tmp = ptr->increment1;
	dds_put_msg (addr10, tmp);
}

//------------------------------------------------------------------------------------------//
void dds_init_registr12 (void)
{
	uint64_t tmp = 0;
	registr_activated (addr10);
	dds_put_msg (addr11, tmp);
	dds_put_msg (addr12, tmp);
	
	registr_activated (addr14);
	dds_put_msg (addr15, tmp);
	dds_put_msg (addr16, tmp);
	
	registr_activated (addr18);
	dds_put_msg (addr19, tmp);
	dds_put_msg (addr1A, tmp);
	
	registr_activated (addr1C);
	dds_put_msg (addr1D, tmp);
	dds_put_msg (addr1E, tmp);
}

//------------------------------------------------------------------------------------------//
void dds_init_registr14 (MONO_SIGNAL_t * ptr)
{
	uint64_t tmp = ptr->increment2;
	dds_put_msg (0x14, tmp);
}

//------------------------------------------------------------------------------------------//
void dds_init_registr (uint8_t number_registr, uint64_t tmp)
{
	dds_put_msg (number_registr, tmp);
}

//------------------------------------------------------------------------------------------//

