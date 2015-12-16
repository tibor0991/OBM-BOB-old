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

//input values
#define ENTER_VALUE  895
#define DOWN_VALUE   767
#define LEFT_VALUE   639
#define UP_VALUE     510
#define RIGHT_VALUE  382
#define BACK_VALUE	 254
#define RESET_VALUE  126
#define REST_VALUE   1

//button IDs
#define ENTER_BUTTON 	1
#define DOWN_BUTTON  	2
#define LEFT_BUTTON     4	
#define UP_BUTTON     	8
#define RIGHT_BUTTON  	16
#define BACK_BUTTON     32
#define RESET_BUTTON    64
#define REST_BUTTON	128

#define INPUT_TOL	 4
#define MAX_BUTTONS		8
#define LOCKDOWN_TIMER 500
#define INPUT_THRESHOLD 30

#include <Arduino.h>

struct Button
{
	byte ID;	//the button ID
	int value;	//the median input value assigned to this button
};


#define PASTER(x, y) x##y
#define ASSIGN_BUTTON(index, NAME) \
			_buttonList[index].ID = PASTER(NAME,_BUTTON); \
			_buttonList[index].value = PASTER(NAME,_VALUE);

class InputHandler
{
	public:
		InputHandler(byte pin);
		byte acquireInput();
                
	private:
		bool _checkButton(int inputValue, Button& button);
		bool _pressed;
		byte _currentButton;
		byte _inputPin;
		Button _buttonList[MAX_BUTTONS];
		unsigned long int _lockdown;
};


