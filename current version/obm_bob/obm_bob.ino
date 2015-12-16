/*
Copyright (C) 2015 - Gianfranco Damato

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
*/

#include <DHT.h>
#include <Wire.h>
#define SERIAL_LCD //comment this line to use the I2C display
#ifdef SERIAL_LCD
	#include "LiquidCrystal_Serial.h
#else
	#include <LiquidCrystal_I2C.h>
#endif
#include <RTClib.h>
#include "InputHandler.h"
#include "ScreenManager.h"

InputHandler input(A0);
ScreenManager screenMgr;


void setup() {
  // put your setup code here, to run once:
  pinMode(A0, INPUT);
  Serial.begin(9600);  
  Wire.begin();
  FrontController::setup();
  screenMgr.setup();
  screenMgr.setScreen(3);
  
}

void loop() {
  // put your main code here, to run repeatedly:
    
  byte button = input.acquireInput();
  screenMgr.sendButton(button);
  screenMgr.update();
  FrontController::update();
  //alarm check (must be done outside the controller)
  //if (alarm just triggered) set alarm screen
}
