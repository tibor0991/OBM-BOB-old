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
#include "Screen.h"

byte warning_sign0[8] = 
{
	B00100,
	B01110,
	B11111,
	B11111,
	B11111,
	B11111,
	B11111,
	B11111
};

byte warning_sign1[8] =	
{
	B11111,
	B11111,
	B11111,
	B11111,
	B01110,
	B01110,
	B01110,
	B01110
};

byte warning_sign2[8] =
{
	B01110,
	B01110,
	B01110,
	B01110,
	B00100,
	B00100,
	B00100,
	B00100
};

byte warning_sign3[8] = 
{
	B00000,
	B00000,
	B01110,
	B11111,
	B11111,
	B01110,
	B00000,
	B00000
};

class AlarmScreen : public Screen
{
	public:
		AlarmScreen() : Screen(){}
		virtual void handleInput(byte button)
		{
			if (button == RESET_BUTTON)
			{
				noTone(A1);
				_nextState = IDLE;
			}
		}
		virtual void setupScreen()
		{	
			lcd.createChar(0, warning_sign0);
			lcd.createChar(1, warning_sign1);
			lcd.createChar(2, warning_sign2);
			lcd.createChar(3, warning_sign3);
		
			lcd.clear();		
			lcd.home();
			lcd.print((char)0);
			lcd.setCursor(0,1);
			lcd.print((char)1);
			lcd.setCursor(0,2);
			lcd.print((char)2);
			lcd.setCursor(0,3);
			lcd.print((char)3);
			
			lcd.setCursor(7, 0);
			lcd.print("WARNING");
			
			pinMode(A1, OUTPUT);
			tone(A1, 440);
			
			_nextState = THIS;
			
			
		}
		
		virtual void update()
		{
			
		}

	private:
};