/*  
 *  GrayHill controller
 *  Developed for Arduino UNO and MCP2515
 *  Saoud AlNaqbi
 */

//This library depends on and requires TimerOne library, SPI library, and autowp mcp2515 library v1.03
#include "GrayHill_CANOPEN.h"
#include <mcp2515.h>


#define CS_PIN 10
#define INTERRUPT_PIN 3
#define KEYPAD_BASE_ID 0x70A
#define ENABLE_PASSCODE false

#define PIN_RELAY_1  1 // the Arduino pin, which connects to the IN1 pin of relay module
#define PIN_RELAY_2  3 // the Arduino pin, which connects to the IN2 pin of relay module
#define PIN_RELAY_3  4 // the Arduino pin, which connects to the IN3 pin of relay module
#define PIN_RELAY_4  5 // the Arduino pin, which connects to the IN4 pin of relay module
#define PIN_RELAY_5  6 // the Arduino pin, which connects to the IN5 pin of relay module
#define PIN_RELAY_6  7 // the Arduino pin, which connects to the IN6 pin of relay module
#define PIN_RELAY_7  8 // the Arduino pin, which connects to the IN7 pin of relay module
#define PIN_RELAY_8  9 // the Arduino pin, which connects to the IN8 pin of relay module

MCP2515 mcp2515(CS_PIN);
CANKeypad keypad(mcp2515, INTERRUPT_PIN, KEYPAD_BASE_ID, ENABLE_PASSCODE); 


