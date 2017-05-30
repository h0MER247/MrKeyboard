# MrKeyboard
A little project which uses an Arduino Uno and a corresponding windows application to emulate a PS/2 keyboard device.

### Sketch
![sketch](https://cloud.githubusercontent.com/assets/5739639/26608793/c9816ccc-459d-11e7-8737-d6958a610bd2.png)

### Why?
While reviving my old 486 PC, I soon realized that I had thrown all my old keyboards away. Several USB to PS/2 adapters later and
discovering that none of them actually worked, I wrote this piece of software. Now my Arduino finally has some real practical use
and maybe this project is useful for you, too.

### Windows application
The Windows application is a keyboard hook, which intercepts all keystrokes and sends their virtual keycode to the Arduino via the
serial interface. Also, the application contains some logic to generate the "[Virtual Keycode] -> [PS/2 Scan Set 2]"-lookup tables
that are required by the Arduino project.

### Arduino
The Arduino receives the virtual keycodes and creates corresponding make and break codes, which are then sent via the PS/2
protocol to my old 486 PC.

Have fun.

[Virtual Keycode]:https://msdn.microsoft.com/de-de/library/windows/desktop/dd375731(v=vs.85).aspx
[PS/2 Scan Set 2]:http://www.computer-engineering.org/ps2keyboard/scancodes2.html
