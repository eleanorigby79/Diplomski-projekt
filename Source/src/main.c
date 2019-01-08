
 #include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rt_misc.h>

#define NUMEL 10
RCC_ClocksTypeDef RCC_Clocks;
extern volatile uint8_t LED_Toggle;
volatile int user_mode;
#pragma import(__use_no_semihosting_swi)
struct __FILE { int handle; /* Add whatever you need here */ };
FILE __stdout;
FILE __stdin;
int fputc(int ch, FILE *f)
{
ITM_SendChar(ch);
return(ch);
}
int fgetc(FILE *f)
{
char ch;
ch = 1;
return((int)ch);
}
int ferror(FILE *f)
{
/* Your implementation of ferror */
return EOF;
}
void _ttywrch(int ch)
{
ITM_SendChar(ch);
}
void _sys_exit(int return_code)
{
label: goto label; /* endless loop */
}
int main(void)
{ 
  /* Inicijalizacija LED-ica */
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED4);
  STM_EVAL_LEDInit(LED5);
  STM_EVAL_LEDInit(LED6);
	
 
	USART1_Init();
	timer2_init();
	
  /* Palimo zelenu cim pokrenemo program */
  //STM_EVAL_LEDOn(LED4);
       
  /* Konfiguriramo sistemski tak na 1ms */
  RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);

  /* Inicijaliziramo tipku */
  STM_EVAL_PBInit(BUTTON_USER, BUTTON_MODE_EXTI);
	
	/*Pozivamo prvu funkciju u kojoj inicijaliziramo mikrofon */
  WavePlayBack(I2S_AudioFreq_16k); 
	char c[NUMEL];
	int i = 0;
	int32_t coeff;
  while (1) {
//		if(uartGetc(USART1) == 'r') {
//	
//			STM_EVAL_LEDOn(LED6);
//			while(i<NUMEL) {
//				c[i] = uartGetc(USART1);
//				i++;
//				printf("%c",USART1->DR);
//				}
//			coeff = atoi(c);
//				
//		}
	}
}