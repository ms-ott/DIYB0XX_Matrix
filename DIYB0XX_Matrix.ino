/*
  DIYB0XX Native USB v1.202 code by Crane.
  Matrix patch writen by ms-ott
  This code utilizes
    Nicohood's Nintendo library
    MHeironimus' Arduino Joystick Library

  This code is for DIYB0XXes using Arduinos with 8 bit AVR microcontrollers
  running at 16 MHz that support native usb (Micro, Leonardo), as well as for those using a matrix input
  layout to allow for more buttons. 

  For more information on button matricies, check out https://www.dribin.org/dave/keyboard/keyboard.pdf

*/
#include "Nintendo.h"
#include <Joystick.h>

const byte ROWS = 7; 
const byte COLS = 3;
byte buttons[ROWS][COLS] = { //Main matrix array
{1,2,3},{4,5,6},
{7,8,9},{10,11,12}, 
{13,14,15},{16,17,18}, 
{19,20,21} 
};

bool keys_status_last[ROWS][COLS] = 
{
  {false,false,false}, {false,false,false},
  {false,false,false}, {false,false,false},
  {false,false,false}, {false,false,false},
  {false,false,false}
};

byte row_pins[ROWS] = {2, 3, 4, 5, 6, 7, 8,}; // row pins
byte col_pins[COLS] = {A0, A1, A2}; //column pins

bool isLightShieldButtons = true;
bool UseNewModVertical = true;

uint8_t fTwoIPNoReactivate(bool isLOW, bool isHIGH, bool& wasLOW, bool& wasHIGH, bool& lockLOW, bool& lockHIGH);
uint8_t fTwoIP(bool isLOW, bool isHIGH, bool& wasLOW, bool& wasHIGH);
uint8_t fNeutral(bool isLOW, bool isHIGH);

Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID, JOYSTICK_TYPE_GAMEPAD,
                   21, 0,                  // Button Count, Hat Switch Count
                   true, true, false,     // X and Y, but no Z Axis
                   true, true, false,   // Rx, Ry, no Rz
                   false, true,          // No rudder, throttle
                   false, false, false);  // No accelerator, no brake, no steering

CGamecubeConsole GamecubeConsole(A5);
Gamecube_Data_t d = defaultGamecubeData;

enum game
{
  Melee,
  PM,
  Ultimate,
  Rivals,
};

enum device
{
  GC,
  PC
};

enum SOCD
{
  Neutral,
  TwoIP,
  TwoIPNoReactivate
};

bool wasLEFT = false;
bool wasRIGHT = false;
bool wasUP = false;
bool wasDOWN = false;

bool wasCLEFT = false;
bool wasCRIGHT = false;
bool wasCUP = false;
bool wasCDOWN = false;

bool lockLEFT = false;
bool lockRIGHT = false;
bool lockUP = false;
bool lockDOWN = false;

bool lockCLEFT = false;
bool lockCRIGHT = false;
bool lockCUP = false;
bool lockCDOWN = false;

game currentGame = Rivals;
device currentDevice = PC;
SOCD currentSOCD = TwoIPNoReactivate;

                                              // EXTREMELY IMPORTANT V V V
                    // Device and Game need to be set above before using the B0XX.
                    // I couldn't implement button holding on plugin as it does not work with a matrix
                    // (Or at least I haven't figured out how to make it work yet).
                    // You could also totally use a seperate switch with the original method.
                                              // EXTREMELY IMPORTANT ^ ^ ^

bool isL = 0;
bool isLEFT = 0;
bool isDOWN = 0;
bool isRIGHT = 0;
bool isMODX = 0;
bool isMODY = 0;
bool isSTART = 0;
bool isB = 0;
bool isX = 0;
bool isZ = 0;
bool isUP = 0;
bool isR = 0;
bool isY = 0;
bool isCDOWN = 0;
bool isA = 0;
bool isCRIGHT = 0;
bool isCLEFT = 0;
bool isCUP = 0;
bool isLIGHTSHIELD = 0;
bool isMIDSHIELD = 0;
bool isEXTRA1 = 0;
bool isEXTRA2 = 0;

