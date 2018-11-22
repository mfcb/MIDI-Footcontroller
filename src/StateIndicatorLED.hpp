//
//  StateIndicatorLED.hpp
//  
//
//  Created by Markus Buhl on 15.11.18.
//

#ifndef StateIndicatorLED_hpp
#define StateIndicatorLED_hpp

#include <stdio.h>
#include "StateIndicatorLED.hpp"
#include "Arduino.h"
#include "FastLED.h"

enum IndicatorLEDType {
    SOLID,
    BLINKING,
    BREATHING
};

class StateIndicatorLED {
protected:
    unsigned int m_LEDIndex;
    CRGB m_color;
    IndicatorLEDType m_LEDType;
    byte m_stateCounter = 0;
    const int m_interval = 50;
    const int m_incrementSpeed = 2;
public:
    StateIndicatorLED() {
        m_LEDIndex = 0;
        m_color = CRGB::Blue;
        m_LEDType = SOLID;
    }
    
    StateIndicatorLED(unsigned int LEDIndex, CRGB baseColor, IndicatorLEDType LEDType) {
        m_LEDIndex = LEDIndex;
        m_color = baseColor;
        m_LEDType = LEDType;
    }
    
    void setLEDIndex(unsigned int LEDIndex) {
        m_LEDIndex = LEDIndex;
    }
    
    unsigned int getLEDIndex() {
        return m_LEDIndex;
    }
    
    void setColor(CRGB color) {
        m_color = color;
    }
    
    CRGB getColor() {
        switch(m_LEDType) {
            case IndicatorLEDType::SOLID:
                return m_color;
                break;
            case IndicatorLEDType::BLINKING:
                m_stateCounter+=m_incrementSpeed;
                if(m_stateCounter > m_interval) {
                    m_stateCounter = 0;
                }
                return (m_stateCounter < m_interval / 2) ? m_color : CRGB::Black;
                break;
            case IndicatorLEDType::BREATHING:
                byte v = sin8(m_stateCounter);
                byte r = m_color.r / 255 * v;
                byte g = m_color.g / 255 * v;
                byte b = m_color.b / 255 * v;
                m_stateCounter+=m_incrementSpeed;
                return CRGB(r,g,b);
                break;
        }
        return m_color;
    }
    
    void setLEDType(IndicatorLEDType LEDType) {
        m_stateCounter = 0;
        m_LEDType = LEDType;
    }
    
    IndicatorLEDType getLEDType() {
        return m_LEDType;
    }
};


#endif /* StateIndicatorLED_hpp */
