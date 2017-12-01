#include    "xc.h"             
#include    "stdint.h"         
#include    "stdbool.h"         
#include	"RAINBO.h"			





unsigned char LPFInput[10] = {128, 128, 128, 128, 128, 128, 128, 128, 128, 128};
unsigned char lastVal = 0;
unsigned char LPFAvg;
unsigned char tempIN;

unsigned char red = 0;
unsigned char green = 64;
unsigned char blue = 0;

unsigned char tempRed;
unsigned char tempGreen;
unsigned char tempBlue;

const char maxLEDs = 30;

unsigned char temp;
unsigned char i;

unsigned char adConvert(unsigned char chan) {
    ADON = 1;
    ADCON0 = (ADCON0 & 0b10000011);
    ADCON0 = (ADCON0 | chan);
    __delay_us(2);

    GO = 1;
    while (GO);

    ADON = 0;
    return (ADRESH);
}

void lights(unsigned char height, unsigned char leds) {
    if (height > 127) {
        if (height < 249) {
            height = (height / 4) - 32;
        } else {
            height = 30;
        }
    } else {
        height = 0;
    }
    //if(height==0)return;
    for (leds; leds != 0; leds--) {
        temp = (leds < height) ? green : 0;
        for (i = 8; i != 0; i--) {
            asm("bsf LATA,4");
            asm("nop");
            asm("btfss _temp,7");
            asm("bcf LATA,4");
            asm("nop");
            asm("lslf _temp,f");
            asm("bcf LATA,4");
        }
        temp = red; //(leds < height) ? red : 0;
        for (i = 8; i != 0; i--) {
            asm("bsf LATA,4");
            asm("nop");
            asm("btfss _temp,7");
            asm("bcf LATA,4");
            asm("nop");
            asm("lslf _temp,f");
            asm("bcf LATA,4");
        }
        temp = blue; //(leds < height) ? blue : 0;
        for (i = 8; i != 0; i--) {
            asm("bsf LATA,4");
            asm("nop");
            asm("btfss _temp,7");
            asm("bcf LATA,4");
            asm("nop");
            asm("lslf _temp,f");
            asm("bcf LATA,4");
        }
    }
}

int main(void) {
    init();
    __delay_us(200);


    while (1) {
        tempIN = adConvert(LPF);
        if (tempIN > 127) {
            LPFInput[lastVal] = tempIN;
            lastVal = (lastVal == 9) ? 0 : lastVal + 1;
        }
        for (unsigned char v = 0; v != sizeof (LPFInput); v++) {
            LPFAvg += LPFInput[v];
        }
        lights(LPFAvg, maxLEDs);
        //pwmLED(LPFInput);
        //neoRGB(red, green, blue, maxLEDs);
        __delay_ms(50);
    }
}