bool isDPADUP = 0;
bool isDPADDOWN = 0;
bool isDPADLEFT = 0;
bool isDPADRIGHT = 0;

const uint8_t minValue = 28;
const uint8_t maxValue = 228;

void setup()
{

  if (currentGame == Ultimate) {
    currentSOCD = TwoIP;
  }
  if (currentGame == Rivals) {
    currentSOCD = Neutral;
  }
  
  for(int col = 0; col < COLS; col++) {
    pinMode( col_pins[col], INPUT_PULLUP );
  }

  for( int row = 0; row < ROWS; row++) {
    pinMode (row_pins[row], OUTPUT );
    digitalWrite( row_pins[row], HIGH );
  }
  
  if (currentDevice == PC){
  Joystick.begin();
      Joystick.setXAxisRange(0, 255);
      Joystick.setYAxisRange(0, 255);
      Joystick.setRxAxisRange(0, 255);
      Joystick.setRyAxisRange(0, 255);
      Joystick.setThrottleRange(0, 255);
  }
}

void loop()
{
  for( int row = 0; row < ROWS; row++ ) {  //This is what polls the button matrix
    digitalWrite( row_pins[row], LOW );
    for( int col = 0; col < COLS; col++) {
      boolean result = !digitalRead( col_pins[col] );
      
      if( result == HIGH && keys_status_last[row][col] == false) { //pressed
        if (buttons[row][col] == 1){
          isZ = 1; // Change any of these values based on your pins
          }
          
          else if (buttons[row][col] == 2){
          isCRIGHT = 1;
          }
          
          else if (buttons[row][col] == 3){
          isDPADUP = 1;
          }
          
          else if (buttons[row][col] == 4){
            if (currentGame == Rivals) {
              isEXTRA1 = 1;
            }
          else {isLIGHTSHIELD = 1;}
          }
          
          else if (buttons[row][col] == 5){
          isLEFT = 1;
          }
          
          else if (buttons[row][col] == 6){
          isA = 1; //
          }
          
          else if (buttons[row][col] == 7){
          isX = 1; //
          }
          
          else if (buttons[row][col] == 8){
          isCDOWN = 1;
          }
          
          else if (buttons[row][col] == 9){
          isL = 1; //
          }
          
          else if (buttons[row][col] == 10){
          isY = 1;
          }
          
          else if (buttons[row][col] == 11){
          isCLEFT = 1;
          }
          
          else if (buttons[row][col] == 12){
          isDOWN = 1;
          }
          
          else if (buttons[row][col] == 13){
          isR = 1; //
          }
          
          else if (buttons[row][col] == 14){
          isCUP = 1;
          }

          else if (buttons[row][col] == 15){
          isRIGHT = 1;
          }

          else if (buttons[row][col] == 16){
          isB = 1; //
          }

          else if (buttons[row][col] == 17){
          isUP = 1; //
          }

          else if (buttons[row][col] == 18){
          isMODX = 1;
          }

          else if (buttons[row][col] == 19){
          isSTART = 1;
          }

          else if (buttons[row][col] == 20){
            if (currentGame == Rivals) {
              isEXTRA2 = 1;
            }
          else {isMIDSHIELD = 1;}
          }

          else if (buttons[row][col] == 21){
          isMODY = 1;
          }
        }
      else if( result == LOW && keys_status_last[row][col] == true) { //released
        if (buttons[row][col] == 1){
          isZ = 0; // If you add/remove a press event, don't forget to add/remove it here too!
          }
          
          else if (buttons[row][col] == 2){
          isCRIGHT = 0;
          }
          
          else if (buttons[row][col] == 3){
          isDPADUP = 0;
          }
          
          else if (buttons[row][col] == 4){
            if (currentGame == Rivals) {
              isEXTRA1 = 0;
            }
          else {isLIGHTSHIELD = 0;}
          }
          
          else if (buttons[row][col] == 5){
          isLEFT = 0;
          }
          
          else if (buttons[row][col] == 6){
          isA = 0; //
          }
          
          else if (buttons[row][col] == 7){
          isX = 0; //
          }
          
          else if (buttons[row][col] == 8){
          isCDOWN = 0;
          }
          
          else if (buttons[row][col] == 9){
          isL = 0; //
          }
          
          else if (buttons[row][col] == 10){
          isY = 0;
          }
          
          else if (buttons[row][col] == 11){
          isCLEFT = 0;
          }
          
          else if (buttons[row][col] == 12){
          isDOWN = 0;
          }
          
          else if (buttons[row][col] == 13){
          isR = 0; //
          }
          
          else if (buttons[row][col] == 14){
          isCUP = 0;
          }

          else if (buttons[row][col] == 15){
          isRIGHT = 0;
          }

          else if (buttons[row][col] == 16){
          isB = 0; //
          }

          else if (buttons[row][col] == 17){
          isUP = 0; //
          }

          else if (buttons[row][col] == 18){
          isMODX = 0;
          }

          else if (buttons[row][col] == 19){
          isSTART = 0;
          }

          else if (buttons[row][col] == 20){
            if (currentGame == Rivals) {
              isEXTRA2 = 0;
            }
          else {isMIDSHIELD = 0;}
          }

          else if (buttons[row][col] == 21){
          isMODY = 0;
          }
        }
    keys_status_last[row][col] = result; }
  
  digitalWrite( row_pins[row], HIGH ); }

  uint8_t controlX = 128;
  uint8_t controlY = 128;
  uint8_t cstickX = 128;
  uint8_t cstickY = 128;
  uint8_t RLight = 0;
  uint8_t LLight = 0;

  int8_t positionX = 0;
  int8_t positionY = 0;
  int8_t positionCX = 0;
  int8_t positionCY = 0;

  bool HORIZONTAL = false;
  bool VERTICAL = false;
  bool DIAGONAL = false;

  /********* SOCD *********/
  if (currentSOCD == TwoIPNoReactivate) {
  controlX = fTwoIPNoReactivate(isLEFT, isRIGHT, wasLEFT, wasRIGHT, lockLEFT, lockRIGHT);
    controlY = fTwoIPNoReactivate(isDOWN, isUP, wasDOWN, wasUP, lockDOWN, lockUP);
    cstickX = fTwoIPNoReactivate(isCLEFT, isCRIGHT, wasCLEFT, wasCRIGHT, lockCLEFT, lockCRIGHT);
    cstickY = fTwoIPNoReactivate(isCDOWN, isCUP, wasCDOWN, wasCUP, lockCDOWN, lockCUP);
  }

  if (currentSOCD == TwoIP) {
  controlX = fTwoIP(isLEFT, isRIGHT, wasLEFT, wasRIGHT);
    controlY = fTwoIP(isDOWN, isUP, wasDOWN, wasUP);
    cstickX = fTwoIP(isCLEFT, isCRIGHT, wasCLEFT, wasCRIGHT);
    cstickY = fTwoIP(isCDOWN, isCUP, wasCDOWN, wasCUP);
  }

  if (currentSOCD == Neutral) {
  controlX = fNeutral(isLEFT, isRIGHT);
    controlY = fNeutral(isDOWN, isUP);
    cstickX = fNeutral(isCLEFT, isCRIGHT);
    cstickY = fNeutral(isCDOWN, isCUP);
  }

  if (controlX != 128) {
  HORIZONTAL = true;
  if (controlX == minValue) positionX = -1;
    else positionX = 1;
  }
  if (controlY != 128) {
  VERTICAL = true;
  if (controlY == minValue) positionY = -1;
    else positionY = 1;
  }
  if (HORIZONTAL && VERTICAL) DIAGONAL = true;

  if (cstickX != 128) {
    if (cstickX == minValue) positionCX = -1;
      else positionCX = 1;
    }
  if (cstickY != 128) {
  if (cstickY == minValue) positionCY = -1;
    else positionCY = 1;
  }
  /********* Modifiers *********/

  if (isMODX) {
  if (HORIZONTAL) {
      if (currentGame == Melee) controlX = 128 + (positionX * 53);
      if (currentGame == Ultimate) controlX = 128 + (positionX * 40);
      if (currentGame == PM) controlX = 128 + (positionX * 70);
      if (currentGame == Rivals) controlX = 128 + (positionX * 40);
    }
    if (VERTICAL) {
      if (currentGame == Melee) {
        if (UseNewModVertical)
          controlY = 128 + (positionY * 23);
        else
          controlY = 128 + (positionY * 52);
      }
      if (currentGame == Ultimate) controlY = 128 + (positionY * 49);
      if (currentGame == PM) controlY = 128 + (positionY * 60);
      if (currentGame == Rivals) controlY = 128 + (positionY * 40);
    }

    if (isB) {
      if (currentGame == Melee) controlX = 128 + (positionX * 53);
      if (currentGame == Ultimate) {
        controlX = 128 + (positionX * 47);
        controlY = 128 + (positionY * 41);
      }
    }
    if (positionCX != 0) {
      cstickX = 128 + (positionCX * 65);
      cstickY = 128 + (positionY * 23);
    }

    if (DIAGONAL) {
      if (currentGame == Melee) {
        controlX = 128 + (positionX * 59);
        controlY = 128 + (positionY * 23);
      }
      if (currentGame == Ultimate) {
        controlX = 128 + (positionX * 40);
        controlY = 128 + (positionY * 26);
        if (isB) controlX = 128 + (positionX * 53);
      }
      if (currentGame == PM) {
        controlX = 128 + (positionX * 70);
        controlY = 128 + (positionY * 34);
      }

      if (isCUP) {
        if (currentGame == Melee) {
          controlX = 128 + (positionX * 49);
          controlY = 128 + (positionY * 35);
        }
        if (currentGame == Ultimate) {
          controlX = 128 + (positionX * 71);
          controlY = 128 + (positionY * 35);
        }
        if (currentGame == PM) {
          controlX = 128 + (positionX * 77);
          controlY = 128 + (positionY * 55);
        }
      }

      if (isCDOWN) {
        if (currentGame == Melee) {
          controlX = 128 + (positionX * 49);
          controlY = 128 + (positionY * 24);
        }
        if (currentGame == Ultimate) {
          controlX = 128 + (positionX * 61);
          controlY = 128 + (positionY * 49);
        }
        if (currentGame == PM) {
          controlX = 128 + (positionX * 82);
          controlY = 128 + (positionY * 32);
        }
      }

      if (isCLEFT) {
        if (currentGame == Melee) {
          controlX = 128 + (positionX * 52);
          controlY = 128 + (positionY * 31);
        }
        if (currentGame == Ultimate) {
          controlX = 128 + (positionX * 66);
          controlY = 128 + (positionY * 42);
        }
        if (currentGame == PM) {
          controlX = 128 + (positionX * 84);
          controlY = 128 + (positionY * 50);
        }
      }

      if (isCRIGHT) {
        if (currentGame == Melee) {
          controlX = 128 + (positionX * 51);
          controlY = 128 + (positionY * 43);
        }
        if (currentGame == Ultimate) {
          controlX = 128 + (positionX * 75);
          controlY = 128 + (positionY * 27);
        }
        if (currentGame == PM) {
          controlX = 128 + (positionX * 72);
          controlY = 128 + (positionY * 61);
        }
      }
    }
  }

  if (isMODY) {
  if (HORIZONTAL) {
      if (currentGame == Melee) controlX = 128 + (positionX * 27);
      if (currentGame == Ultimate) controlX = 128 + (positionX * 27);
      if (currentGame == PM) controlX = 128 + (positionX * 28);
      if (currentGame == Rivals) controlX = 128 + (positionX * 27);
    }
    if (VERTICAL) {
      if (currentGame == Melee) controlY = 128 + (positionY * 59);
      if (currentGame == Ultimate) controlY = 128 + (positionY * 51);
      if (currentGame == PM) controlY = 128 + (positionY * 34);
      if (currentGame == Rivals) controlY = 128 + (positionY * 50);
    }

    if (isB) {
      if (currentGame == Melee) controlX = 128 + (positionX * 80);
      if (currentGame == Ultimate) {
        controlX = 128 + (positionX * 41);
        controlY = 128 + (positionY * 61);
      }
      if (currentGame == PM) controlX = 128 + (positionX * 59);
    }

    if (DIAGONAL) {
      if (currentGame == Melee) {
        controlX = 128 + (positionX * 23);
        controlY = 128 + (positionY * 59);
      }
      if (currentGame == Ultimate) {
        controlX = 128 + (positionX * 38);
        controlY = 128 + (positionY * 49);
      }
      if (currentGame == PM) {
        controlX = 128 + (positionX * 28);
        controlY = 128 + (positionY * 58);
      }

      if (isCUP) {
        if (currentGame == Melee) {
          controlX = 128 + (positionX * 45);
          controlY = 128 + (positionY * 63);
        }
        if (currentGame == Ultimate) {
          controlX = 128 + (positionX * 35);
          controlY = 128 + (positionY * 71);
        }
        if (currentGame == PM) {
          controlX = 128 + (positionX * 55);
          controlY = 128 + (positionY * 77);
        }
      }

      if (isCDOWN) {
        if (currentGame == Melee) {
          controlX = 128 + (positionX * 28);
          controlY = 128 + (positionY * 57);
        }
        if (currentGame == Ultimate) {
          controlX = 128 + (positionX * 49);
          controlY = 128 + (positionY * 61);
        }
        if (currentGame == PM) {
          controlX = 128 + (positionX * 32);
          controlY = 128 + (positionY * 82);
        }
      }

      if (isCLEFT) {
        if (currentGame == Melee) {
          controlX = 128 + (positionX * 34);
          controlY = 128 + (positionY * 57);
        }
        if (currentGame == Ultimate) {
          controlX = 128 + (positionX * 42);
          controlY = 128 + (positionY * 66);
        }
        if (currentGame == PM) {
          controlX = 128 + (positionX * 50);
          controlY = 128 + (positionY * 84);
        }
      }

      if (isCRIGHT) {
        if (currentGame == Melee) {
          controlX = 128 + (positionX * 47);
          controlY = 128 + (positionY * 57);
        }
        if (currentGame == Ultimate) {
          controlX = 128 + (positionX * 27);
          controlY = 128 + (positionY * 75);
        }
        if (currentGame == PM) {
          controlX = 128 + (positionX * 62);
          controlY = 128 + (positionY * 72);
        }
      }
    }
  }

  if (isLEFT && isRIGHT && !VERTICAL)
  controlX = 128 + (positionX * 100);


  if (isLightShieldButtons && (currentGame == Melee)) {
      if (isLIGHTSHIELD || isMIDSHIELD) {
        if (currentDevice == PC) {
          if (isLIGHTSHIELD) RLight = 50;
          if (isMIDSHIELD) RLight = 95;
        }
        else {
          if (isLIGHTSHIELD) RLight = 80;
          if (isMIDSHIELD) RLight = 125;
        }


        if (HORIZONTAL && (positionY == -1)) {
          controlX = 128 + (positionX * 57);
          controlY = 128 - 55;
        }

        if (isMODX) {
          if (HORIZONTAL) {
            if (currentGame == Ultimate)
              controlX = 128 + (positionX * 51);
            if (currentGame == Melee)
              controlX = 128 + (positionX * 51);
            if (currentGame == PM)
              controlX = 128 + (positionX * 48);
          }

          if (VERTICAL) {
            if (currentGame == Ultimate)
              controlY = 128 + (positionY * 51);
            if (currentGame == Melee)
              controlY = 128 + (positionY * 43);
            if (currentGame == PM)
              controlY = 128 + (positionY * 48);
          }

          if (DIAGONAL)
            if (currentGame == Melee) controlX = 128 + (positionX * 43);

        }
      }
    }

  if (isL) {
  LLight = 140;
  if (HORIZONTAL) controlX = 128 + (positionX * 100);
    if (VERTICAL) controlY = 128 + (positionY * 100);
    if (HORIZONTAL && (positionY == 1)) {
      if (currentGame == Melee) {
        controlX = 128 + (positionX * 43);
        controlY = 128 + 43;
      }
      if (currentGame == PM) {
        controlX = 128 + (positionX * 67);
        controlY = 128 + 67;
      }
    }
    if (HORIZONTAL && (positionY == -1)) {
      controlX = 128 + (positionX * 57);
      if (currentGame == Melee) controlY = 128 - 55;
      else {
        controlX = 128 + (positionX * 100);
        controlY = minValue;
      }
    }
    if ((currentGame == Melee) && (isMODX || isMODY)) {
      if (!isLightShieldButtons) {
        if (!(isMODX && isMODY)) {
          isL = false;
          LLight = 80;
        }
      }

      if (DIAGONAL) {
        if (isMODX) {
          controlX = 128 + (positionX * 51);
          controlY = 128 + (positionY * 30);
        }
        if (isMODY) {
          controlX = 128 + (positionX * 40);
          controlY = 128 + (positionY * 68);
        }
      }
    }
  }

  if (isR) {
  RLight = 140;
  if (HORIZONTAL) {
      if (currentGame == Ultimate)
        controlX = 128 + (positionX * 51);
      if (currentGame == Melee)
        controlX = 128 + (positionX * 51);
      if (currentGame == PM)
        controlX = 128 + (positionX * 48);
    }
    if (VERTICAL) {
      if (currentGame == Ultimate)
        controlY = 128 + (positionY * 51);
      if (currentGame == Melee)
        controlY = 128 + (positionY * 43);
      if (currentGame == PM)
        controlY = 128 + (positionY * 48);
    }
    if (DIAGONAL) {
      if (currentGame == Melee) controlX = 128 + (positionX * 43);
      if (isMODX) {
        if (currentGame == Melee) {
          controlX = 128 + (positionX * 51);
          controlY = 128 + (positionY * 30);
        }
        if (currentGame == PM) {
          controlX = 128 + (positionX * 68);
          controlY = 128 + (positionY * 40);
        }
      }
      if (isMODY) {
        controlX = 128 + (positionX * 40);
        controlY = 128 + (positionY * 68);
      }
    }
  }

  /********* DPAD *********/
  if (isMODX && isMODY) {
  cstickX = 128;
  cstickY = 128;
  if (isCUP == 1) {
      isDPADUP = true;
    }
    else if (isCUP == 0) {
      isDPADUP = false;
    }

    if (isCDOWN == 1) {
      isDPADDOWN = true;
    }
    else if (isCDOWN == 0) {
      isDPADDOWN = false;
    }
    if (isCLEFT == 1) {
      isDPADLEFT = true;
    }
    else if (isCLEFT == 0) {
      isDPADLEFT = false;
    }
    if (isCRIGHT == 1) {
      isDPADRIGHT = true;
    }
    else if (isCRIGHT == 0) {
      isDPADRIGHT = false;
    }
  }
  /********* PC Dinput Setup *********/

  if (currentDevice == PC) {
  Joystick.setButton(0, isA);
    Joystick.setButton(1, isB);
    Joystick.setButton(2, isX);
    Joystick.setButton(3, isY);
    Joystick.setButton(4, isZ);
    Joystick.setButton(5, isL);
    Joystick.setButton(6, isR);
    Joystick.setButton(7, isSTART);
    Joystick.setButton(8, isDPADLEFT);
    Joystick.setButton(9, isDPADUP);
    Joystick.setButton(10, isDPADRIGHT);
    Joystick.setButton(11, isDPADDOWN);
    Joystick.setButton(12, isLIGHTSHIELD);
    Joystick.setButton(13, isEXTRA1); //Extra buttons should be added here too
    Joystick.setButton(14, isEXTRA2);

    Joystick.setXAxis(controlX);
    if (currentGame == Rivals)
  Joystick.setYAxis(((controlY-128) * -1) + 128); //This just uninverts the Y-axis for games like rivals
else
  Joystick.setYAxis(controlY);
  
    Joystick.setRxAxis(cstickX);
    if (currentGame == Rivals)
  Joystick.setRyAxis(((cstickY-128) * -1) + 128);
else
  Joystick.setRyAxis(cstickY);
    if (isLightShieldButtons)
      Joystick.setThrottle(RLight);
    else
      Joystick.setThrottle(LLight);
  }
  /********* GC Report *********/
  else {
    d.report.l = isL;
    d.report.start = isSTART;
    d.report.b = isB;
    d.report.x = isX;
    d.report.z = isZ;
    d.report.r = isR;
    d.report.y = isY;
    d.report.a = isA;
    d.report.dup = isDPADUP;
    d.report.ddown = isDPADDOWN;
    d.report.dleft = isDPADLEFT;
    d.report.dright = isDPADRIGHT;

    d.report.xAxis = controlX;
    d.report.yAxis = controlY;
    d.report.cxAxis = cstickX;
    d.report.cyAxis = cstickY;
    d.report.right = RLight;
    d.report.left = LLight;
    GamecubeConsole.write(d);
  }


}


