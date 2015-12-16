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
#include "IdleScreen.h"
#include "MenuScreen.h"
#include "TimeSetScreen.h"
#include "AlarmScreen.h"
#define MAX_SCREENS 4

class ScreenManager
{
	public:
		void setup()
		{
			Screen::initDisplay(20,4);
			addScreen(&idle, 0);
			addScreen(&menu, 1);
			addScreen(&timeset, 2);
			addScreen(&alarm, 3);
		}
		void setScreen(byte index)
		{
			_currentScr = _screens[index];
			_currentScr->setupScreen();
		}
		void sendButton(byte button)
		{
			_currentScr->handleInput(button);
		}
		void addScreen(Screen* newScreen, byte screen_ID)
		{
			_screens[screen_ID] = newScreen;
		}
		void update()
		{
			byte nextState = _currentScr->getNextState();
			
			if (nextState != THIS)
			{
				Serial.print("Next state:"); Serial.println(nextState);
				setScreen(nextState);
			}
			else 
			{
				_currentScr->update();
			}
		}
	private:
		Screen* _screens[MAX_SCREENS];
		Screen* _currentScr;
		MenuScreen menu;
		IdleScreen idle;
		TimeSetScreen timeset;
		AlarmScreen alarm;
		
};