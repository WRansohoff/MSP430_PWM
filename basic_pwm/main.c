#include <msp430.h>

#define MAX_TIME  (1000)
int tim = 500;
int tv = -50;

// Short delay method, until I figure out timers.
void delay (unsigned long int d) {
  unsigned long int i;
  for (i = 0; i < d; ++i) { asm("nop"); }
}

int main() {
  // Stop the watchdog timer.
  WDTCTL  =  (WDTPW + WDTHOLD);

  // Setup P1.6 for PWM output.
  P1DIR  |=  (BIT6);
  P1SEL  |=  (BIT6);
  // Setup Timer 0.
  TA0CCR0  = (MAX_TIME);
  TA0CCTL1 = (OUTMOD_7);
  TA0CCR1  = (tim);
  // Start timer 0.
  TA0CTL  |= (TASSEL_2 | MC_1);

  while (1) {
    tim += tv;
    if (tim > MAX_TIME) {
      tim = MAX_TIME;
      tv = -tv;
    }
    else if (tim < 0) {
      tim = 0;
      tv = -tv;
    }
    TA0CCR1 = (tim);
    delay(10000);
  }
  return 0;
}
