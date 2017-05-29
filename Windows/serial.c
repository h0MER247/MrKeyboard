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



HANDLE openCOM(const char *port, DWORD baud, BYTE dataBits, BYTE stopBits, BYTE parity) {
    
    HANDLE res = CreateFile(
            
        port,
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );
    
    if(FAILED(res))
        goto onErr;
    
    DCB params = { 0 };
    params.DCBlength = sizeof(DCB);
    if(!GetCommState(res, &params))
        goto onErr;
    
    params.BaudRate = baud;
    params.ByteSize = dataBits;
    params.StopBits = stopBits;
    params.Parity = parity;
    if(!SetCommState(res, &params))
        goto onErr;
    
    COMMTIMEOUTS t = { 0 };
    t.ReadIntervalTimeout = 100;
    t.ReadTotalTimeoutConstant = 100;
    t.ReadTotalTimeoutMultiplier = 5;
    t.WriteTotalTimeoutConstant = 100;
    t.WriteTotalTimeoutMultiplier = 5;
    if(!SetCommTimeouts(res, &t))
        goto onErr;
    
    return res;
    
onErr:
    closeCOM(res);
    return res;
}



void closeCOM(HANDLE h) {
    
    if(!FAILED(h))
        CloseHandle(h);
}



int writeCOM(HANDLE h, const char *data, DWORD numBytes) {
    
    DWORD bytesWritten = 0;
    
    if(!WriteFile(h, data, numBytes, &bytesWritten, NULL))
        return 0;
    
    return bytesWritten;
}
