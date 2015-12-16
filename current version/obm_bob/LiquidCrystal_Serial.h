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

#include <Arduino.h>
#include <Print.h>

class LiquidCrystal_Serial : public Print
{
	//commonly used functions
	public:
		void begin(int w, int h); //sets the internal variables and starts the serial interface
		void clear();
		void home();
		void setCursor(int x, int y);

	public: 
		virtual void write(uint8_t);	//reimplemented function from Print.h
	
	private:
		byte width, height;
		

};

void LiquidCrystal_Serial::begin(int w, int h)
{
	if (!Serial) Serial.begin(9600);
	width = w;
	height = h;
} 

void LiquidCrystal_Serial::clear()
{
	Serial.write(12);
}

void LiquidCrystal_Serial::home();
{
	Serial.write(128);
}

void LiquidCrystal_Serial::setCursor(int x, int y);
{
	byte index = y * width + x + 128;
	Serial.write(index);
}

void LiquidCrystal_Serial::write(uint8_t c)
{
	Seria.write(c);
}