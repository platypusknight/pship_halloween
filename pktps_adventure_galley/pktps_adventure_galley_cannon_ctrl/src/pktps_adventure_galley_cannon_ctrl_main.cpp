//================================================================================
//  Platypus Knight Technologies 
//
//                _.- ~~^^^'~- _ __ .,.- ~ ~ ~  ~  -. _
//     ________,'       ::.                       _,-  ~ -.
//    ((      ~_\   -s-  ::                     ,'          ;,
//     \\       <.._ .;;;`                     ;           }  `',
//      ``======='    _ _- _ (   }             `,          ,'\,  `,
//                   ((/ _ _,i   ! _ ~ - -- - _ _'_-_,_,,,'    \,  ;
//                      ((((____/            (,(,(, ____>        \,'
// 
// 
//====================================================================================
// Copyright (c) 2026 by Luke Curtis
//
// Pirate Ship Halloween is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option) any
// later version.
//
// Pirate Ship Halloween is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public License. If not,
// see <https://www.gnu.org/licenses/>.
//====================================================================================
// Code Owner    : Luke Curtis
// Project       : PirateShipHalloween
//====================================================================================

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <util/atomic.h>
#include <avr/interrupt.h>
#include "pktcmn_uart.h"


// Relay output -- set to whichever pin your relay is actually on.
// (DDRB = 0x2A makes PB1, PB3 and PB5 outputs.)
#define RELAY_PORT      PORTB
#define RELAY_BIT       PB1
#define RELAY_ON()      (RELAY_PORT |=  (1 << RELAY_BIT))   // swap these two if your
#define RELAY_OFF()     (RELAY_PORT &= ~(1 << RELAY_BIT))   // relay board is active-low

#define TIMER_TICK_HZ       200     // 5 ms period
#define RELAY_HOLD_PERIODS  50      // 50 x 5 ms = 250 ms

volatile bool launch_pending = false;
volatile uint16_t launch_hold_cnt = 0;

ISR(INT0_vect)
{
  if (launch_hold_cnt == 0)
    launch_pending = true;
}

ISR(TIMER1_COMPA_vect)
{
  if (launch_hold_cnt > 0)
  {
    if (--launch_hold_cnt == 0)
      RELAY_OFF();
  }
}

int main()
{  
  DDRB = 0x2A;
  DDRD = 0x00;
  PORTD |=  (1<<PD2);

  TCCR1A = 0;                                 // CTC mode (WGM13:0 = 0100)
  TCCR1B = (1 << WGM12) | (1 << CS11) | (1 << CS10);  // CTC, prescaler = 64
  OCR1A  = (F_CPU / 64 / TIMER_TICK_HZ) - 1;  // 16e6/64/200 - 1 = 1249
  TCNT1  = 0;
  TIMSK1 |= (1 << OCIE1A);                    // enable compare-match A interrupt
  
  EICRA &= ~((1<<ISC01)|(1<<ISC00));
  EICRA |=  (1<<ISC01);       // ISC01:ISC00 = 10 -> falling edge triggers INT0
  EIMSK |=  (1<<INT0);        // enable INT0

  // UART
  // 103 ==> 9600 baud
  UartIf uart_if(103);

  // enable interrupts
  sei();

  uart_if.putstring("!!!!!!Config Complete!!!!!!\n\r");
  _delay_ms(500);

  while(1)
  {
    if (launch_pending)
    {
      uart_if.putstring("Start of launch\n\r");
      
      // 16-bit variable on an 8-bit CPU: update it with interrupts blocked
      ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
      {
        launch_pending  = false;
        launch_hold_cnt = RELAY_HOLD_PERIODS;
        RELAY_ON();
      }
    }
  }

  return 0;
}
