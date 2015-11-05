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

#include "SdDevice.h"

#define CS_PIN 10

bool SdDevice::connect(byte pin)
{
	pinMode(CS_PIN, OUTPUT);
	SdFat SD;
	_isConnected = SD.begin(CS_PIN);
	return (!_isConnected); //true if connection exists
}

bool SdDevice::isConnected()
{
	return _isConnected;
}

void SdDevice::disconnect()
{
	
}

void SdDevice::read()
{
	
}

void SdDevice::write()
{
	
}

void SdDevice::createFile(const char* filename)
{
	File newFile;
	SdFat SD;
	SD.begin(CS_PIN);
	newFile = SD.open(filename, FILE_WRITE);
	newFile.close();
}

void SdDevice::writeLineToFile(const char* filename, const char* line)
{
	File newFile;
	SdFat SD;
	SD.begin(CS_PIN);
	newFile = SD.open(filename, FILE_WRITE);
	newFile.print(line);
	newFile.print(millis(), DEC);
	newFile.close();
}
