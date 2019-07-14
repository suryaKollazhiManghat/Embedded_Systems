
#include<TM4C123GH6PM.h>

int main()
{
  //configure GPIO F blue LED
  SYSCTL->RCGCGPIO |= (1<<5); //enable clock on Port F
  GPIOF->DIR = (1<<2); //make PF2 output
  GPIOF->AFSEL &= ~(1<<2); //disable alternate function on PF2
  GPIOF->DEN = (1<<2); //digital i/o enable PF2
  GPIOF->DATA |= (1<<2); //PF1 set to HIGH (led on)
  
  //To use GPTM the appropirate TIMERn bit must be set
  SYSCTL->RCGCTIMER |= (1<<0); 
  //1: Disable the timer (clear the TnEN bit in the GPTMCTL register).1. Ensure the timer is disabled (the TnEN bit in the GPTMCTL register is cleared) before making
//any changes
  TIMER0->CTL &= ~(1<<0); 
  //2: Write the GPTMCFG witha a value of 0x00000000, to configure periodic timer
  TIMER0->CFG = 0x00000000;
  //3.Configure the TnMR field in the GPTM Timer n Mode Register (GPTMTnMR):
  TIMER0->TAMR |= (0x2<<0);
  // 4. Optionally configure the TnSNAPS, TnWOT, TnMTE, and TnCDIR bits in the GPTMTnMR register
//to select whether to capture the value of the free-running timer at time-out, use an external
//trigger to start counting, configure an additional trigger or interrupt, and count up or down.
  TIMER0->TAMR &= ~(1<<4);
  //5:  Load the start value into the GPTM Timer n Interval Load Register (GPTMTnILR)
  TIMER0->TAILR = 0x00F42400; //16,000,000 this corresponds to the clk speed of this microcontroller-this register is used to load the starting count value into the timer.
  //6: If interrupts are required, set the appropriate bits in the GPTMIMR
  //7: Set the TnEN bit in the GPTMCTL register to enable the timer and start counting
  TIMER0->CTL |= (1<<0);//Enabling timer
  
  while(1)
  {
		
		//8. Poll the GPTMRIS register or wait for the interrupt to be generated (if enabled). In both cases,
 //the status flags are cleared by writing a 1 to the appropriate bit of the GPTM Interrupt Clear
//Register (GPTMICR).

    if((TIMER0->RIS & 0x00000001) == 1) //0 TIMER A HAS NOT TIME OUT
    {
      TIMER0->ICR |= (1<<0);//interrupt clear
      GPIOF->DATA ^= (1<<2);
    }
  }
        
  return 0;
}