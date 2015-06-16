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

#include "InputDaemon.h"

void InputDaemon::setup()
{
	Serial.println(F("Starting InputDaemon...7"));
}

void InputDaemon::_run()
{
	_bufferIndex = 0;
	while (Serial.available() > 0 && _bufferIndex < 8)
	{
		byte input = Serial.read();
		if ( //this is wrong, but currently is the only right method
			input == 'w' ||	//up
			input == 'a' ||	//left
			input == 's' ||	//down
			input == 'd' ||	//right
			input == 'q' || //back
			input == 'e' )  //ok
		_inputBuffer[_bufferIndex++] = input;
	}
	
	if (_bufferIndex > 0) // at least one input received
	{
		_bufferIndex = 0;
		while (_bufferIndex < 6)
		{
			pushMessageData(_inputBuffer[_bufferIndex]);
			_bufferIndex++;
		}
			
		sendMessage(MENU_D);
		clearMessageData();
	}
}

void InputDaemon::_execute(const Message& msg)
{

}