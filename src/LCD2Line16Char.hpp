//
//  LCD2Line16Char.hpp
//  
//
//  Created by Markus Buhl on 16.11.18.
//

#ifndef LCD2Line16Char_hpp
#define LCD2Line16Char_hpp

#include <stdio.h>
#include "Wire.h"
#include "LCD.h"
#include "LiquidCrystal_I2C.h"

#define LCD_lSize 16
class LCD2Line16Char {
protected:
    LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 2, 1, 0, 4, 5, 6, 7); // 0x27 is the I2C bus address for an unmodified module ORIGINAL: 0x27,2,1,0,4,5,6,7
    String m_line1;
    String m_line2;
    bool m_ready = false;
    
    void flush() {
        if(!m_ready) {
            init();
        }
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print(m_line1);
        lcd.setCursor(0,1);
        lcd.print(m_line2);
    }
public:
    LCD2Line16Char() {
    }
    
    void init() {
        lcd.setBacklightPin(3, POSITIVE);
        lcd.setBacklight(HIGH); // NOTE: You can turn the backlight off by setting it to LOW instead of HIGH
        lcd.begin(16, 2);
        m_ready = true;
    }
    void printMessage(String line1, String line2) {
        m_line1 = line1;
        m_line2 = line2;
        flush();
    };
    void printMessageLine1(String message) {
        m_line1 = message;
        flush();
    };
    void printMessageLine2(String message) {
        m_line2 = message;
        flush();
    };
    
    
    
};

#endif /* LCD2Line16Char_hpp */
