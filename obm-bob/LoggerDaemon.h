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

/*
LoggerDaemon.h: every 5 minutes saves an entry in the log file on a SD card.
The SD card is managed through a SD adapter mounted on the SPI bus.
*/

#include "Daemon.h"

#include <SPI.h>
#include <SdFat.h>
#include <string.h> //thanks C std lib for existing

#define CS_PIN 10
#define MAX_FILENAME 32


class LoggerDaemon : public Daemon
{
	public:
		LoggerDaemon(byte ID) : Daemon(ID){}
		void setup();
	protected:
		virtual void _run();
        virtual void _execute(const Message& msg);
	private:
		enum LoggerState {SESSION_START = 0, SESSION_LOG};
		LoggerState _state;
		byte _dateReceived, _dateRequest;
		byte _DD, _MM, _YY;
		byte _hh, _mm, _ss;
		char filename[MAX_FILENAME];
		bool _isOpened;
		
};