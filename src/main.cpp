#include "stm32f103c8t6.h"
#include "mbed.h"
#include "USBKeyboard.h"

DigitalOut  myled(PA_9);

int main(void) {
    confSysClock();

    USBKeyboard keyboard;

    while (1) {
        myled = !myled;
        keyboard.keyCode(KEY_CAPS_LOCK);
        wait_ms(100);
    }
}
