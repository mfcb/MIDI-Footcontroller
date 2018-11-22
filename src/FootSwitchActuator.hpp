//
//  FootSwitchActuator.hpp
//  
//
//  Created by Markus Buhl on 15.11.18.
//

#ifndef FootSwitchActuator_hpp
#define FootSwitchActuator_hpp

#include <stdio.h>
#include "Arduino.h"

enum ActuatorState {
    UNCHANGED = 0,
    INACTIVE,
    ACTIVE
};

enum ActuatorType {
    ONESHOT,
    HOLD,
    TOGGLE
};

typedef void (*FootSwitchActuatorCallback)(int id);

class FootSwitchActuator {
protected:
    //Analog or Digital Button Pin?
    static int lastID;
    int m_actuatorPin = -1;
    bool m_ready = false;
    bool m_isAnalog = false;
    int m_actuatorState = 0;
    int m_prevActuatorState = HIGH; //Begin on HIGH since we are dealing with Pullup Resistors (which are HIGH when circuit is open)
    int m_ID = lastID;
    //Debounce timing
    unsigned long m_lastDebounceTime = 0;
    const unsigned long m_debounceDelay = 30;
    bool m_active = false;
    
    ActuatorType m_actuatorType = ActuatorType::ONESHOT;
    FootSwitchActuatorCallback m_callback;
    
    int getActuatorState() {
        if(!m_ready) {
            initPin();
            m_ready = true;
        }
        int reading = digitalRead(m_actuatorPin);
        
        if(reading != m_prevActuatorState) {
            m_lastDebounceTime = millis();
        }
        
        if((millis() - m_lastDebounceTime) > m_debounceDelay) {
            if(reading != m_actuatorState) {
                m_actuatorState = reading;
            }
        }
        
        m_prevActuatorState = reading;
        return m_actuatorState;
    }
    
    void updateActuatorONESHOT(int state) {
        if(state) {
            if(m_active) {
                return;
            }
            activate();
        } else {
            if(!m_active) {
                return;
            }
            deactivate();
        }
    }
    
    void updateActuatorHOLD(int state) {
        if(state) {
            if(m_active) {
                return;
            }
            Serial.println("called an actuator");
            activate();
        } else {
            deactivate();
        }
    }
    
    void updateActuatorTOGGLE(int state) {
        
    }
    
    void activate() {
        m_active = true;
        if(m_callback != nullptr) {
            m_callback(m_ID);
        }
    }
    
    void deactivate() {
        m_active = false;
    }
    
public:
    //GETTERS & SETTERS
    FootSwitchActuator() {
        m_ID = lastID;
        lastID++;
    }
    
    FootSwitchActuator(int actuatorPin, bool isAnalog = false) {
        FootSwitchActuator();
        m_actuatorPin = actuatorPin;
        m_isAnalog = isAnalog;
    }
    
    void initPin() {
        pinMode(m_actuatorPin, INPUT_PULLUP);
    }
    
    void setIsAnalog(bool isAnalog) {
        m_isAnalog = isAnalog;
    }
    
    bool isAnalog() {
        return m_isAnalog;
    }
    
    void setID(int id) {
        m_ID = id;
    }
    
    int getID() {
        return m_ID;
    }
    
    void updateActuatorState() {
        if(m_isAnalog) {
            updateActuatorStateANALOG();
            return;
        }
        int state = !getActuatorState();
        //inverse logic: active actuator returns 0!
        switch(m_actuatorType) {
            case ActuatorType::ONESHOT:
                updateActuatorONESHOT(state);
                break;
            case ActuatorType::HOLD:
                updateActuatorHOLD(state);
                break;
            case ActuatorType::TOGGLE:
                updateActuatorTOGGLE(state);
                break;
        }
        
    }
    
    void updateActuatorStateANALOG() {
        int input = analogRead(m_actuatorPin);
        Serial.print("Analog 7: ");
        Serial.println(input);
    }
    
    void setActuatorPin(int actuatorPin) {
        m_actuatorPin = actuatorPin;
    }
    
    int getActuatorPin() {
        return m_actuatorPin;
    }
    
    
    
    int getReading() {
        if(m_isAnalog) {
            return analogRead(m_actuatorPin);
        } else {
            return digitalRead(m_actuatorPin);
        }
        
    }
    
    void setCallback(FootSwitchActuatorCallback callback) {
        m_callback = callback;
    }
    
    
};

#endif /* FootSwitchActuator_hpp */
