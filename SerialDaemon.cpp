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

#include "SerialDaemon.h"

SerialDaemon::SerialDaemon()
{
	Serial.begin(9600);
	Serial.println("SerialDaemon started.");
}

void SerialDaemon::_run()
{

}

void SerialDaemon::_execute(const Message& msg)
{
	Serial.print("Message from ");
	Serial.print(msg.senderID, HEX);
	Serial.print(":");
	for (byte i=0; i<MESSAGE_SIZE; i++)
	{
		Serial.print(msg.data[i], HEX);
	}
	Serial.println();
}