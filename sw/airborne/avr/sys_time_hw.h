/*
 * Paparazzi $Id$
 *
 * Copyright (C) 2005 Pascal Brisset, Antoine Drouin
 *
 * This file is part of paparazzi.
 *
 * paparazzi is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * paparazzi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with paparazzi; see the file COPYING.  If not, write to
 * the Free Software Foundation, 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 */

/*
 *\brief AVR timer functions 
 *
 */

#ifndef SYS_TIME_HW_H
#define SYS_TIME_HW_H

#include "std.h"
#include <avr/io.h>
#define F_CPU (CLOCK*1000000)
#include <avr/delay.h>

/*
 * Enable Timer1 (16-bit) running at Clk/1 for the global system
 * clock.
 *
 * Low frequency periodic tasks will be signaled by timer 2
 * running at Clk/1024.  For 16 Mhz clock, this will be every
 * 16384 microseconds, or 61.03515625 Hz.
 */
static inline void sys_time_init( void ) {

  /* Timer0: Modem clock is started in modem.h in ctc mode*/

  /* Timer1 @ Clk/1: System clock */
  TCCR1A = 0x00;
  TCCR1B = _BV(CS10);

  /* Timer2 @ Clk/1024: Periodic clock */
#if defined (__AVR_ATmega8__)
  TCCR2         = _BV(CS20) | _BV(CS21) | _BV(CS22);
#elif defined (__AVR_ATmega128__)
  TCCR2         = _BV(CS20) | _BV(CS22);
#else
#warning "Unknown arch"
#endif

#ifdef TIMER3
  /* Timer3 @ Clk/1: motor controller */
  TCCR3A = 0x00;
  TCCR3B = 0x01;
#endif

  cpu_time = 0;
}


#define SYS_TICS_OF_USEC(us) (uint16_t)((us)*CLOCK)
#define SIGNED_SYS_TICS_OF_USEC(us) (int16_t)((us)*CLOCK)
#define LONG_SYS_TICS_OF_USEC(us) (uint8_t)(((uint32_t)(us)*CLOCK)/1024ul)

/*
 *  Periodic tasks occur when Timer2 overflows.  Check and unset
 * the overflow bit. Occurs at 61.03515625 Hz
 *
 */
static inline bool_t sys_time_periodic( void ) {
  if( !bit_is_set( TIFR, TOV2 ) )
    return FALSE;
  TIFR = _BV(TOV2);
  return TRUE;
}


#define DelayUS(x) _delay_us(x)


#endif /* SYS_TIME_HW_H */
