#include    "xc.h"             
#include    "stdint.h"         
#include    "stdbool.h"         
#include	"RAINBO.h"			

unsigned char LPFInput[10] = {127, 127, 127, 127, 127, 127, 127, 127, 127, 127};
unsigned char lastVal = 0;
unsigned char LPFAvg;
unsigned char tempIN;
unsigned char max1 = 0, max2 = 0, max3 = 0, max4 = 0;

unsigned char red = 0;
unsigned char green = 64;
unsigned char blue = 0;

unsigned char tempRed;
unsigned char tempGreen;
unsigned char tempBlue;

const char maxLEDs = 30;

unsigned char temp;
unsigned char i;

// max LPF is 251
// min LPF is 230?
// there will be 8 groups of 3 LEDs on each side of the gramophone
const unsigned char LPFLookupTable[256] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 31
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //63
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 2, 2, 2, 2, 3, // 95
    3, 3, 4, 4, 4, 4, 5, 5, 5, 6, 6, 6, 6, 7, 7, 7, 8, 8, 8, 8, 9, 9, 9, 10, 10, 10, 10, 11, 11, 11, 12, 12,
    12, 12, 13, 13, 13, 14, 14, 14, 14, 15, 15, 15, 16, 16, 16, 16, 17, 17, 17, 18, 18, 18, 18, 19, 19, 19, 20, 20, 20, 20, 21, 21,
    21, 22, 22, 22, 22, 23, 23, 23, 24, 24, 24, 24, 25, 25, 25, 26, 26, 26, 26, 27, 27, 27, 28, 28, 28, 28, 29, 29, 29, 30, 30, 30, //191
    30, 30, 30, 30, 30, 30, 30, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 221, 222, 223,
    224, 225, 226, 227, 228, 229, 230, 0, 1, 2, 4, 6, 8, 9, 10, 12, 14, 15, 16, 18, 20, 22, 24, 26, 27, 28, 29, 30, 30, 30, 30, 30 // first 30 is 251th index
};

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
    //    if (height > 15) {
    //        height = (height / 8) - 1;
    //    } else {
    //        height = 0;
    //    }
    for (leds; leds != 0; leds--) {
        temp = (leds <= height) ? green : 0;
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
        max1 = 0;
        max2 = 0;
        max3 = 0;
        max4 = 0;
        tempIN = adConvert(LPF);
        LPFInput[lastVal] = tempIN;
        lastVal = (lastVal == 9) ? 0 : lastVal + 1;
        for (i = 0; i < 10; i++) {
            if (LPFInput[i] > max1) {
                max4 = max3;
                max3 = max2;
                max2 = max1;
                max1 = LPFInput[i];
            } else if (LPFInput[i] > max2) {
                max4 = max3;
                max3 = max2;
                max2 = LPFInput[i];
            } else if (LPFInput[i] > max3) {
                max4 = max3;
                max3 = LPFInput[i];
            } else if (LPFInput[i] > max4) {
                max4 = LPFInput[i];
            }
        }
        LPFAvg = (max1 + max2 + max3 + max4) / 4;
        lights(LPFLookupTable[LPFAvg], maxLEDs);
        __delay_ms(5);
    }
}