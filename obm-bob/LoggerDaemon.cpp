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

#include "LoggerDaemon.h"

/*
CSV string format:

[YYYY][MM][DD][hh][mm][ss][TEMP][HUM][FAN_IN][FAN_OUT][TERM]
Example:
2015,06,17,18,05,35,36.5,60.2,50.3,30.4#
01234567890123456789012345678901234567890
0		  1         2         3         4
Estimated size of a single line: 40 chars
40 chars * 1 byte = 40 bytes

This means that, with an average number of logs of 288 each day, a 24-h logfile weights about 288 * 40 = 11520 bytes = 11,25 kB
This raises a question? Can the amount of free RAM hold these 11520 bytes? 
I should do a static code analysis to find out how much RAM is left available by the OS.
*/

void LoggerDaemon::setup()
{
	Serial.println(F("Starting LoggerDaemon..."));
	pinMode(CS_PIN, OUTPUT);
	
	SdFat SD;
	if (!SD.begin(CS_PIN)) Serial.println(F("Error setting up the SD comm!"));
	else 
	{
		Serial.println(F("SD connected."));	
		/*
		The SD card is structured as following:
		-root
		|----logs
			|----YYYYMMDD
				|----YYYYMMDD.ind
				|----00.log
				|----01.log
		
		.ind files track the start time of each log, while .log files keep trackof the data
		
		Example .ind file:
		00,13:35:46
		01,16:56:45
		
		Example .log file (00.log):
		36.5,60.2,50.3,30.4#
		36.0,60.0,50.3,30.4#
		35.5,60.2,50.3,30.4#
		
		This means:
		-smaller log files
		-more robust system
		-session tracking
		
		NOTE TO SELF: write a PC-side log packer to pack all logs into a single log file, both for daily logs and total logs.
		*/
	}
	_state = SESSION_START;
	_dateRequest = 1;
}

void LoggerDaemon::_run()
{
	//if time spent since start/last save is more 5 minutes ( 5 * 60 * 1000 = 300000 ms)
	//or if requested from an external signal
	//save all data to the log file
	
	/*
		LOGGER STATES:
		-SESSION_START: the logger just started, needs to ask the RTC Daemon the current date/time, create a new folder if possible,
						create a new index file, 
	*/
	switch(_state)
	{
		case SESSION_START:
			if (_dateRequest) //just started, didn't even request the date
			{
				_dateRequest = 0;
				sendMessage(RTC_D);
				clearMessageData();
			} 
			else if (_dateReceived) //skip everything until you get a date/time
			{
				_dateReceived = 0;
				//blablabla
				/*
				What happens now?
				I don't want to use the String class from Arduino, since I'm pretty fucking sure it uses dynamic allocation,
				therefore I can't use class String's methods for converting integers to char arrays;
				and that's where the modulo operator comes in help.
				//PS : I love modulo.
				*/
				
			}
			break;
	}
	
}

void LoggerDaemon::_execute(const Message& msg)
{
	switch(msg.senderID)
	{
		case RTC_D:
			//parses time
			//[DD][MM][YY][HH][mm][ss]
			_DD = msg.data[0];
			_MM = msg.data[1];
			_YY = msg.data[2];
			_hh = msg.data[3];
			_mm = msg.data[4];
			_ss = msg.data[5];
			_dateReceived = 1;
			break;
	}
}