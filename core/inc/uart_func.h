#ifndef UART_FUNC_H
#define UART_FUNC_H

// Includes -------------------------------------------------------------------------//
#include "main.h"
#include "typedef.h"
#include "ring_buffer.h"

// Private defines------------------------------------------------------------------//
#define BUF_SIZE 50

//Private prototypes---------------------------------------------------------------//
void DBG_LoLevel_Init(MDR_UART_TypeDef* , uint32_t , uint32_t );
void DBG_PutString (char *);

//Private variables ---------------------------------------------------------------//
extern char DBG_buffer[];
//char DBG_buffer[BUF_SIZE];
#endif
