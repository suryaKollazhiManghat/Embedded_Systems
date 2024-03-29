#include "C:\Keil_v5\ARM\PACK\Keil\TM4C_DFP\1.1.0\Device\Include\TM4C123\TM4C123GH6PM.h"
#include <string.h>
#include <stdlib.h>


char readChar(void);
void printChar(char c);
void printString(char * string);
char* readString(char delimiter);
//void SystemInit();

int main(void) 
{
	//	1. Enable the UART0 module using the RCGCUART register (see page 344). Receiver Transmitter Run mode clock Gating control
	 SYSCTL->RCGCUART |= (1<<0); 
    
    // 2. Enable the clock to the appropriate GPIO module via the RCGCGPIO register (see page 340).Enabled PORTA
    // To find out which GPIO port to enable, refer to Table 23-5 on page 1351.
    SYSCTL->RCGCGPIO |= (1<<0); 
    
    // 3. Set the GPIO AFSEL bits for the appropriate pins (see page 671). To determine which GPIOs to
    // configure, see Table 23-4 on page 1344
    GPIOA->AFSEL = (1<<1)|(1<<0); 
    
    // 4. Configure the GPIO current level and/or slew rate as specified for the mode selected (see
    // page 673 and page 681
    
    // 5. Configure the PMCn fields in the GPIOPCTL register to assign the UART signals to the appropriate
    // pins (see page 688 and Table 23-5 on page 1351).PORTF 0,1 Configured to UART.
    GPIOA->PCTL = (1<<0)|(1<<4);  
    
    GPIOA->DEN = (1<<0)|(1<<1); 
	UART0->CTL &= ~(1<<0);
    
    // 2. Write the integer portion of the BRD to the UARTIBRD register
    UART0->IBRD = 104;      
    // 3. Write the fractional portion of the BRD to the UARTFBRD register.
    UART0->FBRD = 11; 
    
    // 4. Write the desired serial parameters to the UARTLCRH register (in this case, a value of 0x0000.0060)
    UART0->LCRH = (0x3<<5)|(1<<4);     // 8-bit, no parity, 1-stop bit
    
    // 5. Configure the UART clock source by writing to the UARTCC register
    UART0->CC = 0x0;          

    // 6. Optionally, configure the �DMA channel (see �Micro Direct Memory Access (�DMA)� on page 585)
    // and enable the DMA option(s) in the UARTDMACTL register
    
    // 7. Enable the UART by setting the UARTEN bit in the UARTCTL register.
    UART0->CTL = (1<<0)|(1<<8)|(1<<9); 
    
    // Configure LED pins
		
    SYSCTL->RCGCGPIO |= (1<<5); // enable clock on PortF
    GPIOF->DIR = (1<<1)|(1<<2)|(1<<3);  // make LED pins (PF1, PF2, and PF3) outputs
    GPIOF->DEN = (1<<1)|(1<<2)|(1<<3); // enable digital function on LED pins
     GPIOF->DATA |= ((1<<1)|(1<<2)|(1<<3)); // turn off leds
		 
while(1)
    {
        printString("Type");
        char c = readChar();
        printChar(c);
        printString("\n\r");
        
        switch(c)
        {
        case 'r':
          GPIOF->DATA = (1<<1);
            break;
        case 'b':
          GPIOF->DATA = (1<<2);
            break;
        case 'g':
          GPIOF->DATA = (1<<3);
            break;
        default:
          GPIOF->DATA &= ~((1<<1)|(1<<2)|(1<<3));
            break;
        }
     }
    
}

char readChar(void)  
{
    char c;
    while((UART0->FR & (1<<4)) != 0)
		{ c = UART0->DR;                 
    return c;}                    
}

void printChar(char c)  
{
  while((UART0->FR & (1<<5)) != 0)
	{
    UART0->DR = c;           }
}

void printString(char * string)
{
  while(*string!='\n')
  {
    printChar(*(string++));
  }
}