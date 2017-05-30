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

#define USE_LEDS

#define PIN_CLOCK            2
#define PIN_DATA             4
#define PIN_NUMLOCK_LED      9
#define PIN_CAPSLOCK_LED     10
#define PIN_SCROLLLOCK_LED   11

#ifdef USE_LEDS
PS2Keyboard g_keyboard(
  
  PIN_CLOCK,
  PIN_DATA,
  PIN_NUMLOCK_LED,
  PIN_CAPSLOCK_LED,
  PIN_SCROLLLOCK_LED
);
#else
PS2Keyboard g_keyboard(
  
  PIN_CLOCK,
  PIN_DATA
);
#endif



void handleSerialData() {

  static uint8_t buf[4];
  static uint8_t idx = 0;
  
  delayMicroseconds(500);

  while(Serial.available() > 0) {

    if((buf[idx++] = Serial.read()) == '\n') {
      
      bool isPressed = buf[0] == '1';
      uint8_t vKey = buf[1];
      
      if(isPressed)
        g_keyboard.pressKey(vKey);
      else
        g_keyboard.releaseKey(vKey);

      idx = 0;
    }
    idx &= 0x03;
  }
}



void setup() {

    Serial.begin(9600);
}

void loop() {

  if(Serial.available() > 0)
    handleSerialData();

  g_keyboard.update();
}