uint8_t fTwoIPNoReactivate(bool isLOW, bool isHIGH, bool& wasLOW, bool& wasHIGH, bool& lockLOW, bool& lockHIGH) {
  uint8_t control = 128;
  if (isLOW && isHIGH) {
    if (wasHIGH) {
      control = minValue;
      lockHIGH = true;
    }
    if (wasLOW) {
      control = maxValue;
      lockLOW = true;
    }
  }
  if (!isLOW && isHIGH && (lockHIGH == false)) {
    control = maxValue;
    wasHIGH = true;
    wasLOW = false;
    lockLOW = false;
  }
  if (isLOW && !isHIGH && (lockLOW == false)) {
    control = minValue;
    wasLOW = true;
    wasHIGH = false;
    lockHIGH = false;
  }
  if (!isLOW && !isHIGH) {
    wasHIGH = false;
    wasLOW = false;
    lockLOW = false;
    lockHIGH = false;
  }
  return control;
}

uint8_t fTwoIP(bool isLOW, bool isHIGH, bool& wasLOW, bool& wasHIGH) {
  uint8_t control = 128;
  if (isLOW && wasHIGH)
    control = minValue;
  if (isHIGH && wasLOW)
    control = maxValue;
  if (!isLOW && isHIGH) {
    control = maxValue;
    wasHIGH = true;
    wasLOW = false;
  }
  if (isLOW && !isHIGH) {
    control = minValue;
    wasLOW = true;
    wasHIGH = false;
  }
  return control;
}

uint8_t fNeutral(bool isLOW, bool isHIGH) {
  uint8_t control = 128;
  if (!isLOW && isHIGH)
    control = maxValue;
  if (isLOW && !isHIGH)
    control = minValue;
  return control;
}
