/*
               MORSE ENDECODER
 
 Morse encoder / decoder classes for the Arduino.
 
 
 This example decodes Morse signals present on digital input 7.
 (active low, and then also using the internal pullup resistor)
 
 It also encodes Morse sent via the serial interface to the Arduino, on
 digital output pin 13.
 
 It can also decode audible signals, if using the constant MORSE_AUDIO
 instead of MORSE_KEYER, but then it is important to note that the
 input pin nr. will be for ANALOG inputs (0-5 on Atmega 168 - 328),
 and not the digital inputs.
 
 
 
 Copyright (C) 2010 raron
 
 GNU GPLv3 license:
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 
 
 Contact: raronzen@gmail.com  (not checked too often..)
 */


#include <MorseEnDecoder.h>


int morseInPin = 1;      
int morseOutPin = 0;
                //          1         2  
                //0123456789012345678901234 
char inBuff[25] =  {0};
char outBuff[25] = {0};
byte inPtr = 0;
byte outPtr = 0;

morseDecoder morseInput(morseInPin, MORSE_KEYER, MORSE_ACTIVE_LOW);
morseEncoder morseOutput(morseOutPin);

unsigned long NextUpdate = 0;
boolean Sending = true; 

void setup()
{ 
  pinMode(morseOutPin, OUTPUT);
  pinMode(morseInPin, INPUT);
  digitalWrite(morseOutPin, HIGH);
  delay(1000);
  digitalWrite(morseOutPin, LOW);
  // Setting Morse speed in wpm - words per minute
  // If not set, 13 wpm is default
  morseInput.setspeed(13);
  morseOutput.setspeed(13);
}



void loop()
{
  // Needs to call these once per loop
  morseInput.decode();
  morseOutput.encode();
  
  // morse output ---...--- 
  if (((long)millis() - (long)NextUpdate) > 0) // if current millis() time minus nestupdate time is bigger then 0
  {
    NextUpdate = millis() + 120000;        // nest update in 120 000 milliseconds from now
    strcpy(outBuff, "hello world hello yo/////");     // a NEW message
    outPtr = 0;                            // outPtr reset to 0
    Sending = true;                        // ready to send
  }

  if (morseOutput.available() && Sending == true)  // could we send? and do we have something to send?
  {
    morseOutput.write(outBuff[outPtr]);            // send the Character Nr outPtr
    if (outBuff[outPtr] == '/') Sending = false;   // if last character "/" we are DONE 
    outPtr++;                                      // increase the outPtr by 1
  }
  // morse output +++ STOP +++ 
  

  // If a character is decoded from the input,
  if (morseInput.available())                 // do we have a morse character?
  {
    char receivedMorse = morseInput.read();   // read thius character
    inBuff[inPtr] = receivedMorse;             // add it to our in Buffer at position inPtr
    if (receivedMorse == '/')                 // is this character a '/'?
    {      
      parseInput();                           // parse the inBuff (do something with this message)
      inPtr = 0;                              // reset set the inPtr to the beginning of inBuff
    }
    else 
    {
      inPtr++;                                // increase inBuff by 1
    }
    if (inPtr == 24)                          // if inBuff == 24 we didnt receive a '/' yet)
    {
      inPtr = 0;                              // reset it to 0 (throw the inBuff just away)
    }
  }
}


void parseInput() {
  // compare inBuff with some messgaes here
  char On[] = "ON";
  int onPtr = 2;
  char Off[] = "OFF";
  int offPtr = 3;
  
  
}



