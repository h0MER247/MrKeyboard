/*
 * Copyright (C) 2017 h0MER247
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */
#include "ps2device.h"



PS2Device::PS2Device(uint8_t pinClock, uint8_t pinData) {

  m_pinClock = pinClock;
  m_pinData = pinData;
  
  setDataLine(HIGH);
  setClockLine(HIGH);
}



PS2Result PS2Device::readPS2(uint8_t *data) {
  
  uint8_t tmp = 0x00;
  uint8_t parity = 0x00;
  uint8_t t;
  
  // Wait for "request to send" from host
  t = 100;
  while((getDataLine() == HIGH) && --t > 0)
    delayMicroseconds(10);
  if(!t) return RES_TIMED_OUT;
  
  t = 100;
  while((getClockLine() == LOW) && --t > 0)
    delayMicroseconds(10);
  if(!t) return RES_TIMED_OUT;
  
  // Deactivate interrupts
  cli();

  waitHalfClockCycle();
  
  // Start bit
  setClockLine(LOW);
  waitFullClockCycle();
  setClockLine(HIGH);

  // Read data
  for(uint8_t i = 0; i < 8; i++) {

    waitHalfClockCycle();
    if(getDataLine() == HIGH) {

      tmp |= 1 << i; parity ^= 0x01;
    }
    waitHalfClockCycle();
    
    setClockLine(LOW);
    waitFullClockCycle();
    setClockLine(HIGH);
  }

  // Read parity
  waitHalfClockCycle();
  if(getDataLine() == HIGH)
    parity ^= 0x01;
  waitHalfClockCycle();
 
  // Stop bit
  setClockLine(LOW);
  waitFullClockCycle();
  setClockLine(HIGH);
  
  // Acknowledge
  waitHalfClockCycle();
  setDataLine(LOW);
  waitHalfClockCycle();
  setClockLine(LOW);
  
  waitFullClockCycle();
  
  setClockLine(HIGH);
  waitHalfClockCycle();
  setDataLine(HIGH);
  
  waitHalfClockCycle();

  // Activate interrupts
  sei();
  
  if(!parity)
    return RES_PARITY_ERROR;
    
  *data = tmp;
  
  return RES_OK;
}

PS2Result PS2Device::writePS2(uint8_t data) {

  // The bus is idle if clock and data is high
  if(getClockLine() == LOW) return RES_NOT_READY;
  if(getDataLine() == LOW) return RES_NOT_READY;
  
  // Deactivate interrupts
  cli();
  
  // Initiate transfer
  setDataLine(LOW);
  waitHalfClockCycle();
  
  // Start bit
  setClockLine(LOW);
  waitFullClockCycle();
  setClockLine(HIGH);
  
  // Write data
  uint8_t parity = 0x01;
  
  for(uint8_t i = 0; i < 8; i++) {

    waitHalfClockCycle();
    setDataLine((data & 0x01) != 0 ? HIGH : LOW);
    waitHalfClockCycle();
    
    setClockLine(LOW);
    waitFullClockCycle();
    setClockLine(HIGH);
    
    parity ^= data & 0x01; data >>= 1;
  }

  // Parity bit
  waitHalfClockCycle();
  setDataLine(parity != 0 ? HIGH : LOW);
  waitHalfClockCycle();
  
  setClockLine(LOW);
  waitFullClockCycle();
  setClockLine(HIGH);

  // Stop bit
  waitHalfClockCycle();
  setDataLine(HIGH);
  waitHalfClockCycle();
  
  setClockLine(LOW);
  waitFullClockCycle();
  setClockLine(HIGH);
  
  waitHalfClockCycle();

  // Activate interrupts
  sei();
  
  return RES_OK;
}

