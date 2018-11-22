/**
   I2C/IIC LCD Serial Adapter Module Example
   Tutorial by http://mklec.com

   Instructions at http://blog.mklec.com/how-to-use-iici2c-serial-interface-module-for-1602-lcd-display

   This uses the Liquid Crystal library from https://bitbucket.org/fmalpartida/new-liquidcrystal/downloads GNU General Public License, version 3 (GPL-3.0)
   Pin Connections:
        SCL = A5 (2)
        SDA = A4 (3)
        VCC = 5V
        GND = GND
*/


/*

   LED Numbering:

   ----TOP----
   LED 1 = 5
   LED 2 = 4
   LED 3 = 3
   LED 4 = 2
   LED 5 = 1
   LED 6 = 0
   --BOTTOM---
   LED 7 = 6
   LED 8 = 7
   LED 9 = 8
   LED 10 = 9
   LED 11 = 10
   LED 12 = 11

    5v - 0 -1 -2 -3 -4 -5 -6 -7
*/ //5v - 7- 6 -5 -4 -3 -2 -1 -0

#include <FastLED.h>
#include <MIDI.h>
#include "src/FootSwitchActuator.hpp"
#include "src/StateIndicatorLED.hpp"
#include "src/LCD2Line16Char.hpp"
#include "src/LEDBoard.hpp"

//Timing
const int setupTime = 800;
unsigned long lastMilli = 0;
const int interval = 20;

//MIDI
//Create Serial MIDI instance to accept incoming MIDI signals
MIDI_CREATE_DEFAULT_INSTANCE();

//LCD Display
LCD2Line16Char lcd;
const int lcdUpdateRate = 1000;

//LEDS
const int ledPin = 13;
const int nLEDS = 12;
int activeTrackButton = -1;
int activeBank = 0;
int selectedBank = 0;
CRGB bankColors[] = {CRGB::Blue, CRGB::Yellow, CRGB::Red};

LEDBoard<nLEDS, ledPin> ledBoard;

//BUTTONS
const int nButtons = 12;
//Button numbers are mirrored:
//6, 5, 4, 3, 2, 1, 12, 11, 10, 09, 08, 07
const int buttons[] = { 8, 7, 6, 5, 4, 3, A7, 13, 12, 11, 10, 9};
bool pushedButtons[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
const byte buttonNotes[] = { 54, 55, 56, 57, 58, 59, 60, 61, 62, 63 , 64 , 65 };
int j = 0;
bool isReady = false;
const int octaveSize = 12;
byte midiNotes[] = {41, 40, 39, 38, 37, 36, 42,43,44,45,46,47};
byte trackNumbers[] = {0,0,4,3,2,1,5,6,7,8,0,0, 0,0,12,11,10,9,13,14,15,16,0,0, 0,0,20,19,18,17,21,22,23,24,0,0};

//MODES OF OPERATION:
enum BoardMode {
  NORMAL = 0,
  SETUP,
};

BoardMode boardMode = NORMAL;

//TRACKBOARD:

//All Switches and Lights pertainting to track selection
//These are all Hold Buttons
void handleTrackButton(int id) {
  //Track Button ids match with LEDS
  //So: 8, 7, 6, 5, 12, 11, 10, 9
  activeTrackButton = id;
  activeBank = selectedBank;
  //MIDI.sendNoteOn(midiNotes[id]+activeBank*octaveSize, 127, 1);
  Serial.println(midiNotes[id]+activeBank*octaveSize);
  String message = "Track ";
  message.concat(trackNumbers[id+activeBank*octaveSize]);
  lcd.printMessageLine2(message);
  updateBoard();  
}

void handleBankUpButton(int id) {
  if(boardMode == BoardMode::SETUP) {
    //
  }
  if(selectedBank < 2) {
    selectedBank++;
  }
  String message = "Bank ";
  message.concat(selectedBank);
  
  lcd.printMessageLine2(message);
  updateBoard();
}

void handleBankDownButton(int id) {
  if(selectedBank >0) {
    selectedBank--;
  }
  String message = "Bank ";
  message.concat(selectedBank);
  lcd.printMessageLine2(message);
  updateBoard();
}

void updateBoard() {
  ledBoard.fillRange(2,6,bankColors[selectedBank]);
  ledBoard.fillRange(6,10,bankColors[selectedBank]);
  if(activeTrackButton != -1 && activeBank == selectedBank) {
    ledBoard.setLEDColorAt(activeTrackButton,CRGB::White);
  }
  ledBoard.flushLEDs();
}

FootSwitchActuator trackButton1(8, false);
FootSwitchActuator trackButton2(7, false);
FootSwitchActuator trackButton3(6, false);
FootSwitchActuator trackButton4(5, false);
FootSwitchActuator trackButton5(12, false);
FootSwitchActuator trackButton6(11, false);
FootSwitchActuator trackButton7(10, false);
FootSwitchActuator trackButton8(9, false);

FootSwitchActuator bankDownButton(4, false);
FootSwitchActuator bankUpButton(3, false);

FootSwitchActuator stopButton(A7, true);

void setup()
{
  digitalWrite(2, LOW);
  //LCD: Output Startup Message
  lcd.printMessageLine1("Starting");
  //Begin MIDI
  Serial.begin(9600);
  Serial.println("booting");
  MIDI.turnThruOff();
  //Set LEDs to blue
  ledBoard.init();
  ledBoard.fillAll(CRGB::Blue); //Blue is the startup color
  ledBoard.setLEDTypeOfRange(2, 6, IndicatorLEDType::SOLID);
  ledBoard.setLEDTypeOfRange(6, 10, IndicatorLEDType::SOLID);
  ledBoard.flushLEDs();

  //Footswitches
  //To match up with the LEDS, first Button needs ID 5, second needs 4, ...
  trackButton1.setCallback(handleTrackButton);
  trackButton1.setID(5);
  trackButton2.setCallback(handleTrackButton);
  trackButton2.setID(4);
  trackButton3.setCallback(handleTrackButton);
  trackButton3.setID(3);
  trackButton4.setCallback(handleTrackButton);
  trackButton4.setID(2);
  trackButton5.setCallback(handleTrackButton);
  trackButton5.setID(9);
  trackButton6.setCallback(handleTrackButton);
  trackButton6.setID(8);
  trackButton7.setCallback(handleTrackButton);
  trackButton7.setID(7);
  trackButton8.setCallback(handleTrackButton);
  trackButton8.setID(6);

  bankDownButton.setCallback(handleBankDownButton);
  bankDownButton.setID(1);
  bankUpButton.setCallback(handleBankUpButton);
  bankUpButton.setID(0);

  stopButton.setCallback(handleBankUpButton);
  stopButton.setID(11);

}





void loop()
{
  if (millis() > setupTime) { // Leave some time for the pedal to be ready
    if (!isReady) {
      lcd.printMessage("GroundControl", "Revision I");
      Serial.println("GroundControl Rev1 Ready");
      isReady = true;
    }
  }
  
  // Send note 42 with velocity 127 on channel 1
  if (millis() - lastMilli >= interval) {
    lastMilli = millis();
    ledBoard.flushLEDs();
  }
  trackButton1.updateActuatorState();
  trackButton2.updateActuatorState();
  trackButton3.updateActuatorState();
  trackButton4.updateActuatorState();
  trackButton5.updateActuatorState();
  trackButton6.updateActuatorState();
  trackButton7.updateActuatorState();
  trackButton8.updateActuatorState();

  bankDownButton.updateActuatorState();
  bankUpButton.updateActuatorState();

  stopButton.updateActuatorState();
}

