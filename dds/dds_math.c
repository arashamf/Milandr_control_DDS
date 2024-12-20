// Includes----------------------------------------------------------------------------------//
#include "dds_math.h"
#include "typedef.h"
#include "uart_func.h"

//DEFINES -----------------------------------------------------------------------------------//

//TYPEDEFS ----------------------------------------------------------------------------------//

//STATIC PROTOTYPES--------------------------------------------------------------------------//

//VARIABLES----------------------------------------------------------------------------------//


//-------------------------------------------------------------------------------------------//
void monosignal_init (MONO_SIGNAL_t * monosignal)
{
	monosignal->divider 	= DIV36;
	monosignal->f_clk 		= FCLK;
	monosignal->f_out1 		= FOUT1;
	monosignal->f_out2 		= FOUT2;
	monosignal->f_out3 		= 3000000;
	monosignal->f_out4 		= 4000000;
	monosignal->increment1 = ((double)monosignal->divider/monosignal->f_clk)*monosignal->f_out1;
	monosignal->increment2 = ((double)monosignal->divider/monosignal->f_clk)*monosignal->f_out2;
	monosignal->increment3 = ((double)monosignal->divider/monosignal->f_clk)*monosignal->f_out3;
	monosignal->increment4 = ((double)monosignal->divider/monosignal->f_clk)*monosignal->f_out4;
}

//-------------------------------------------------------------------------------------------//
void monosignal_calc (MONO_SIGNAL_t * monosignal, MSG_CTRL_DDS_t * DDS_msgTX)
{
	DDS_msgTX->registr_10.inc1 = ((monosignal->increment1&0x0000000007) >> 0);
	DDS_msgTX->registr_10.inc2 = ((monosignal->increment1&0x000007FFF8) >> 3);
	DDS_msgTX->registr_10.inc3 = ((monosignal->increment1&0x007FFF80000) >> 19);
	DDS_msgTX->registr_10.inc4 = ((monosignal->increment1&0x0800000000) >> 35);
}



