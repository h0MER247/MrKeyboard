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
#include <windows.h>
#include <stdio.h>

#define VK_NUMPAD_RETURN    0x00
#define PRINT_TABLE(X)      printTable(#X, X, sizeof(X) / sizeof(unsigned char));

typedef enum {
    
    F0, E0, SPECIAL
            
} ScancodeType;

unsigned char g_scancode[256] = { 0 };
unsigned char g_scancodeIsF0[256 / 8] = { 0 };
unsigned char g_scancodeIsMapped[256 / 8] = { 0 };



void mapKey(ScancodeType scancodeType, unsigned char vKey, unsigned char scancode) {
    
    int idx = vKey >> 3;
    int mask = 0x80 >> (vKey % 8);
    
    g_scancodeIsMapped[idx] |= mask;
    
    if(scancodeType != SPECIAL) {
        
        g_scancode[vKey] = scancode;
        
        if(scancodeType == F0)
            g_scancodeIsF0[idx] |= mask;
        else
            g_scancodeIsF0[idx] &= ~mask;
    }
}

void printTable(const char *name, const unsigned char *data, int len) {
    
    printf("static PROGMEM const uint8_t %s[] = {\n", name);
    for(int i = 0; i < len; i++) {
        
        if((i % 8) == 0)
            printf("\n  ");
        
        printf("0x%02x", data[i]);
        
        if(i < len - 1)
            printf(",");
        
        if((i % 8) < 7)
            printf(" ");
    }
    printf("\n};\n");
}



