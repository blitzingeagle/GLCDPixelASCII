/*
 * File:   main.c
 * Author: Morris Chen
 * Copyright © 2018 Morris Chen. All rights reserved.
 * 
 * Created on January 31, 2018, 6:44 PM
 * 
 * Description: Demonstration of using bitmasks to create a typeset for GLCD
 * 
 * Preconditions:
 *   1. GLCD is in a PIC socket
 */

/***** Includes *****/
#include <xc.h>
#include "config_bits.h"
#include "GLCD_PIC.h"
#include "px_ascii.h"

void init(void) {
    // <editor-fold defaultstate="collapsed" desc="Machine Configuration">
    /********************************* PIN I/O ********************************/
    /* Write outputs to LATx, read inputs from PORTx. Here, all latches (LATx)
     * are being cleared (set low) to ensure a controlled start-up state. */  
    LATA = 0x00;
    LATB = 0x00; 
    LATC = 0x00;
    LATD = 0x00;
    LATE = 0x00;

    /* After the states of LATx are known, the data direction registers, TRISx
     * are configured. 0 --> output; 1 --> input. Default is  1. */
    TRISA = 0xFF; // All inputs (this is the default, but is explicated here for learning purposes)
    TRISB = 0xFF;
    TRISC = 0b10000000; /* RC3 is SCK/SCL (SPI/I2C),
                         * RC4 is SDA (I2C),
                         * RC5 is SDA (SPI),
                         * RC6 and RC7 are UART TX and RX, respectively. */
    TRISD = 0b00000001; /* RD0 is the GLCD chip select (tri-stated so that it's
                         * pulled up by default),
                         * RD1 is the GLCD register select,
                         * RD2 is the character LCD RS,
                         * RD3 is the character LCD enable (E),
                         * RD4-RD7 are character LCD data lines. */
    TRISE = 0b00000100; /* RE2 is the SD card chip select (tri-stated so that
                         * it's pulled up by default).
                         * Note that the upper 4 bits of TRISE are special 
                         * control registers. Do not write to them unless you 
                         * read §9.6 in the datasheet */
    
    /************************** A/D Converter Module **************************/
    ADCON0 = 0x00;  // Disable ADC
    ADCON1 = 0b00001111; // Set all A/D ports to digital (pg. 222)
    // </editor-fold>
    
    /* Initialize GLCD. */
    initGLCD();
}

void draw_px_char(int x, int y, px_char p) {
    uint32_t m = 0b1;
    for(int i = 0; i < 32; i++) {
        if(m & p.part[0]) glcdDrawPixel(x+i%PX_CHAR_WIDTH, y+i/PX_CHAR_WIDTH, BLACK);
        if(m & p.part[1]) glcdDrawPixel(x+(i+32)%PX_CHAR_WIDTH, y+(i+32)/PX_CHAR_WIDTH, BLACK);

        m <<= 1;
    }
}

void main(void) {
    init();
    
    /* Fill screen with rainbow colors. */
    glcdDrawRectangle(0, GLCD_SIZE_VERT, 0, 128, RED);
    
    print_px_string(0, 14, "         MAKE        ");
    print_px_string(0, 28, "       AMERICA       ");
    print_px_string(0, 42, "        GREAT        ");
    print_px_string(0, 56, "        AGAIN        ");
    print_px_string(0, 84, "      Trump 2020     ");
    
    while(1) {
    }
    
    return;
}
