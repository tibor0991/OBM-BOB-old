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

#ifndef INPUT_D_H
#define INPUT_D_H

#include "Daemon.h"

class InputDaemon : public Daemon
{
	public:
		InputDaemon(byte ID) : Daemon(ID){}
        virtual void setup();
	protected:
		virtual void _run();
        virtual void _execute(const Message& msg);
	private:
		byte _inputBuffer[8];
		byte _bufferIndex;
};

#endif  INPUT_D_H
