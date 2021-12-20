/* Tone.cpp

  A Tone Generator Library

  Written by Brett Hagman

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

Version Modified By Date     Comments
------- ----------- -------- --------
0001    B Hagman    09/08/02 Initial coding
0002    B Hagman    09/08/18 Multiple pins
0003    B Hagman    09/08/18 Moved initialization from constructor to begin()
0004    B Hagman    09/09/26 Fixed problems with ATmega8
0005    B Hagman    09/11/23 Scanned prescalars for best fit on 8 bit timers
                    09/11/25 Changed pin toggle method to XOR
                    09/11/25 Fixed timer0 from being excluded
0006    D Mellis    09/12/29 Replaced objects with functions
0007    B Cook      10/05/03 Rewritten to only work with Timer1 and support direct hardware output
0008    B Cook      10/05/03 Rewritten so the timer can be selected at compile time
0009	T Carpenter 12/08/06 Rewritten to remove requirement for all the wierd timer name creation macros.

*************************************************/

#include <avr/interrupt.h>
#include "Arduino.h"
#include "wiring_private.h"
#include "pins_arduino.h"

// timerx_toggle_count:
//  > 0 - duration specified
//  = 0 - stopped
//  < 0 - infinitely (until stop() method called, or new play() called)

static volatile long tone_timer_toggle_count;
static volatile uint8_t *tone_timer_pin_register;
static volatile uint8_t tone_timer_pin_mask;

static uint8_t tone_pin = 255;


void tone( uint8_t _pin, unsigned int frequency, unsigned long duration )
{
}


void noTone( uint8_t _pin )
{
}
