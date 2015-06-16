/*
	OBM-BOB : Firmware for the Open Bio Medical "B.O.B." Project.
	
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

#ifndef MENU_D_H
#define MENU_D_H

#include "Daemon.h"

#define DISPLAY_PIN 1
#define IDLE_ROLL_TIME 2000

/*
Issue to fix: for now, the Parallax LCD is managed by the menu since the way 
things are shown on the display and the infos that will be shown are tightly coupled
*/

class MenuDaemon : public Daemon
{
	public:
		MenuDaemon(byte ID) : Daemon(ID){}
        virtual void setup();
	protected:
		virtual void _run();
        virtual void _execute(const Message& msg);
	private:
		enum MENU_STATES {IDLE_SCREEN = 0};
		MENU_STATES _currentState;
		
		//IDLE SCREEN vars
		byte _tempInt, _tempDec;
		byte _humInt, _humDec;
		unsigned long int _rollTime;
		byte _dataChanged;
		
		byte _DD, _MM, _YY;
		byte _hh, _mm, _ss;
};

#endif  MENU_D_H
