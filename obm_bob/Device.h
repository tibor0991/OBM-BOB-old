/*
	Device.h
	
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

#ifndef _DEVICE
#define _DEVICE

typedef unsigned char byte;

class Device 
{
	public:
		virtual bool connect(byte pin) = 0;
		virtual bool isConnected() = 0;
		virtual void disconnect() = 0;
		virtual void read() = 0;
		virtual void write() = 0;
	protected:
		bool _isConnected;
};

#endif