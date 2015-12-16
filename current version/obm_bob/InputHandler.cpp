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

#include "InputHandler.h"

InputHandler::InputHandler(byte pin) : 
	_pressed(false), 
	_currentButton(REST_BUTTON),
	_inputPin(pin),
	_lockdown(0)
{
	pinMode(pin, INPUT);

	//assign button values and inputs
	ASSIGN_BUTTON(0, UP)
	ASSIGN_BUTTON(1, LEFT)
	ASSIGN_BUTTON(2, DOWN)
	ASSIGN_BUTTON(3, RIGHT)
	ASSIGN_BUTTON(4, ENTER)
	ASSIGN_BUTTON(5, BACK)
	ASSIGN_BUTTON(6, RESET)
	ASSIGN_BUTTON(7, REST)
}

byte InputHandler::acquireInput()
{
	_currentButton = REST_BUTTON;
	if (_pressed)
	{	
		if (millis() >= _lockdown) _pressed = false;
		
	}
	else
	{	
		int inputValue = analogRead(_inputPin);
		if (inputValue > INPUT_THRESHOLD)
		{
			//Serial.println(inputValue, DEC);
			_pressed = true;
			_lockdown = millis() + LOCKDOWN_TIMER;
			for (byte index = 0; index < MAX_BUTTONS; index++)
			{
				//Serial.print("Is it button ID: "); Serial.print(_buttonList[index].ID);Serial.println();
				if (_checkButton(inputValue, _buttonList[index]))
				{
					_currentButton = _buttonList[index].ID;
					//Serial.print("Button ID: "); Serial.print(_currentButton);Serial.println();
					break;
				}
			}
		}
	}
	return _currentButton;
}


bool InputHandler::_checkButton(int inputValue, Button& button)
{
	return ( abs(inputValue - button.value) <= INPUT_TOL );
}
