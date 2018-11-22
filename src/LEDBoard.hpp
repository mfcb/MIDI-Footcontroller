//
//  LEDBoard.hpp
//  
//
//  Created by Markus Buhl on 16.11.18.
//

#ifndef LEDBoard_hpp
#define LEDBoard_hpp

#include <stdio.h>
#include "FastLED.h"
#include "StateIndicatorLED.hpp"

template<int nLEDs, int ledPin>
class LEDBoard {
protected:
    struct CRGB m_physicalLEDs[nLEDs];
    StateIndicatorLED m_stateIndicatorLEDs[nLEDs];
    int m_nLEDs = nLEDs;
    int m_LEDPointer = 0;
public:
    LEDBoard() {
        for(int i=0;i<nLEDs;i++) {
            m_stateIndicatorLEDs[i].setLEDIndex(i);
        }
    };
    
    void init() {
        FastLED.addLeds<WS2812, ledPin, RGB>(m_physicalLEDs, 0, nLEDs);
        FastLED.setCorrection(LEDColorCorrection::Typical8mmPixel);
        FastLED.setTemperature(ColorTemperature::DirectSunlight);
        FastLED.clear();
    }
    
    void setLEDTypeAt(int index, IndicatorLEDType type) {
        m_stateIndicatorLEDs[index].setLEDType(type);
    }
    
    void setLEDTypeOfRange(int start, int end, IndicatorLEDType type) {
        for(int i=start;i<end;i++) {
            m_stateIndicatorLEDs[i].setLEDType(type);
        }
    }
    void setLEDTypeAll(IndicatorLEDType type) {
        for(int i=0;i<nLEDs;i++) {
            m_stateIndicatorLEDs[i].setLEDType(type);
        }
    }
    
    void setLEDColorAt(int index, CRGB color) {
        m_stateIndicatorLEDs[index].setColor(color);
    }
    
    void fillAll(CRGB color) {
        for(int i=0;i<m_nLEDs;i++) {
            m_stateIndicatorLEDs[i].setColor(color);
        }
    }
    
    void fillRange(int start, int end, CRGB color) {
        for(int i = start;i<end;i++) {
            m_stateIndicatorLEDs[i].setColor(color);
        }
    }
    
    void flushLEDs() {
        for(int i=0;i<m_nLEDs;i++) {
            m_physicalLEDs[i] = m_stateIndicatorLEDs[i].getColor();
        }
        FastLED.show();
    }
    
    
    
};

#endif /* LEDBoard_hpp */
