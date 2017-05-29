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
#include "serial.h"
#include <windows.h>
#include <stdio.h>

// Used to generate the necessary scancode tables for the arduino project
//#define GENERATE_TABLES

#define VK_NUMPAD_RETURN 0x00

#define COM_PORT         "COM4"
#define COM_BAUD_RATE    CBR_9600
#define COM_DATA_BITS    8
#define COM_STOP_BITS    ONESTOPBIT
#define COM_PARITY       NOPARITY

HHOOK m_keyHook;
HANDLE m_comPort;
int m_running;



void sendKey(int isPressed, char vKey) {
    
    char buffer[3] = {
        
        isPressed ? '1' : '0',
        vKey,
        '\n'
    };
    
    writeCOM(m_comPort, buffer, 3);
}

LRESULT CALLBACK keyHookFunc(int nCode, WPARAM wParam, LPARAM lParam) {
    
    static char vkStateOld[256] = { 0 };
    static char vkStateNew[256] = { 0 };
    
    if(nCode == HC_ACTION) {
    
        PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)lParam;
        
        switch(wParam) {
            
            case WM_KEYDOWN:
            case WM_SYSKEYDOWN:
            case WM_SYSKEYUP:
            case WM_KEYUP:
                if(p->vkCode == VK_RMENU)
                    return 1;
                
                // Fix AltGR
                if(p->vkCode == VK_LCONTROL && p->scanCode == 0x21d)
                    p->vkCode = VK_RMENU;
                
                // Fix Enter on Num-Block
                if(p->vkCode == VK_RETURN && ((p->flags & 0x01) == 0x01))
                   p->vkCode = VK_NUMPAD_RETURN;
                
                // Check if a new button was pressed, and send it out if that's the case
                vkStateOld[p->vkCode] = vkStateNew[p->vkCode];
                vkStateNew[p->vkCode] = wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN;
                
                if((vkStateOld[p->vkCode] ^ vkStateNew[p->vkCode]) != 0)
                    sendKey(vkStateNew[p->vkCode], p->vkCode & 0xff);
                
                // Release all buttons on exit
                if(vkStateNew[VK_LCONTROL] != 0 && vkStateNew[VK_ESCAPE] != 0) {
                    
                    for(int i = 0; i < 256; i++)
                        if(vkStateOld[i] != 0) sendKey(0, i);
                    
                    m_running = 0;
                }
                
                return 1;
         }
     }
    
     return CallNextHookEx(m_keyHook, nCode, wParam, lParam);
}



#ifdef GENERATE_TABLES

extern void generateTables();

int main(int argc, char **argv) {
    
    generateTables();
    
    return EXIT_SUCCESS;
}

#else

int main(int argc, char **argv) {
    
    printf("Mr. Keyboard - Exit with LCTRL + ESC\n");
    
    if(FAILED(m_comPort = openCOM(COM_PORT, COM_BAUD_RATE, COM_DATA_BITS, COM_STOP_BITS, COM_PARITY))) {
        
        printf("Failed to open serial port \"%s\"\n", COM_PORT);
        return EXIT_FAILURE;
    }
    if(FAILED(m_keyHook = SetWindowsHookEx(WH_KEYBOARD_LL, keyHookFunc, GetModuleHandle(NULL), 0))) {
        
        printf("Failed to install keyboard hook\n");
        return EXIT_FAILURE;
    }
    
    m_running = 1;
    while(m_running) {
        
        MSG msg;
        while(PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {}

        Sleep(10);
    }
    
    UnhookWindowsHookEx(m_keyHook);
    closeCOM(m_comPort);
    
    printf("Bye.\n");
    
    return EXIT_SUCCESS;
}

#endif
