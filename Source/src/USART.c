/* usart.c */
#include <USART.h>
#include <stm32f4xx.h> // common stuff
#include <stm32f4xx_gpio.h> // gpio control
#include <stm32f4xx_rcc.h> // reset anc clocking
#include <usart.h>

// RX FIFO buffer
uint8_t RX_BUFFER[BUFSIZE];
int RX_BUFFER_HEAD, RX_BUFFER_TAIL;
// TX state flag
uint8_t TxReady;


// init USART1
void USART1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	// enable peripheral clocks (note: different bus interfaces for each peripheral!)
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	// map port B pins for alternate function
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; // Pins 6 (TX) and 7 (RX) will be used for USART1
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF; // GPIO pins defined as alternate function
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; // I/O pins speed (signal rise time)
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP; // push-pull output
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP; // activates pullup resistors
	GPIO_Init(GPIOB, &GPIO_InitStruct); // set chosen pins
	
	// set alternate function to USART1 (from multiple possible alternate function choices)
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_USART1); // pins will automatically be assigned to TX/RX - refer to datasheet to see AF mappings
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_USART1);
	
	// use USART_InitStruct to config USART1 peripheral
	USART_InitStruct.USART_BaudRate = BAUDRATE; // set baudrate from define
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;// 8 data bits
	USART_InitStruct.USART_StopBits = USART_StopBits_1; // 1 stop bit
	USART_InitStruct.USART_Parity = USART_Parity_No; // no parity check
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // no HW control flow
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; // enable both character transmit and receive
	USART_Init(USART1, &USART_InitStruct); // set USART1 peripheral
	
	// set interrupt triggers for USART1 ISR (but do not enable USART1 interrupts yet)
	USART_ITConfig(USART1, USART_IT_TXE, DISABLE);// should be disbled
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); // character received (to trigger buffering of new character)
	TxReady = 1; // USART1 is ready to transmit
	RX_BUFFER_HEAD = 0; RX_BUFFER_TAIL = 0; // clear rx buffer
	
	// prepare NVIC to receive USART1 IRQs
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; // configure USART1 interrupts
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;// max. priority
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; // max. priority
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; // enable USART1 interrupt in NVIC
	NVIC_Init(&NVIC_InitStructure); // set NVIC for USART1 IRQ
	
	// enables USART1 interrupt generation
	USART_Cmd(USART1, ENABLE);
}


void uartClose(USART_TypeDef* USARTx) {
		
	USART_Cmd(USARTx, DISABLE);
	
	}


void uartPutString(volatile char *c, USART_TypeDef* USARTx) {
	
		while(*c) {
			while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
			
			USART_SendData(USARTx, *c);
			
			*c++;
			}
	}
	
	
int uartGetc(USART_TypeDef* USARTx) {
	
	while(USART_GetFlagStatus(USARTx, USART_FLAG_RXNE) == RESET);
	
	return USART_ReceiveData(USARTx);
	
	}

void uartSendInt(uint8_t *c, USART_TypeDef* USARTx) {
	
	while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
	USART_SendData(USARTx, *c);
	
	}

int USART1_IRQHandler(void) {	
		
		if(USART_GetITStatus(USART1, USART_IT_RXNE)) {
			
			char t = USART_ReceiveData(USART1);
			uartPutString(&t, USART1);
			
			USART_ClearITPendingBit(USART1, USART_IT_RXNE);
			return 1;
		}
		
		else
			return -1;
	}		