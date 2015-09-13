#include <Arduino.h>
#include <Wire.h>
#include <RTClib.h>
#include <DHT.h>
#include <SPI.h>
#include <SdFat.h>
#include <string.h>

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
File logFile;
unsigned long int logger_timer = 0;
#define CS_PIN 10
#define MAX_FILENAME 32
#define MAX_ROW_LENGTH 20
char file_path[MAX_FILENAME];
char row_buffer[MAX_ROW_LENGTH];
#define LOGGING_INTERVAL 300000


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
byte pressed_button = 0;
#define ENTER_BUTTON 	0x00000001
#define DOWN_BUTTON  	0x00000010
#define LEFT_BUTTON   	0x00000100
#define UP_BUTTON     	0x00001000
#define RIGHT_BUTTON  	0x00010000
#define BACK_BUTTON	 	0x00100000
#define RESET_BUTTON  	0x01000000
#define ERROR			0x11111111

//display
bool data_changed = false;
enum DisplayState {IDLE, MENU, DATE_SETUP};
DisplayState d_state = IDLE;

void setup()
{
	Serial.begin(9600);
	Serial.println("Starting BOB...");
	
	Wire.begin();
	
	//setting up the RTC
	rtc.begin();
	//if (! rtc.isrunning()) rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
	
	//setting up DHT
	/*
	dht_sensors[0] = &dht1;
	dht_sensors[1] = &dht2;
	dht_sensors[2] = &dht3;
	*/
	
	dht1.begin();
	dht2.begin();
	dht3.begin();
	
	//setting up the SD card reader
	pinMode(CS_PIN, OUTPUT);
	logger.begin(CS_PIN);
	DateTime now = rtc.now();
	memset(file_path, '\0', MAX_FILENAME);
	memset(row_buffer, '\0', MAX_ROW_LENGTH);
	if (!logger.exists("logs") logger.mkdir("logs");
	sprintf(file_path, "/logs/%02d%02d%d", now.year(), now.month(), now.day());
	if (!logger.exists(file_path)) logger.mkdir(file_path);
	sprintf(file_path, "/logs/%02d%02d%d/%02d%02%02.log", now.year(), now.month(), now.day(), now.hour(), now.minute(), now.second());
	logFile = logger.open(file_path, FILE_WRITE);
	logFile.close();
	
	
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
			pressed_button = 0;
			break;
		case ENTER_VALUE:
			pressed_button = ENTER_BUTTON;
			break;
		case DOWN_VALUE:
			pressed_button = DOWN_BUTTON;
			break;
		case LEFT_VALUE:
			pressed_button = LEFT_BUTTON;
			break;
		case UP_VALUE:
			pressed_button = UP_BUTTON;
			break;
		case RIGHT_VALUE:
			pressed_button = RIGHT_BUTTON;
			break;
		case BACK_VALUE:
			pressed_button = BACK_BUTTON;
			break;
		case RESET_VALUE:
			pressed_button = RESET_BUTTON;
			break;
		default:
			pressed_button = ERROR;
			break;
	}
	
	
	
	//display
	
	switch(d_state)
	{
		case IDLE:
			if (pressed_button != ENTER_BUTTON)
			{
				char display_line[20];
				Serial.write(128);
				DateTime now = rtc.now();
				/*
				//Serial.write(12); //clears the screen
				//	                       1111111111
				//				 01234567890123456789
				//print the date
				
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
				Serial.print(now.second(), DEC);*/
				memset(display_line, '\0', 20);
				sprintf(display_line, "%02d/%02d/%d %02d:%02d:%02d\n", now.day(), now.month(), now.year(), now.hour(), now.minute(), now.second());
				Serial.print(display_line);
				
				if (data_changed)
				{
					Serial.write(148);/*
					//	                    1111111111
					//			  01234567890123456789
					//            Temperature: ##.# °C
					Serial.print(F("Temperature:"));
					Serial.print(temp_avg, DEC);
					 Serial.print(" C");
					*/
					memset(display_line, '\0', 20);
					sprintf(display_line, "Temperature:%02.2f C\n", temp_avg);
					
					//prints the humidity
					Serial.write(168);/*
					//	                    1111111111
					//			  01234567890123456789
					//            Humidity: ##.#%
					Serial.print(F("Humidity:"));
					Serial.print(hum_avg, DEC);
					Serial.print("%");
					*/
					memset(display_line, '\0', 20);
					sprintf(display_line, "Humidity:%02.2f C\n", hum_avg);
					
					
					data_changed = false;
				}
			}
			else d_state = MENU;
			break;
		case MENU:
			switch(pressed_button)
			{
				case BACK_BUTTON:
					d_state = IDLE;
					break;
				default:
					Serial.println("There will be a menu");
					break;
			}
			break;
	}

	//logging
	if (millis() > logger_timer)
	{
		logFile = logger.open(file_path, FILE_WRITE);
		memset(row_buffer, '\0', MAX_ROW_LENGTH);
		sprintf(row_buffer, "%.2f,%.2f\n", temp_avg, hum_avg);
		logFile.println(row_buffer);
		logFile.close();
		logger_timer += LOGGING_INTERVAL;
	}
	
	pressed_button = 0;
}