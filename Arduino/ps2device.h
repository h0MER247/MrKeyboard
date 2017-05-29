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
#ifndef __PS2_DEVICE_H__
#define __PS2_DEVICE_H__

#include "Arduino.h"

enum PS2Result {

  RES_OK,
  RES_NOT_READY,
  RES_TIMED_OUT,
  RES_PARITY_ERROR,
};

/**
 * @see http://www.computer-engineering.org/ps2protocol/
 */
class PS2Device {

  public:
		PS2Device(uint8_t pinClock, uint8_t pinData);
		
		PS2Result writePS2(uint8_t data);
		PS2Result readPS2(uint8_t *data);

  private:
    inline void waitFullClockCycle() { delayMicroseconds(40); }
    inline void waitHalfClockCycle() { delayMicroseconds(20); }
    
    inline void setPinHi(uint8_t pin) { pinMode(pin, INPUT); digitalWrite(pin, HIGH); }
    inline void setPinLo(uint8_t pin) { pinMode(pin, OUTPUT); digitalWrite(pin, LOW); }
    
    inline void setDataLine(uint8_t level) { if(level == LOW) { setPinLo(m_pinData); } else { setPinHi(m_pinData); } }
    inline void setClockLine(uint8_t level) { if(level == LOW) { setPinLo(m_pinClock); } else {setPinHi(m_pinClock); } }
    inline uint8_t getClockLine() { return digitalRead(m_pinClock); }
    inline uint8_t getDataLine() { return digitalRead(m_pinData); }
   
	private:
		uint8_t m_pinClock;
		uint8_t m_pinData;
};

#endif /* __PS2_DEVICE_H__ */