void setup() {
  Serial.begin(115200);
  keypad.setSerial(&Serial); //Required for the keypad library to print things out to serial

  uint8_t keypadPasscode[4] = {1,2,3,4};
  keypad.setKeypadPassword(keypadPasscode);
  keypad.setKeyBrightness(70);





  // initialize digital pin as an output.
  pinMode(PIN_RELAY_1, OUTPUT);
  pinMode(PIN_RELAY_2, OUTPUT);
  pinMode(PIN_RELAY_3, OUTPUT);
  pinMode(PIN_RELAY_4, OUTPUT);
  pinMode(PIN_RELAY_5, OUTPUT);
  pinMode(PIN_RELAY_6, OUTPUT);
  pinMode(PIN_RELAY_7, OUTPUT);
  pinMode(PIN_RELAY_8, OUTPUT);
  
  digitalWrite(PIN_RELAY_1, 1);
  digitalWrite(PIN_RELAY_2, 1);
  digitalWrite(PIN_RELAY_3, 1);
  digitalWrite(PIN_RELAY_4, 1);
  digitalWrite(PIN_RELAY_5, 1);
  digitalWrite(PIN_RELAY_6, 1);
  digitalWrite(PIN_RELAY_7, 1);
  digitalWrite(PIN_RELAY_8, 1);




 //     MOMENTARY  TOGGLE  CYCLE3  CYCLE4  

  keypad.setKeyMode(PKP_KEY_1, BUTTON_MODE_CYCLE3);                // ... master on/off - ECU       
  keypad.setKeyMode(PKP_KEY_2, BUTTON_MODE_MOMENTARY);            // ... Starter         
  keypad.setKeyMode(PKP_KEY_3, BUTTON_MODE_TOGGLE);              // ... FAN              
  keypad.setKeyMode(PKP_KEY_4, BUTTON_MODE_CYCLE3);             // ... Light              
  keypad.setKeyMode(PKP_KEY_5, BUTTON_MODE_CYCLE3);            // ... AUX1 AUX2          
  keypad.setKeyMode(PKP_KEY_6, BUTTON_MODE_TOGGLE);           // ... AUX3 AUX4                
  keypad.setKeyMode(PKP_KEY_7, BUTTON_MODE_MOMENTARY);          // ... DATA LOG          
  keypad.setKeyMode(PKP_KEY_8, BUTTON_MODE_CYCLE4);         // ... Dash Next            
  


  uint8_t defaultStates[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
  keypad.setDefaultButtonStates(defaultStates);

  keypad.begin(CAN_500KBPS, MCP_8MHZ); //These are MCP settings to be passed  
}

//----------------------------------------------------------------------------new

static uint8_t prevButtonStates[12] = {0}; // initialize the previous button state array
static byte byte0 = 0x00;
static byte byte1 = 0x00;
static byte byte2 = 0x00;

void loop() {
  keypad.process(); //must have this in main loop.

  static uint8_t prevButtonStates[12] = {0}; // initialize the previous button state array

  // check for button state changes and send messages over the CAN bus
  for(int i=0; i<12; i++) {
    if (keypad.buttonState[i] != prevButtonStates[i]) {
      uint8_t buttonState = keypad.buttonState[i];
      prevButtonStates[i] = buttonState;

      

      if (i == 0) { // Button 1
      byte0 &= ~(0x01);
      byte0 &= ~(0x02);
      byte0 &= ~(0x04);
        if (buttonState == 1) { // First LED on, others off
          byte0 |= 0x01;
        } else if (buttonState == 2) { // First two LEDs on, others off
          byte0 |= 0x02;
          byte0 |= 0x01;
        } else if (buttonState == 3) { // All three LEDs on
          byte0 |= 0x04;
          byte0 |= 0x02;
          byte0 |= 0x01;
        }
      } else if (i == 1) { // Button 2
      byte0 &= ~(0x08); 
      byte0 &= ~(0x10);
      byte0 &= ~(0x20);
        if (buttonState == 1) { // First LED on, others off
          byte0 |= 0x08;
        } else if (buttonState == 2) { // First two LEDs on, others off
          byte0 |= 0x08;
          byte0 |= 0x10;
        } else if (buttonState == 3) { // First three LEDs on, others off
          byte0 |= 0x08;
          byte0 |= 0x10;
          byte0 |= 0x20;
        }
        } else if (i == 2) { // Button 3
        byte0 &= ~(0x40);
        byte0 &= ~(0x80);
        byte1 &= ~(0x01);  
        if (buttonState == 1) { // First LED on, others off
          byte0 |= 0x40;
        } else if (buttonState == 2) { // First two LEDs on, others off
          byte0 |= 0x40;
          byte0 |= 0x80;
        } else if (buttonState == 3) { // First three LEDs on, others off
          byte0 |= 0x40;
          byte0 |= 0x80;
          byte1 |= 0x01;
        }
        } else if (i == 3) { // Button 4
        byte1 &= ~(0x02);
        byte1 &= ~(0x04);
        byte1 &= ~(0x08); 
        if (buttonState == 1) { // First LED on, others off
          byte1 |= 0x02;
        } else if (buttonState == 2) { // First two LEDs on, others off
          byte1 |= 0x04;
          byte1 |= 0x02;
        } else if (buttonState == 3) { // First three LEDs on, others off
          byte1 |= 0x08;
          byte1 |= 0x04;
          byte1 |= 0x02;
        }
        } else if (i == 4) { // Button 5
        byte1 &= ~(0x10);
        byte1 &= ~(0x20);
        byte1 &= ~(0x40); 
        if (buttonState == 1) { // First LED on, others off
          byte1 |= 0x10;
        } else if (buttonState == 2) { // First two LEDs on, others off
          byte1 |= 0x10;
          byte1 |= 0x20;
        } else if (buttonState == 3) { // First three LEDs on, others off
          byte1 |= 0x10;
          byte1 |= 0x20;
          byte1 |= 0x40;
        }
        } else if (i == 5) { // Button 6
        byte1 &= ~(0x80);
        byte2 &= ~(0x01);
        byte2 &= ~(0x02); 
        if (buttonState == 1) { // First LED on, others off
          byte1 |= 0x80;
        } else if (buttonState == 2) { // First two LEDs on, others off
          byte1 |= 0x80;
          byte2 |= 0x01;
        } else if (buttonState == 3) { // First three LEDs on, others off
          byte1 |= 0x80;
          byte2 |= 0x01;
          byte2 |= 0x02;
        }
        } else if (i == 6) { // Button 7
        byte2 &= ~(0x04);
        byte2 &= ~(0x08);
        byte2 &= ~(0x10); 
        if (buttonState == 1) { // First LED on, others off
          byte2 |= 0x04;
        } else if (buttonState == 2) { // First two LEDs on, others off
          byte2 |= 0x04;
          byte2 |= 0x08;
        } else if (buttonState == 3) { // First three LEDs on, others off
          byte2 |= 0x04;
          byte2 |= 0x08;
          byte2 |= 0x10;
        }
        } else if (i == 7) { // Button 8
        byte2 &= ~(0x20);
        byte2 &= ~(0x40);
        byte2 &= ~(0x80); 
        if (buttonState == 1) { // First LED on, others off
          byte2 |= 0x20;
        } else if (buttonState == 2) { // First two LEDs on, others off
          byte2 |= 0x40;
        } else if (buttonState == 3) { // First three LEDs on, others off
          byte2 |= 0x80;
        }
      }
      

      // Construct and send the CAN frame
      can_frame frame;
      frame.can_id = 0x20A; // use unique CAN ID
      frame.can_dlc = 8; // send 8 bytes of data
      frame.data[0] = byte0; // send the byte0 value as data
      frame.data[1] = byte1; // send the byte1 value as data
      frame.data[2] = byte2; // send the byte2 value as data
      mcp2515.sendMessage(&frame);
    }
  }
}
//----------------------------------------------------------------------------new