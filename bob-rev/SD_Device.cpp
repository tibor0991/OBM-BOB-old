/*
	SdDevice.cpp
	
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

#include "SD_Device.h"

#define CS_PIN 10

bool SD_Device::connect()
{
	pinMode(CS_PIN, OUTPUT);
	SdFat SD;
	_isConnected = SD.begin(CS_PIN);
	return (!_isConnected); //true if connection exists
}

bool SD_Device::isConnected()
{
	return _isConnected;
}

void SD_Device::disconnect()
{
	
}

void SD_Device::readDevice(void* data)
{
	
}

void SD_Device::writeDevice(void* data)
{
	
}

void SD_Device::createFile(const char* filename)
{
	File newFile;
	SdFat SD;
	SD.begin(CS_PIN);
	newFile = SD.open(filename, FILE_WRITE);
	newFile.close();
}

void SD_Device::writeLineToFile(const char* filename, const char* line)
{
	File newFile;
	SdFat SD;
	SD.begin(CS_PIN);
	newFile = SD.open(filename, FILE_WRITE);
	newFile.print(line);
	newFile.print(millis(), DEC);
	newFile.close();
}
