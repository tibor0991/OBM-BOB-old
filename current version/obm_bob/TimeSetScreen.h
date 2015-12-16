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

#define _dd _dateTime[0]
#define _MM _dateTime[1]
#define	_yy _dateTime[2]
		
#define	_hh _dateTime[3]
#define	_mm _dateTime[4]
#define	_ss _dateTime[5]

const uint8_t daysInMonth [] PROGMEM = { 31,28,31,30,31,30,31,31,30,31,30,31 }; //thanks rtclib

#define UPPERLINE_CHAR (char)0
byte upperLine[8] = 
{
	B11111,
	B11011,
	B10001,
	B10001,
	B00000,
	B00000,
	B00000,
	B00000
};

class TimeSetScreen : public Screen
{
public:
		TimeSetScreen() : Screen() {}
		virtual void handleInput(byte button)
		{
			switch(button)
			{
				case UP_BUTTON:
					if (_index < 3)
						_lowerBound = 1;
					else _lowerBound = 0;
					switch(_index)
					{
						case 0:
							_upperBound = pgm_read_byte(daysInMonth + _MM - 1);
							break;
						case 1:
							_upperBound = 12;
							break;
						case 2:
							_upperBound = 254;
							break;
					}
					if (_dateTime[_index] < _upperBound) _dateTime[_index]++;
					else _dateTime[_index] = _lowerBound;
					break;
				case DOWN_BUTTON:
					if (_index < 3)
						_lowerBound = 1;
					else _lowerBound = 0;
					switch(_index)
					{
						case 0:
							_upperBound = pgm_read_byte(daysInMonth + _MM - 1);
							break;
						case 1:
							_upperBound = 12;
							break;
						case 2:
							_upperBound = 254;
							break;
					}
					if (_dateTime[_index] > _lowerBound) _dateTime[_index]--;
					else _dateTime[_index] = _upperBound;
					break;
				case LEFT_BUTTON:
					if (_index > 0)
					{
						lcd.setCursor(_ulSize[_index], 2);
						for (byte i=0; i<_ulSize[_index+1] - _ulSize[_index]; i++) lcd.print(' ');
						_index--;
						lcd.setCursor(_ulSize[_index], 2);
						for (byte i=0; i<_ulSize[_index+1] - _ulSize[_index] - 1; i++) lcd.print(UPPERLINE_CHAR);
					}
					break;
				case RIGHT_BUTTON:
					if (_index < 5)
					{			
						/*
						0  1  2    3  4  5
						__/__/____ __:__:__
						012345678901234567890
						000000000011111111111
						
						0 - 0
						1 - 3
						2 - 6
						3 - 11
						4 - 14
						5 - 17
						*/
						lcd.setCursor(_ulSize[_index], 2);
						for (byte i=0; i<_ulSize[_index+1] - _ulSize[_index]; i++) lcd.print(' ');
						_index++;
						lcd.setCursor(_ulSize[_index], 2);
						for (byte i=0; i<_ulSize[_index+1] - _ulSize[_index] - 1; i++) lcd.print(UPPERLINE_CHAR);
					}
					break;
				case BACK_BUTTON:
					_nextState = MENU;
					break;
				case ENTER_BUTTON:
					FrontController::getClockManager().setDateTime(_dd, _MM, _yy, _hh, _mm, _ss);
					_nextState = MENU;
					break;
			}
		}
		virtual void setupScreen()
		{	
			lcd.createChar(UPPERLINE_CHAR, upperLine);
			
			lcd.clear();
			
			lcd.home();
			
			lcd.print("Set Time:");
			
			DateTime current = FrontController::getClockManager().getCurrent();
			_dateTime[0] = current.day();
			_dateTime[1] = current.month();
			_dateTime[2] = current.year() - 2000;
			_dateTime[3] = current.hour();
			_dateTime[4] = current.minute();
			_dateTime[5] = current.second();
						
			lcd.setCursor(0,1);
			char dateBuffer[20];
			memset(dateBuffer, '\0', 20);
			sprintf(dateBuffer, "%02u/%02u/%04u %02u:%02u:%02u", _dd, _MM, _yy+2000, _hh, _mm, _ss);
			lcd.print(dateBuffer);
			
			_index = 0;
			
			lcd.setCursor(0,2);
			lcd.print(UPPERLINE_CHAR);
			lcd.print(UPPERLINE_CHAR);
			
			
			
			_nextState = THIS;
		}
		
		virtual void update()
		{
			lcd.setCursor(0, 1);
			char dateBuffer[20];
			memset(dateBuffer, '\0', 20);
			sprintf(dateBuffer, "%02u/%02u/%04u %02u:%02u:%02u", _dd, _MM, _yy+2000, _hh, _mm, _ss);
			lcd.print(dateBuffer);
		}
	private:
		byte _index;
		byte _upperBound, _lowerBound;
				
		byte _ulSize[7] = {0,3,6,11,14,17,20};
		byte _dateTime[6];
};