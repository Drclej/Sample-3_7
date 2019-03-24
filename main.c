#include <msp430f249.h>
#include "tube.h"
#define uchar unsigned char
#define uint unsigned int


uchar key = 0x00;

#pragma vector = PORT2_VECTOR
__interrupt void Port_2(void)
{
  P2IFG &=~ BIT2;
  key = 0x01;
}

void delayus(uint t)
{
  while(t--);
}

void initialize()
{
  WDTCTL = WDTPW + WDTHOLD;
  
  P1DIR = 0xFF;
  P1SEL = 0x00;
  P1OUT = 0x00;

  P2DIR = BIT0 + BIT1;
  P2OUT = BIT0 + BIT1;
  P2IE |= BIT2;
  P2IES |= BIT2;
  P2IFG &=~ BIT2;
  _EINT();
}

void main()
{
  uchar count = 30;
  uint i;
  WDTCTL = WDTPW + WDTHOLD;
  initialize();
  while (1)
  {
    if (key == 0x01)
    {
      _DINT();
      key = 0;
      for(count = 99;count > 0;--count)
      {
        for(i = 0;i < 600;++i)
        {
          P1OUT = 0;
          tube1(count);
          P2OUT &=~ BIT1;
          delayus(3);
          P2OUT |= BIT1;
           
          P1OUT = 0;
          tube2(count);
          P2OUT &=~ BIT0;
          delayus(3);
          P2OUT |= BIT0;
        }
      }
      P2OUT = 0x01;
      P1OUT = 0x3F;
      _EINT();
    }
  }
  
}