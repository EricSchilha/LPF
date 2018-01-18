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
unsigned char green = 255;
unsigned char blue = 0;

bool increasing = true;

unsigned char tempRed;
unsigned char tempGreen;
unsigned char tempBlue;

const char maxLEDs = 24;

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

void lights(unsigned char height, unsigned char max) {
//        if (height > 15) {
//            height = (height / 8) - 1;
//        } else {
//            height = 0;
//        }
        height = (height - 63) / 8;
    for (unsigned char leds = 0; leds != max; leds++) {
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
        temp = (leds <= height) ? red : 0;
        for (i = 8; i != 0; i--) {
            asm("bsf LATA,4");
            asm("nop");
            asm("btfss _temp,7");
            asm("bcf LATA,4");
            asm("nop");
            asm("lslf _temp,f");
            asm("bcf LATA,4");
        }
        temp = (leds <= height) ? blue : 0;
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
        if(S1 == 0){
            if(red == 255){
                if(increasing){
                    green++;
                    if(green == 255){
                        increasing = false;
                        red--;
                    }
                } else {
                    blue--;
                    if(blue == 0){
                        increasing = true;
                    }
                }
            } else if(green == 255) {
                if(increasing){
                    blue++;
                    if(blue == 255){
                        increasing = false;
                        green--;
                    }
                } else {
                    red--;
                    if(red == 0){
                        increasing = true;
                    }
                }
            } else if(blue == 255) {
                if(increasing){
                    red++;
                    if(red == 255){
                        increasing = false;
                        blue--;
                    }
                } else {
                    green--;
                    if(green == 0){
                        increasing = true;
                    }
                }
            }
        }
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
        lights(LPFAvg, maxLEDs);
        __delay_ms(5);
    }
}