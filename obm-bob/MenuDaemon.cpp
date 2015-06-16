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

void MenuDaemon::setup()
{
	Serial.println(F("Starting MenuDaemon..."));
	
	Serial.write(17);
	delay(5);
	
	_dataChanged = 0;
	_currentState = IDLE_SCREEN1;
	_rollTime = 0;
}

void MenuDaemon::_run()
{

	//notes to self: 13 is next line, 12 is screen clear
	//128 + (20* line index) sends the cursor to a line

	/*
	Example of a display printout:
			  1111111111
	01234567890123456789
	14/06/2015 14:35:01
	Temperature: ##.# °C
	Humidity:    ##.# %	
	Fan In:30% Out:40%
	*/
	switch (_currentState)
	{
		case IDLE_SCREEN:
			//prints the idle page
			
			//change the data on the display ONLY IF a change in data happened
					
			if (_dataChanged)
			{

				Serial.write(12); //clears the screen
				//	                       1111111111
				//				 01234567890123456789
				//print the date
				Serial.print(_DD, DEC);
				Serial.print("/");
				Serial.print(_MM, DEC);
				Serial.print("/");
				Serial.print(_YY+2000, DEC);
				
				Serial.print(" ");
				
				//print the time
				Serial.print(_hh, DEC);
				Serial.print(":");
				Serial.print(_mm, DEC);
				Serial.print(":");
				Serial.print(_ss, DEC);
				
				//prints the temp
				Serial.write(13);
				//	                    1111111111
				//			  01234567890123456789
				//            Temperature: ##.# °C
				Serial.print(F("Temperature:"));
				Serial.print(_tempInt, DEC);
				/*Sorry   */   Serial.print(".");
				Serial.print(_tempDec, DEC);
				/*Bad indent*/   Serial.print(" °C");
				
				//prints the humidity
				Serial.write(13);
				//	                    1111111111
				//			  01234567890123456789
				//            Humidity: ##.#%
				Serial.print(F("Humidity:"));
				Serial.print(_humInt, DEC);
				/*Sorry   */Serial.print(".");
				Serial.print(_humDec, DEC);
				/*Bad indent*/Serial.print("%");
				
				_dataChanged = 0; //resets the flag
			}		
			break;
	}
	sendMessage(RTC_D);
	clearMessageData();
}

void MenuDaemon::_execute(const Message& msg)
{
	switch(msg.senderID)
	{
		case SENSORS_D:
			//Serial.println("Received a message from SENSORS_D");
			_tempInt = msg.data[0];
			_tempDec = msg.data[1];
			_humInt = msg.data[2];
			_humDec = msg.data[3];
			_dataChanged = 1;
			break;
		case INPUT_D:
			//it just prints what input it gets right now, it will be changed soon
			/*
			Serial.print("Received a message from INPUT_D :");
			for (byte i=0; i<MESSAGE_SIZE; i++)
			{
				Serial.print("[");
				Serial.write(msg.data[i]);
				Serial.print("] ");
			}
			Serial.println();
			*/
			break;
		case RTC_D:
			//parses time
			//[DD][MM][YY][HH][mm][ss]
			_DD = msg.data[0];
			_MM = msg.data[1];
			_YY = msg.data[2];
			_hh = msg.data[3];
			_mm = msg.data[4];
			_ss = msg.data[5];
			_dataChanged = 1;
			break;
	}
}
