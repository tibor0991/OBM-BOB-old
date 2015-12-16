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