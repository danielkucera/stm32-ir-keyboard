#include "stm32f103c8t6.h"
#include "mbed.h"
#include "USBKeyboard.h"
#include "USBSerial.h"
#include "ReceiverIR.h"

#define RST PB_6
#define SWDIO PB_14
#define SWCLK PB_13
#define SWIM PB_11
#define LED PA_9

DigitalOut  myled(PA_9);
USBKeyboard* keyboard;

/**
 * Receive.
 *
 * @param format Pointer to a format.
 * @param buf Pointer to a buffer.
 * @param bufsiz Size of the buffer.
 *
 * @return Bit length of the received data.
 */
int receive(ReceiverIR* ir_rx, RemoteIR::Format *format, uint8_t *buf, int bufsiz, int timeout = 100) {
    int cnt = 0;
    while (ir_rx->getState() != ReceiverIR::Received) {
        cnt++;
        if (timeout < cnt) {
            return -1;
        }
    }
    return ir_rx->getData(format, buf, bufsiz * 8);
}


int main(void) {
    confSysClock();

    ReceiverIR ir_rx(SWCLK);
    USBKeyboard keyboard;
    //USBSerial serial;

    uint8_t buf[32];
    int bitlength;
    int key;
    RemoteIR::Format format;

    memset(buf, 0x00, sizeof(buf));

    while (1) {
        bitlength = receive(&ir_rx, &format, buf, sizeof(buf));

        if (bitlength < 4) {
          myled = 0;
          continue;
        }

        myled = 1;

        key = buf[0]*256 + buf[2];
        switch (key){
          case 0x151:
            keyboard.keyCode(LEFT_ARROW);
            break;
          case 0x150:
            keyboard.keyCode(RIGHT_ARROW);
            break;
          case 0x110:
            keyboard.mediaControl(KEY_VOLUME_DOWN);
            break;
          case 0x118:
            keyboard.mediaControl(KEY_VOLUME_UP);
            break;
          case 0x100:
            keyboard.printf("Hello World! ");
            break;
        }
        //serial.writeBlock(buf, bitlength);
    }
}
