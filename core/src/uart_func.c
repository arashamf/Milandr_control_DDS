#include "main.h"
#include "typedef.h"
#include "HW_Profile.h"
#include "ring_buffer.h"
#include "uart_func.h"

//Private functions prototypes -------------------------------------------------//
static void UART_LoLevel_Init(MDR_UART_TypeDef* , uint32_t , uint32_t , UartMode );
static void UART_InitIRQ(IRQn_Type IRQn, uint32_t priority);
static void UARTSetBaud(MDR_UART_TypeDef* , uint32_t , uint32_t );

//Private define --------------------------------------------------------------//

//Private consts --------------------------------------------------------------//

//Private variables -----------------------------------------------------------//
char DBG_buffer[BUF_SIZE];

//----------------------------------------------------------------------------------------//
void DBG_LoLevel_Init(MDR_UART_TypeDef* UARTx, uint32_t UARTx_CLOCK, uint32_t uartBaudRate)
{
  PORT_InitTypeDef GPIOInitStruct; // Структура для инициализации линий ввода-вывода

  UART_InitTypeDef UARTInitStruct;  // Структура для инициализации модуля UART

  RST_CLK_PCLKcmd(UARTx_CLOCK, ENABLE); // Разрешение тактирования порта  и модуля UART

  // Общая конфигурация линий ввода-вывода
  PORT_StructInit (&GPIOInitStruct);
  GPIOInitStruct.PORT_SPEED = PORT_SPEED_MAXFAST;
  GPIOInitStruct.PORT_MODE  = PORT_MODE_DIGITAL;

			// Конфигурация и инициализация линии для передачи данных 
	RST_CLK_PCLKcmd(DBG_CLOCK_Pin_TX , ENABLE);
	GPIOInitStruct.PORT_FUNC  = DBG_PORT_FuncTX ;	
	GPIOInitStruct.PORT_OE    = PORT_OE_OUT;
	GPIOInitStruct.PORT_Pin   = DBG_PORT_PinTX;
	PORT_Init(DBG_PORT_TX	, &GPIOInitStruct);	

  // Конфигурация модуля UART
  UARTInitStruct.UART_BaudRate            = uartBaudRate;                  // Скорость передачи данных
  UARTInitStruct.UART_WordLength          = UART_WordLength8b;             // Количество битов данных в сообщении
  UARTInitStruct.UART_StopBits            = UART_StopBits1;                // Количество STOP-битов
  UARTInitStruct.UART_Parity              = UART_Parity_No;                // Контроль четности
  UARTInitStruct.UART_FIFOMode            = UART_FIFO_OFF;                 // Включение/отключение буфера
  UARTInitStruct.UART_HardwareFlowControl = UART_HardwareFlowControl_TXE;   // Аппаратный контроль за передачей и приемом данных
                                          

  UART_Init(UARTx, &UARTInitStruct);  // Инициализация модуля UART

  UART_BRGInit(UARTx, UART_HCLKdiv1);  // Выбор предделителя тактовой частоты модуля UART
  UART_Cmd(UARTx, ENABLE); // Разрешение работы модуля UART
}

//----------------------------------------------------------------------------------------------//
static void UART_InitIRQ(IRQn_Type IRQn, uint32_t priority)
{
  NVIC_SetPriority(IRQn, priority);  // Назначение приоритета аппаратного прерывания от UART

  NVIC_EnableIRQ(IRQn); // Разрешение аппаратных прерываний от UART
}	

//----------------------------------------------------------------------------------------------//
static void UARTSetBaud(MDR_UART_TypeDef* UARTx, uint32_t baudRate, uint32_t freqCPU)
{
	uint32_t divider = freqCPU / (baudRate >> 2);
	uint32_t CR_tmp = UARTx->CR;
	uint32_t LCR_tmp = UARTx->LCR_H;
	
	while ( (UARTx->FR & UART_FLAG_BUSY) ) 
		{__NOP();}		

  UARTx->CR = 0;
  UARTx->IBRD = divider >> 6;
  UARTx->FBRD = divider & 0x003F;
  UARTx->LCR_H = LCR_tmp;
  UARTx->CR = CR_tmp;
}


//----------------------------------------------------------------------------------------------//
void DBG_PutString (char * str)
{
	char smb;
	
	while  ((smb = *str++) != 0)
	{
		while(UART_GetFlagStatus(DBG_TX, UART_FLAG_BUSY) == SET) {}
		UART_SendData(DBG_TX, smb);
	}
}

//----------------------------------------------------------------------------------------------//
