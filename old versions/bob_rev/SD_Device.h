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

#ifndef SDDEVICE_H
#define SDDEVICE_H

#include "Device.h"
#include <Arduino.h>
#include <SPI.h>
#include <SdFat.h>

class SD_Device : public Device
{
	public:
		virtual bool connect();
		virtual bool isConnected();
		virtual void disconnect();
		virtual void readDevice(void* dest);
		virtual void writeDevice(void* src);
		
		//SdDevice unique functions
		void createFile(const char* filename);
		void writeLineToFile(const char* filename, const char* line);
	private:
		
		
		
};

#endif