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
#include "keyboard.h"

#define VK_PAUSE       0x13
#define VK_SNAPSHOT    0x2c

#define WRITE_PS2(X)   while(writePS2(X) != RES_OK)

/**
 * PS/2 Scan Set 2
 */
static PROGMEM const uint8_t g_scancode[] = {

  0x5a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x66, 0x0d, 0x00, 0x00, 0x00, 0x5a, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x58, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x76, 0x00, 0x00, 0x00, 0x00,
  0x29, 0x7d, 0x7a, 0x69, 0x6c, 0x6b, 0x75, 0x74,
  0x72, 0x00, 0x00, 0x00, 0x00, 0x70, 0x71, 0x00,
  0x45, 0x16, 0x1e, 0x26, 0x25, 0x2e, 0x36, 0x3d,
  0x3e, 0x46, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x1c, 0x32, 0x21, 0x23, 0x24, 0x2b, 0x34,
  0x33, 0x43, 0x3b, 0x42, 0x4b, 0x3a, 0x31, 0x44,
  0x4d, 0x15, 0x2d, 0x1b, 0x2c, 0x3c, 0x2a, 0x1d,
  0x22, 0x1a, 0x35, 0x1f, 0x27, 0x2f, 0x00, 0x00,
  0x70, 0x69, 0x72, 0x7a, 0x6b, 0x73, 0x74, 0x6c,
  0x75, 0x7d, 0x7c, 0x79, 0x00, 0x7b, 0x71, 0x4a,
  0x05, 0x06, 0x04, 0x0c, 0x03, 0x0b, 0x83, 0x0a,
  0x01, 0x09, 0x78, 0x07, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x77, 0x7e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x12, 0x59, 0x14, 0x14, 0x11, 0x11, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x54, 0x5b, 0x41, 0x4a, 0x49, 0x5d,
  0x4c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x4e, 0x0e, 0x55, 0x52, 0x00,
  0x00, 0x00, 0x61, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
static PROGMEM const uint8_t g_scancodeIsF0[] = {

  0x00, 0xc4, 0x08, 0x10, 0x80, 0x00, 0xff, 0xc0,
  0x7f, 0xff, 0xff, 0xe0, 0xff, 0xf6, 0xff, 0xf0,
  0x00, 0x00, 0xc0, 0x00, 0xe8, 0x00, 0x00, 0x3f,
  0x80, 0x00, 0x00, 0x1e, 0x20, 0x00, 0x00, 0x00
};
static PROGMEM const uint8_t g_scancodeIsMapped[] = {

  0x80, 0xc4, 0x18, 0x10, 0xff, 0x8e, 0xff, 0xc0,
  0x7f, 0xff, 0xff, 0xfc, 0xff, 0xf7, 0xff, 0xf0,
  0x00, 0x00, 0xc0, 0x00, 0xfc, 0x00, 0x00, 0x3f,
  0x80, 0x00, 0x00, 0x1e, 0x20, 0x00, 0x00, 0x00
};

/**
 * Typematic rates and delays
 */
static PROGMEM const uint32_t g_typematicRate[] = {
  
   33333,  37453,  41667,  45872,
   48309,  54054,  58480,  62500,
   66667,  75188,  83333,  91743,
  100000, 108696, 116279, 125000,
  133333, 149254, 166667, 181818,
  200000, 217391, 232558, 250000,
  270270, 303030, 333333, 370370,
  400000, 434783, 476190, 500000
};
static PROGMEM const uint32_t g_typematicDelay[] = {
  
  250000, 500000, 750000, 1000000
};

enum KeyState {

  STATE_FREE,
  STATE_HOLD,
  STATE_PRESSED,
  STATE_RELEASED
};

enum KeyLED {

  LED_NONE = 0x00,
  LED_SCROLLLOCK = 0x01,
  LED_NUMLOCK = 0x02,
  LED_CAPSLOCK = 0x04,
  LED_ALL = LED_SCROLLLOCK | LED_NUMLOCK | LED_CAPSLOCK
};



PS2Keyboard::PS2Keyboard(uint8_t pinClock,
                         uint8_t pinData,
                         uint8_t pinNumLockLED,
                         uint8_t pinCapsLockLED,
                         uint8_t pinScrollLockLED) : PS2Keyboard(pinClock, pinData) {

  m_pinNumLockLED = pinNumLockLED;
  m_pinCapsLockLED = pinCapsLockLED;
  m_pinScrollLockLED = pinScrollLockLED;
  m_useLEDs = true;

  pinMode(pinNumLockLED, OUTPUT);
  pinMode(pinCapsLockLED, OUTPUT);
  pinMode(pinScrollLockLED, OUTPUT);
}

PS2Keyboard::PS2Keyboard(uint8_t pinClock,
                         uint8_t pinData) : PS2Device(pinClock, pinData) {

  m_useLEDs = false;
  
  for(uint8_t i = 0; i < KEY_BUFFER_SIZE; i++)
    m_buffer[i].state = STATE_FREE;

  m_typematicBufferIndex = -1;

  onReset();
}

PS2Keyboard::~PS2Keyboard() {
  
}



void PS2Keyboard::pressKey(uint8_t vKey) {
  
  if(!m_scanningEnabled)
    return;

  // Return if this key is not mapped
  if(!isKeyMapped(vKey))
    return;
    
  // Return if this key is already pressed
  for(uint8_t i = 0; i < KEY_BUFFER_SIZE; i++) {

    if(m_buffer[i].vKey == vKey) {
      
      if(m_buffer[i].state == STATE_PRESSED ||
         m_buffer[i].state == STATE_HOLD) {
          
          return;
       }
    }
  }
  
  // Find a free spot in the buffer
  for(uint8_t i = 0; i < KEY_BUFFER_SIZE; i++) {

    if(m_buffer[i].state == STATE_FREE) {

      m_buffer[i].state = STATE_PRESSED;
      m_buffer[i].vKey = vKey;
      m_buffer[i].timeNextEvent = micros();
      return;
    }
  }
}

void PS2Keyboard::releaseKey(uint8_t vKey) {
  
  if(!m_scanningEnabled)
    return;
    
  // Release the key
  for(uint8_t i = 0; i < KEY_BUFFER_SIZE; i++) {

    if(m_buffer[i].state != STATE_FREE &&
       m_buffer[i].state != STATE_RELEASED &&
       m_buffer[i].vKey == vKey) {

      m_buffer[i].state = STATE_RELEASED;
      m_buffer[i].timeNextEvent = micros();
      return;
    }
  }
}

void PS2Keyboard::update() {  
  
  static uint8_t lastCmd = 0x00;

  //
  // Update communication with host (no parity checks or other goodies for now)
  //
  uint8_t cmd;
  
  if(readPS2(&cmd) == RES_OK) {
  
    delayMicroseconds(100);
    
    // Process sub-command
    if(lastCmd != 0x00) {
  
      switch(lastCmd) {
  
        case 0xed: // Set LEDs
          onSetLEDState(cmd);
          WRITE_PS2(0xfa);
          break;
          
        case 0xf0: // Get/set current scancode
          switch(cmd) {
  
            case 0: // Get current scancode (this is always 2)
              WRITE_PS2(0xfa);
              WRITE_PS2(0x02);
              break;

            case 1:
            case 2:
            case 3: // Set current scancode to 1,2 or 3 (only 2 is supported...)
              WRITE_PS2(0xfa);
              break;

            default: // Unknown
              WRITE_PS2(0xfe);
              break;
          }
          break;

        case 0xf3: // Typematic rate / delay
          onSetTypematicRateAndDelay(cmd);
          WRITE_PS2(0xfa);
          break;
      }
      lastCmd = 0x00;
    }
    
    // Process command
    else {
      
      switch(cmd) {
          
        case 0xed: // Set LEDs
          lastCmd = 0xed;
          WRITE_PS2(0xfa);
          break;

        case 0xee: // Echo
          WRITE_PS2(0xee);
          break;
            
        case 0xf0: // Get/set current scancode
          lastCmd = 0xf0;
          WRITE_PS2(0xfa);
          break;

        case 0xf2: // Identify
          WRITE_PS2(0xfa);
          WRITE_PS2(0xab);
          WRITE_PS2(0x83);
          break;

        case 0xf3: // Typematic rate/delay
          lastCmd = 0xf3;
          WRITE_PS2(0xfa);
          break;
  
        case 0xf4: // Enable scanning
          m_scanningEnabled = true;
          WRITE_PS2(0xfa);
          break;

       case 0xf5: // Disable scanning
          m_scanningEnabled = false;
          WRITE_PS2(0xfa);
          break;

        case 0xf6: // Set defaults
          onReset();
          WRITE_PS2(0xfa);
          break;
          
        case 0xff: // Self test
          onReset();
          WRITE_PS2(0xfa);
          WRITE_PS2(0xaa);
          break;
        
        default: // Unknown
          WRITE_PS2(0xfe);
          return;
      }
    }
  }

  //
  // Update events
  //
  if(m_scanningEnabled && lastCmd == 0x00) {

    uint32_t now = micros();
    
    for(uint8_t i = 0; i < KEY_BUFFER_SIZE; i++) {

      KeyBufferEntry &keyEntry = m_buffer[i];
      
      if(keyEntry.state != STATE_FREE &&
         keyEntry.timeNextEvent <= now) {

        switch(keyEntry.state) {

          case STATE_HOLD:
            if(i == m_typematicBufferIndex) {

              sendScancodeForKey(true, keyEntry.vKey);
              keyEntry.timeNextEvent = now + m_typematicRate;
            }
            break;

          case STATE_PRESSED:
            sendScancodeForKey(true, keyEntry.vKey);
            keyEntry.timeNextEvent = now + m_typematicDelay;
            
            if(keyEntry.vKey == VK_PAUSE) {
             
              keyEntry.state = STATE_RELEASED;
            }
            else {

              keyEntry.state = STATE_HOLD;
              m_typematicBufferIndex = i;
            }
            break;

          case STATE_RELEASED:
            sendScancodeForKey(false, keyEntry.vKey);
            keyEntry.state = STATE_FREE;
            
            if(i == m_typematicBufferIndex)
              m_typematicBufferIndex = -1;
          
            break;
        }
      }
    }
  }
}



void PS2Keyboard::onReset() {
 
  // 10.9 characters per second, 500ms delay
  onSetTypematicRateAndDelay(0x2b);

  // LED test
  if(m_useLEDs) {
    
    onSetLEDState(LED_ALL);
    delayMicroseconds(25000);
    onSetLEDState(LED_NONE);
  }

  // Enable scanning
  m_scanningEnabled = true;
}

void PS2Keyboard::onSetTypematicRateAndDelay(uint8_t data) {

  uint8_t idxRate = data & 0x1f;
  uint8_t idxDelay = (data >> 5) & 0x03;
  
  m_typematicRate = pgm_read_dword(&g_typematicRate[idxRate]);
  m_typematicDelay = pgm_read_dword(&g_typematicDelay[idxDelay]);
}

void PS2Keyboard::onSetLEDState(uint8_t data) {
  
  if(m_useLEDs) {
  
    uint8_t scrollLock = ((data & 0x01) != 0) ? HIGH : LOW;
    uint8_t numLock = ((data & 0x02) != 0) ? HIGH : LOW;
    uint8_t capsLock = ((data & 0x04) != 0) ? HIGH : LOW;
    
    digitalWrite(m_pinScrollLockLED, scrollLock);
    digitalWrite(m_pinNumLockLED, numLock);
    digitalWrite(m_pinCapsLockLED, capsLock);
  }
}



bool PS2Keyboard::isKeyMapped(uint8_t vKey) {

  uint8_t idx = vKey >> 3;
  uint8_t mask = 0x80 >> (vKey & 0x07);

  return (pgm_read_byte(&g_scancodeIsMapped[idx]) & mask) != 0;
}

void PS2Keyboard::sendScancodeForKey(bool isMakeCode, uint8_t vKey) {

  if(vKey == VK_PAUSE) {

    if(isMakeCode) {
      
      WRITE_PS2(0xe1);
      WRITE_PS2(0x14);
      WRITE_PS2(0x77);
      WRITE_PS2(0xe1);
      WRITE_PS2(0xf0);
      WRITE_PS2(0x14);
      WRITE_PS2(0xf0);
      WRITE_PS2(0x77);
    }
  }
  
  else if(vKey == VK_SNAPSHOT) {

    if(isMakeCode) {
      
      WRITE_PS2(0xe0);
      WRITE_PS2(0x12);
      WRITE_PS2(0xe0);
      WRITE_PS2(0x7c);
    }
    else {

      WRITE_PS2(0xe0);
      WRITE_PS2(0xf0);
      WRITE_PS2(0x7c);
      WRITE_PS2(0xe0);
      WRITE_PS2(0xf0);
      WRITE_PS2(0x12);
    }
  }

  else {

    uint8_t idx = vKey >> 3;
    uint8_t mask = 0x80 >> (vKey & 0x07);

    bool isF0Code = (pgm_read_byte(&g_scancodeIsF0[idx]) & mask) != 0;
    uint8_t scancode = pgm_read_byte(&g_scancode[vKey]);

    if(!isF0Code)
      WRITE_PS2(0xe0);
    if(!isMakeCode)
      WRITE_PS2(0xf0);
      
    WRITE_PS2(scancode);
  }
}

