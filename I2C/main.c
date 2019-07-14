#include <TM4C123GH6PM.h>
uint8_t leds = 0x00;

void setSlaveAddress(uint8_t slaveAddress)
{
  //MSA register consists of eight bits: 7 address bits and 1 Receive/Send bit, which determines if the next operation is Receive(1) or Transmit(0)
  //slaveAddress = 0b _001.0000
  I2C1->MSA = (slaveAddress<<1); //Specify the slave address of the master
}

void setRW(uint8_t mode)
{
  //NEXT operation is to Transmit
  if(mode == 0)
  {
    I2C1->MSA &= ~( 1<<0); //Clear bit, transmitting data
  }
  else
  {
    I2C1->MSA |= (1<<0); //set bit, receiving data
  }
}

void writeByte(uint8_t dataByte, uint8_t conditions)
{
  //This byte contains the data transferred during a transaction
  I2C1->MDR = dataByte;
  I2C1->MCS = conditions;
  
  while( (I2C1->MCS & (1<<0)) != 0);//BE HERE WHEN BUSY-until the txn completes.this is done by polling
  
  if( (I2C1->MCS & (1<<1) ) != 0)  //checking error
  {
    if( (I2C1->MCS & (1<<4)) == 1)
			
		{ 
			//controller lost arbitration
   	}
    else
    {
      I2C1->MCS = (1<<2);   //The controller generates the STOP condition
      while( (I2C1->MCS & (1<<0)) != 0);  //busy
    }
  }        
}

int main()
{
  //Step 1
  //Enable the I2C clock using the RCGCI2C register in the System Control module.
  SYSCTL->RCGCI2C = (1<<1); //using I2C Module 1
  //Step 2
	//Enable the clock to the appropriate GPIO module via the RCGCGPIO register in the System
//Control module
  SYSCTL->RCGCGPIO = (1<<0); //Module 1 uses port A => we must enable the clock on Port A
  
  //Step 3
  GPIOA->AFSEL = (1<<6)|(1<<7); //Enabling the alternate functions for pin 6 and 7
  GPIOA->DEN = (1<<6)|(1<<7); //They are digital signals
  
  //Step 4
  //Setting a bit in this register enables the open-drain configuration of the corresponding GPIO pad
  //When this bit is enabled, it should also be set in the GPIODEN register.
  //PA7 corresponds to SDA.
  GPIOA->ODR = (1<<7);
  
  //Step 5
  //GPIOA->PCTL &= ~0xFF000000; //This line is not required
  //Specify which alternate function you want to use, see Table 21-5,GPIO Port Mux Control
  GPIOA->PCTL = (3<<28)|(3<<24); //put a 3 in PMC7 and PMC6 (pins PA7 and PA6 respectively) alternate function select for I2C=0X3
  
  //Step 6
  //Initialize I2C as a Master Master configuration Register
  I2C1->MCR = (1<<4); //Master mode is enabled
  //Step 7
  //Set the desired SCL clock speed of 100 Kbps :Master timer period
  //The TPR (for us is 7) is found using the formula on pg. 972
  I2C1->MTPR = (7<<0); 
  
  //1 byte must be sent
  setSlaveAddress(0x20); //last 7 bits (7-1)
  setRW(0); //first bit (0)
  
  //writeByte(BYTE ONE, BYTE TWO)
  
  //send IODIR register address
  writeByte(0x00, (1<<0)|(1<<1)); //BYTE TWO: conditions = RUN and START
  //send IODIR value to make GPIO pins outputs
  writeByte(0x00, (1<<0)|(1<<2)); //BTYE TWO: conditions = RUN, STOP
  
  while(1)
  {
    writeByte(0x09, (1<<0)|(1<<1));
    writeByte(leds++, (1<<0)|(1<<2));
    
    for(int i = 0; i < 100000; i++){};

  }

  
  return 0;
}