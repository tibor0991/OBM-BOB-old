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
#ifndef RTC_D_H
#define RTC_D_H

#include <Arduino.h>
#include <Wire.h>
#include <RTClib.h>
#include "Daemon.h"

class RTCDaemon : public Daemon
{
	public:
		RTCDaemon();
                virtual void setup();
	protected:
		virtual void _run();
                virtual void _execute(const Message& msg);
	private:
		RTC_DS1307 _rtc;
		unsigned long int previousTime = 0; 
                //if we use a standard int, it quickly overflows
};

#endif RTC_D_H
