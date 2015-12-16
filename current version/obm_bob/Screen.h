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

#pragma once

#include <Arduino.h>
#include "FrontController.h"
#define THIS 255
#define IDLE 0
#define MENU 1
#define TIME_SETTING 2
#define WARNING 3

class Screen
{
	public:	
		Screen() : _nextState(THIS) {}
		virtual void handleInput(byte button) = 0;
		virtual void setupScreen() = 0;
		virtual byte getNextState() { return _nextState; }
		static void initDisplay(byte w, byte h)
		{
			lcd.begin(w,h);
			lcd.clear();
		}
		virtual void update() = 0;
	protected:
		static LiquidCrystal_I2C lcd;
		byte _nextState;
		
};

LiquidCrystal_I2C Screen::lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);