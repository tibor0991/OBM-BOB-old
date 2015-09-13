#include <Arduino.h>
#include <Wire.h>
#include <RTClib.h>
#include <DHT.h>
#include <SPI.h>
#include <SdFat.h>

/**
Single-file firmware for the OBM-BOB

TWF5IHRoZSBnb2RzIG9mIHByb2dyYW1taW5nIGhhdmUgbWVyY3kgb24gbXkgc291bC4=
*/


//variable declaration

//thermal/humidity sensors
#define DHT1_PIN 2
#define DHT2_PIN 3
#define DHT3_PIN 4
#define DHT_TYPE DHT22
DHT dht1(DHT1_PIN, DHT_TYPE);
DHT dht2(DHT2_PIN, DHT_TYPE);
DHT dht3(DHT3_PIN, DHT_TYPE);
DHT* dht_sensors[3];
byte dht_counter = 0;
float temp_avg = 0, hum_avg = 0;
unsigned long int sensor_timer = 0;
#define SENSOR_INTERVAL 5000

//RTC device
RTC_DS1307 rtc;


//SD logger
SdFat logger;
SdFile logFile;
unsigned long int logger_timer = 0;

//input
int input_value = 0;
#define REST_VALUE   0
#define ENTER_VALUE  895
#define DOWN_VALUE   767
#define LEFT_VALUE   639
#define UP_VALUE     510
#define RIGHT_VALUE  382
#define BACK_VALUE	 254
#define RESET_VALUE  126

//display
bool data_changed = false;

void setup()
{
	Serial.begin(9600);
	Serial.println("Starting BOB...");
	
	Wire.begin();
	
	//setting up the RTC
	rtc.begin();
	if (! rtc.isrunning()) rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
	
	//setting up DHT
	/*
	dht_sensors[0] = &dht1;
	dht_sensors[1] = &dht2;
	dht_sensors[2] = &dht3;
	*/
	
	dht1.begin();
	dht2.begin();
	dht3.begin();
	
	Serial.write(12); //clear screen
}

void loop()
{
	//read sensors
	if (millis() > sensor_timer)
	{
		//first try: naive average, might take around 750 ms
		temp_avg = dht1.readTemperature() + dht2.readTemperature() + dht3.readTemperature();
		temp_avg /= 3;
		
		hum_avg = dht1.readHumidity() + dht2.readHumidity() + dht3.readHumidity();
		hum_avg /= 3;
		
		sensor_timer += SENSOR_INTERVAL;
		data_changed = true;
	}
	
	//fix values
	
	
	//get inputs
	input_value = analogRead(A0);
	switch(input_value)
	{
		case REST_VALUE:
			Serial.println("No button pressed!");
			break;
		case ENTER_VALUE:
			Serial.println("ENTER pressed!");
			break;
		case DOWN_VALUE:
			Serial.println("DOWN pressed!");
			break;
		case LEFT_VALUE:
			Serial.println("LEFT pressed!");
			break;
		case UP_VALUE:
			Serial.println("UP pressed!");
			break;
		case RIGHT_VALUE:
			Serial.println("RIGHT pressed!");
			break;
		case BACK_VALUE:
			Serial.println("BACK pressed!");
			break;
		case RESET_VALUE:
			Serial.println("RESET pressed!");
			break;
		default:
			Serial.print("Unrecognized input! "); Serial.println(input_value, DEC);
			break;
	}
	
	
	
	//display
	Serial.write(128);
	//Serial.write(12); //clears the screen
	//	                       1111111111
	//				 01234567890123456789
	//print the date
	DateTime now = rtc.now();
	
	if (now.day() < 10) Serial.print(0, DEC);
	Serial.print(now.day(), DEC);
	Serial.print("/");
	if (now.month() < 10) Serial.print(0, DEC);
	Serial.print(now.month(), DEC);
	Serial.print("/");
	Serial.print(now.year(), DEC);
	
	Serial.print(" ");
	
	//print the time
	if (now.hour() < 10) Serial.print(0, DEC);
	Serial.print(now.hour(), DEC);
	Serial.print(":");
	if (now.minute() < 10) Serial.print(0, DEC);
	Serial.print(now.minute(), DEC);
	Serial.print(":");
	if (now.second() < 10) Serial.print(0, DEC);
	Serial.print(now.second(), DEC);
	
	if (data_changed)
	{
		Serial.write(148);
		//	                    1111111111
		//			  01234567890123456789
		//            Temperature: ##.# °C
		Serial.print(F("Temperature:"));
		Serial.print(temp_avg, DEC);
		/*Bad indent*/   Serial.print(" C");
		
		//prints the humidity
		Serial.write(168);
		//	                    1111111111
		//			  01234567890123456789
		//            Humidity: ##.#%
		Serial.print(F("Humidity:"));
		Serial.print(hum_avg, DEC);
		/*Bad indent*/Serial.print("%");
		
		data_changed = false;
	}


	
	
}