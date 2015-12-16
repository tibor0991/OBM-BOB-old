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
#include "Screen.h"

class MenuScreen : public Screen
{
	public:
		MenuScreen() : Screen(), menu_index(1){}
		virtual void handleInput(byte button)
		{
			switch(button)
			{
				case UP_BUTTON:
					if (menu_index > 1)
					{
						lcd.setCursor(0, menu_index);
						lcd.print(" ");
						lcd.setCursor(0, --menu_index);
						lcd.print(">");
					}
					break;
				case DOWN_BUTTON:
					if (menu_index < 3)
					{
						lcd.setCursor(0, menu_index);
						lcd.print(" ");
						lcd.setCursor(0, ++menu_index);
						lcd.print(">");
					}
					break;
				case BACK_BUTTON:
					_nextState = IDLE;
					break;
				case ENTER_BUTTON:
					switch(menu_index)
					{
						case 1:
							_nextState = TIME_SETTING;
							break;
					}
					break;
			}
		}
		virtual void setupScreen()
		{	
			lcd.clear();
			
			lcd.home();
			lcd.print("Main Menu:");

			lcd.setCursor(0,1);
			lcd.print("> Date/time");

			lcd.setCursor(0,2);
			lcd.print("  Sensors");

			lcd.setCursor(0,3);
			lcd.print("  Data Logging");
			
			menu_index = 1;
			
			_nextState = THIS;
		}
		
		virtual void update()
		{
			
		}
	private:
		byte menu_index;
};