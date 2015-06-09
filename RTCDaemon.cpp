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
#include "RTCDaemon.h"

RTCDaemon::RTCDaemon()
{

}

void RTCDaemon::setup()
{
  Serial.println("RTCDaemon started");
	_rtc.begin();
	if (_rtc.isrunning()) 
          _rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
        else 
          Serial.println("ERROR: Unable to find a RTC device!");
}

void RTCDaemon::_run()
{
        if (_rtc.isrunning())
        {
                //for testing purposes, rtc_d sends the seconds number each 5 secs
        	if (millis() - previousTime >= 5000)
        	{
        		previousTime += 5000;
        		byte dataArray[MESSAGE_SIZE];
                        for (byte i=0; i<MESSAGE_SIZE; i++) dataArray[i] = 0;
        		dataArray[0] = _rtc.now().second();
                        //dataArray[0] = previousTime / 1000.0;
        		sendMessage(0, dataArray);
        	}
        	//it will only reply to time requests in the next release
        }
}

void RTCDaemon::_execute(const Message& msg)
{

}
