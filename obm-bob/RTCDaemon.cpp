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

RTC_DS1307 RTCDaemon::_rtc;
unsigned long int RTCDaemon::previousTime = 0;

void RTCDaemon::setup()
{
  Serial.println(F("RTCDaemon started"));
	_rtc.begin();
	if (_rtc.isrunning()) 
		_rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
	else 
		Serial.println(F("ERROR: Unable to find a RTC device!"));
}

void RTCDaemon::_run()
{

}

void RTCDaemon::_execute(const Message& msg)
{
	if (_rtc.isrunning())
	{
		//if it's running, process the message and send it back to the sender
		//the message is sent back to the requester in the format:
		//[DD][MM][YY][HH][mm][ss]
		DateTime now = _rtc.now();
		pushMessageData(now.day());
		pushMessageData(now.month());
		pushMessageData(now.year() - 2000);
		pushMessageData(now.hour());
		pushMessageData(now.minute());
		pushMessageData(now.second());
		sendMessage(msg.senderID);
		clearMessageData(); //always call this after sending a message
	}
}
