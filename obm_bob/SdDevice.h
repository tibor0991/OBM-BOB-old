/*
	SdDevice.h
	
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

#ifndef _SDDEVICE
#define _SDDEVICE

#include "Device.h"
#include <Arduino.h>
#include <SPI.h>
#include <SdFat.h>

class SdDevice : public Device
{
	public:
		virtual bool connect(byte pin);
		virtual bool isConnected();
		virtual void disconnect();
		virtual void read();
		virtual void write();
		
		//SdDevice unique functions
		void createFile(const char* filename);
		void writeLineToFile(const char* filename, const char* line);
	private:
		SdFat bloat;
		
		
		
};

#endif