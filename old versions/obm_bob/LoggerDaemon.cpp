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
	
	
	//SdFat SD;
	if (!SD.begin(CS_PIN)) Serial.println(F("Error setting up the SD comm!"));
	else 
	{
		Serial.println(F("SD connected."));	
		/*
		The SD card is structured as following:
		-root
		|----logs
			|----YYYYMMDD
				|----hhmmss.log
				|----...
				|----...
		
		.ind files track the start time of each log, while .log files keep trackof the data
		
		Example .ind file:
		
		Example .log file (00.log):
				  11111111112
		012345678901234567890
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
	
	
	memset(filename, '\0', MAX_FILENAME);
	memset(rowBuffer, '\0', MAX_ROW_LENGTH);
	
	_state = SESSION_START;
	_dateRequest = 1;
	
	_DD = _MM = _YY = 0;
	_tempInt = _tempDec = _humInt = _humDec = 0;
	
	_logTimer = 0;
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
	
	//SdFat SD;
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
				I must use a manual integer conversion, hoping that integer division doesn't fuck me.
				*/
				
				/*
				//I hate myself for this and I want to die
				//all in the name of heap purity
				//in case this wasn't clear: NO DYNAMIC ALLOCATION = REALLY LOW CHANCE OF SYSTEM CRASH DUE TO STACK CRASH
				
				filename[0] = '/';
				filename[1] = 'l';
				filename[2] = 'o';
				filename[3] = 'g';
				filename[4] = 's';
				filename[5] = '/';
				filename[6] = '2';
				filename[7] = '0';
				filename[8] = (_YY / 10) + 48;
				filename[9] = (_YY - (filename[8]-48) * 10) + 48;
				filename[10] = (_MM / 10) + 48;
				filename[11] = (_MM - (filename[10]-48) * 10) + 48;
				filename[12] = (_DD / 10) + 48;
				filename[13] = (_DD - (filename[12]-48) * 10) + 48;
				filename[14] = '\0';
				//AAARGH!!!!! WHAT IS THIS BLASPHEMY!!!
				*/
				
				strcpy(filename, "/logs/20");
							
				char date_str[8];
				date_str[0] = (_YY / 10) + 48;
				date_str[1] = (_YY - (date_str[0]-48) * 10) + 48;
				date_str[2] = (_MM / 10) + 48;
				date_str[3] = (_MM - (date_str[2]-48) * 10) + 48;
				date_str[4] = (_DD / 10) + 48;
				date_str[5] = (_DD - (date_str[4]-48) * 10) + 48;
				date_str[6] = '/';
				date_str[7] = '\0';
				
				strcat(filename, date_str);			
				
				//if (!_isOpened) _isOpened = SD.begin(CS_PIN);
				//if (_isOpened) //if the SD stream has been successfully opened
				{
					if (!SD.exists(filename)) SD.mkdir(filename); //if the folder doesn't exist, create it
					
					//parse the current time
					char time_str[8];
					time_str[0] = (_hh / 10) + 48;
					time_str[1] = (_hh - (time_str[0]-48) * 10) + 48;
					time_str[2] = (_mm / 10) + 48;
					time_str[3] = (_mm - (time_str[2]-48) * 10) + 48;
					time_str[4] = (_ss / 10) + 48;
					time_str[5] = (_ss - (time_str[4]-48) * 10) + 48;
					time_str[6] = '.';
					time_str[7] = '\0';
					
					strcat(filename, time_str);
					strcat(filename, "log");
					
					//-----------------DEBUG
					//Serial.println(filename);
					//delay(1000);
					//----------------------
					
					logFile = SD.open(filename, FILE_WRITE);
					logFile.close();
					_state = SESSION_LOG;
				}
			}
			break;
		case SESSION_LOG:
			//Serial.println(F("Logger status: LOG"));
			/*
			5 minutes = 5 * 60 seconds = 300 * 1000 ms
			*/
			#define LOG_TIMER 300000
			if (millis() - _logTimer >= LOG_TIMER)
			{
				//log a row in the log file
				logFile = SD.open(filename, FILE_WRITE);
				logFile.print(_tempInt, DEC); logFile.print("."); logFile.print(_tempDec, DEC); logFile.print(",");
				logFile.print(_humInt, DEC); logFile.print("."); logFile.print(_humDec, DEC); logFile.print(",");
				logFile.print(" N"); logFile.print("\\"); logFile.print("A"); logFile.print(",");
				logFile.print(" N"); logFile.print("\\"); logFile.print("A"); logFile.print("#"); logFile.println();
				logFile.close();
				_logTimer += LOG_TIMER;
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
		case SENSORS_D:
			_tempInt = msg.data[0];
			_tempDec = msg.data[1];
			_humInt = msg.data[2];
			_humDec = msg.data[3];
			break;
	}
	
}