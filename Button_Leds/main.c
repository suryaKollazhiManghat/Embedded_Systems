#include <TM4C123GH6PM.h>


int main()
{
  SYSCTL->RCGCGPIO = 0x20; //0b0010 0000 Enables PortF which is located on the 5fth bit/field [pg.315]
  
  GPIOF->LOCK = 0x4C4F434B; //Enables write access to the GPIOCR register [pg.645]
  GPIOF->CR = 0xff; //Commit register (GPIOAFSEL, GPIOPUR, GPIOPDR, GPIODEN) [pg.646]
  GPIOF->DIR = 0x0E; //0b0000 1110 Used to configue each individual pin as an input(0) or output(1) [pg.624]
  GPIOF->PUR = 0x11; //0b0001 0001 Enables weak pull-up resistors [pg.638]
  GPIOF->DEN = 0x1F; //0b0001 1111 GPIODEN is the digital enable register. Set pin = 1 to enable pin.
                     //switches and LEDs were enabled here.
  
  while(1)
  {
    switch(GPIOF->DATA & 0x11)
    {
    case 0x00:
        //GPIOF->DATA = (1<<1);
        GPIOF->DATA = 0x02;
        break;
    case 0x01:
        GPIOF->DATA = (1<<2);
        break;
    case 0x10:
        GPIOF->DATA = (1<<3);
        break;
    default:
      GPIOF->DATA &= ~((1<<1) | (1<<2) | (1<<3));
      break;
    }
  }
  
}