#include <msp430.h>

#define MAX_TIME  (1000)
#define MAX_COUNT (40)
int rv = 500;
int gv = 0;
int bv = 500;
int drv = -50;
int dbv = 50;
int dgv = 50;
int c = 0;

inline void update_colors(void) {
  rv += drv;
  bv += dbv;
  gv += dgv;
  if (rv > MAX_TIME) {
    rv = MAX_TIME;
    drv = -drv;
  }
  else if (rv < 0) {
    rv = 0;
    drv = -drv;
  }
  if (bv > MAX_TIME) {
    bv = MAX_TIME;
    dbv = -dbv;
  }
  else if (bv < 0) {
    bv = 0;
    dbv = -dbv;
  }
  if (gv > MAX_TIME) {
    gv = MAX_TIME;
    dgv = -dgv;
  }
  else if (gv < 0) {
    gv = 0;
    dgv = -dgv;
  }
  TA1CCR2 = (rv);
  TA1CCR1 = (bv);
  TA0CCR1 = (gv);
}

int main() {
  // Stop the watchdog timer.
  WDTCTL  =  (WDTPW + WDTHOLD);

  // RGB LED: P2.3 -> CCI0B, P2.1 -> CCI1A, P2.5 -> CCI2B
  P2DIR  |=  (BIT1 | BIT5);
  P2SEL  |=  (BIT1 | BIT5);
  //TA1CCTL0 = (OUTMOD_7);
  TA1CCR0  = (MAX_TIME);
  TA1CCTL1 = (OUTMOD_7);
  TA1CCTL2 = (OUTMOD_7);
  TA1CCR1  = (rv);
  TA1CCR2  = (bv);
  // Start timer 1.
  TA1CTL  =  (TASSEL_2 | MC_1);

  // Use timer 0 for the green LED...
  P1DIR  |=  (BIT6);
  P1SEL  |=  (BIT6);
  TA0CCR0  = (MAX_TIME);
  TA0CCTL1 = (OUTMOD_7);
  TA0CTL  |= (TAIE);
  TA0CCR1  = (gv);
  // Start timer 0.
  TA0CTL  |= (TASSEL_2 | MC_1);

  // Turn off the CPU, and enable interrupts.
  _BIS_SR((LPM0_bits | GIE));

  // (N/A)
  while (1) {}
  return 0;
}

__attribute__((interrupt(TIMER0_A1_VECTOR)))
void tim0_a1_isr(void) {
  // Update the colors every N cycles.
  ++c;
  if (c >= MAX_COUNT) {
    update_colors();
    c = 0;
  }
  // Acknowledge interrupt.
  TA0CTL &= ~(TAIFG);
}
