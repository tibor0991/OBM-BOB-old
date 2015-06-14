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

#include "MenuDaemon.h"

SoftwareSerial MenuDaemon::display_s(255, DISPLAY_PIN);

void MenuDaemon::setup()
{
	Serial.println("Starting InputDaemon...");
	
	//setting up the software serial for the parallax display
	pinMode(DISPLAY_PIN, OUTPUT);
	digitalWrite(DISPLAY_PIN, HIGH);
	display_s.begin(9600);
	delay(100);
	display_s.write(12);
	display_s.write(17);
	delay(5);
	
	_dataChanged = 0;
	_currentState = IDLE_SCREEN1;
	_rollTime = 0;
}

void MenuDaemon::_run()
{
	/*
	Idle screen example:
	           111111
	 0123456789012345
	|TEMP: 36.5      |
	|HUM: 80%        |
	*/

	switch (_currentState)
	{
		case IDLE_SCREEN1:
			//prints the first idle page
			
			//printing to USB serial 
			Serial.print("TEMP: ");
			Serial.print(_tempInt, DEC);
			Serial.print(".");
			Serial.print(_tempDec, DEC);
			Serial.println();
			Serial.print("HUM : ");
			Serial.print(_humInt, DEC);
			Serial.print(".");
			Serial.print(_humDec, DEC);
			Serial.println();
			
			//change the data on the display ONLY IF a change in data happened
			if (_dataChanged)
			{
				//	                       111111
				//				 0123456789012345
				display_s.print(" DATE GOES HERE ");
				display_s.write(13);
				//	                       111111
				//				 0123456789012345
				//should print  "T:36.5 -- H:60.2" for example
				display_s.print("T:");
				display_s.print(_tempInt, DEC);
				display_s.print(".");
				display_s.print(_tempDec, DEC);
				
				display_s.print("--");

				display_s.print("H:");
				display_s.print(_humInt, DEC);
				display_s.print(".");
				display_s.print(_humDec, DEC);
				
				_dataChanged = 0;
			}
			
			
			if (millis() - _rollTime >= IDLE_ROLL_TIME)
			{
				_rollTime+= IDLE_ROLL_TIME;
				_currentState = IDLE_SCREEN2;
			}			
			break;
		case IDLE_SCREEN2:
			//prints data on the second idle page

			if (millis() - _rollTime >= IDLE_ROLL_TIME)
			{
				_rollTime+= IDLE_ROLL_TIME;
				_currentState = IDLE_SCREEN1;
			}
			break;
	}
}

void MenuDaemon::_execute(const Message& msg)
{
	switch(msg.senderID)
	{
		case SENSORS_D:
			Serial.println("Received a message from SENSORS_D");
			_tempInt = msg.data[0];
			_tempDec = msg.data[1];
			_humInt = msg.data[2];
			_humDec = msg.data[3];
			_dataChanged = 1;
			break;
		case INPUT_D:
			//it just prints what input it gets right now, it will be changed soon
			Serial.print("Received a message from INPUT_D :");
			for (byte i=0; i<MESSAGE_SIZE; i++)
			{
				Serial.print("[");
				Serial.write(msg.data[i]);
				Serial.print("] ");
			}
			Serial.println();
			break;
	}
}