void generateTables() {
    
    // First row
    mapKey(F0, VK_ESCAPE, 0x76);
    mapKey(F0, VK_F1, 0x05);
    mapKey(F0, VK_F2, 0x06);
    mapKey(F0, VK_F3, 0x04);
    mapKey(F0, VK_F4, 0x0c);
    mapKey(F0, VK_F5, 0x03);
    mapKey(F0, VK_F6, 0x0b);
    mapKey(F0, VK_F7, 0x83);
    mapKey(F0, VK_F8, 0x0a);
    mapKey(F0, VK_F9, 0x01);
    mapKey(F0, VK_F10, 0x09);
    mapKey(F0, VK_F11, 0x78);
    mapKey(F0, VK_F12, 0x07);
    mapKey(SPECIAL, VK_SNAPSHOT, 0xff);
    mapKey(F0, VK_SCROLL, 0x7e);
    mapKey(SPECIAL, VK_PAUSE, 0xff);
    
    // Second row
    mapKey(F0, VK_OEM_5, 0x0e);
    mapKey(F0, 0x31, 0x16);
    mapKey(F0, 0x32, 0x1e);
    mapKey(F0, 0x33, 0x26);
    mapKey(F0, 0x34, 0x25);
    mapKey(F0, 0x35, 0x2e);
    mapKey(F0, 0x36, 0x36);
    mapKey(F0, 0x37, 0x3d);
    mapKey(F0, 0x38, 0x3e);
    mapKey(F0, 0x39, 0x46);
    mapKey(F0, 0x30, 0x45);
    mapKey(F0, VK_OEM_4, 0x4e);
    mapKey(F0, VK_OEM_6, 0x55);
    mapKey(F0, VK_BACK, 0x66);
    mapKey(E0, VK_INSERT, 0x70);
    mapKey(E0, VK_HOME, 0x6c);
    mapKey(E0, VK_PRIOR, 0x7d);
    mapKey(F0, VK_NUMLOCK, 0x77);
    mapKey(E0, VK_DIVIDE, 0x4a);
    mapKey(F0, VK_MULTIPLY, 0x7c);
    mapKey(F0, VK_SUBTRACT, 0x7b);
    
    // Third row
    mapKey(F0, VK_TAB, 0x0d);
    mapKey(F0, 0x51, 0x15);
    mapKey(F0, 0x57, 0x1d);
    mapKey(F0, 0x45, 0x24);
    mapKey(F0, 0x52, 0x2d);
    mapKey(F0, 0x54, 0x2c);
    mapKey(F0, 0x5a, 0x35);
    mapKey(F0, 0x55, 0x3c);
    mapKey(F0, 0x49, 0x43);
    mapKey(F0, 0x4f, 0x44);
    mapKey(F0, 0x50, 0x4d);
    mapKey(F0, VK_OEM_1, 0x54);
    mapKey(F0, VK_OEM_PLUS, 0x5b);
    mapKey(F0, VK_RETURN, 0x5a);
    mapKey(E0, VK_DELETE, 0x71);
    mapKey(E0, VK_END, 0x69);
    mapKey(E0, VK_NEXT, 0x7a);
    mapKey(F0, VK_NUMPAD7, 0x6c);
    mapKey(F0, VK_NUMPAD8, 0x75);
    mapKey(F0, VK_NUMPAD9, 0x7d);
    mapKey(F0, VK_ADD, 0x79);
    
    // Fourth row
    mapKey(F0, VK_CAPITAL, 0x58);
    mapKey(F0, 0x41, 0x1c);
    mapKey(F0, 0x53, 0x1b);
    mapKey(F0, 0x44, 0x23);
    mapKey(F0, 0x46, 0x2b);
    mapKey(F0, 0x47, 0x34);
    mapKey(F0, 0x48, 0x33);
    mapKey(F0, 0x4a, 0x3b);
    mapKey(F0, 0x4b, 0x42);
    mapKey(F0, 0x4c, 0x4b);
    mapKey(F0, VK_OEM_3, 0x4c);
    mapKey(F0, VK_OEM_7, 0x52);
    mapKey(F0, VK_OEM_2, 0x5d);
    mapKey(F0, VK_NUMPAD4, 0x6b);
    mapKey(F0, VK_NUMPAD5, 0x73);
    mapKey(F0, VK_NUMPAD6, 0x74);

    // Fifth row
    mapKey(F0, VK_LSHIFT, 0x12);
    mapKey(F0, VK_OEM_102, 0x61);
    mapKey(F0, 0x59, 0x1a);
    mapKey(F0, 0x58, 0x22);
    mapKey(F0, 0x43, 0x21);
    mapKey(F0, 0x56, 0x2a);
    mapKey(F0, 0x42, 0x32);
    mapKey(F0, 0x4e, 0x31);
    mapKey(F0, 0x4d, 0x3a);
    mapKey(F0, VK_OEM_COMMA, 0x41);
    mapKey(F0, VK_OEM_PERIOD, 0x49);
    mapKey(F0, VK_OEM_MINUS, 0x4a);
    mapKey(F0, VK_RSHIFT, 0x59);
    mapKey(E0, VK_UP, 0x75);
    mapKey(F0, VK_NUMPAD1, 0x69);
    mapKey(F0, VK_NUMPAD2, 0x72);
    mapKey(F0, VK_NUMPAD3, 0x7a);
    mapKey(E0, VK_NUMPAD_RETURN, 0x5a);
    
    // Sixth row
    mapKey(F0, VK_LCONTROL, 0x14);
    mapKey(E0, VK_LWIN, 0x1f);
    mapKey(F0, VK_LMENU, 0x11);
    mapKey(F0, VK_SPACE, 0x29);
    mapKey(E0, VK_RMENU, 0x11);
    mapKey(E0, VK_RWIN, 0x27);
    mapKey(E0, VK_APPS, 0x2f);
    mapKey(E0, VK_RCONTROL, 0x14);
    mapKey(E0, VK_LEFT, 0x6b);
    mapKey(E0, VK_DOWN, 0x72);
    mapKey(E0, VK_RIGHT, 0x74);
    mapKey(F0, VK_NUMPAD0, 0x70);
    mapKey(F0, VK_DECIMAL, 0x71);
    
    // Print tables
    PRINT_TABLE(g_scancode);
    PRINT_TABLE(g_scancodeIsF0);
    PRINT_TABLE(g_scancodeIsMapped);
}
