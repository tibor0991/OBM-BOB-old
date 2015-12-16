/*
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

#pragma once
#include <Arduino.h>
#include <RTClib.h>

class ClockManager
{
	public:
		void setup();
		DateTime getCurrent(); 
		void setDateTime(byte day, byte month, byte year, byte hour, byte minute, byte second)
		{
			DateTime newTime(year, month, day, hour, minute, second);
			_rtc.adjust(newTime);
		}
	private:
		RTC_DS1307 _rtc;
}; 

void ClockManager::setup()
{
	_rtc.begin();
	if ( !_rtc.isrunning()) 
		_rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
}

DateTime ClockManager::getCurrent()
{
	return _rtc.now();
}
