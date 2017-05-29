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
#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

#include "Arduino.h"
#include "PS2Device.h"

#define KEY_BUFFER_SIZE   8

/**
 * @see http://www.computer-engineering.org/ps2keyboard/
 */
class PS2Keyboard : public PS2Device {
  
  public:
	  PS2Keyboard(uint8_t pinClock,
	              uint8_t pinData,
	              uint8_t pinNumLockLED,
	              uint8_t pinCapsLockLED,
	              uint8_t pinScrollLockLED);
    PS2Keyboard(uint8_t pinClock,
                uint8_t pinData);
    ~PS2Keyboard();
                
		void pressKey(uint8_t vKey);
    void releaseKey(uint8_t vKey);
    void update();
    
  private:
    void onReset();
    void onSetTypematicRateAndDelay(uint8_t data);
    void onSetLEDState(uint8_t data);
    
    bool isKeyMapped(uint8_t vKey);
    void sendScancodeForKey(bool isMakeCode, uint8_t vKey);
    
	private:
    uint8_t m_pinNumLockLED;
    uint8_t m_pinCapsLockLED;
    uint8_t m_pinScrollLockLED;
    bool m_useLEDs;

    int32_t m_typematicRate;
    int32_t m_typematicDelay;
    int8_t m_typematicBufferIndex;
    
    bool m_scanningEnabled;
    
    struct KeyBufferEntry {

      uint8_t state;
      uint8_t vKey;
      uint32_t timeNextEvent;
      
    } m_buffer[KEY_BUFFER_SIZE];
};

#endif /* __KEYBOARD_H__ */

