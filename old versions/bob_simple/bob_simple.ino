#include <Arduino.h>
#include <Wire.h>
#include <RTClib.h>
#include <DHT.h>
#include <SPI.h>
#include <SdFat.h>
#include <string.h>

#include "InputHandler.h"


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
float temp_avg = 0.0f, hum_avg = 0.0f;
unsigned long int sensor_timer = 0;
#define SENSOR_INTERVAL 5000

//thermal pad
#define TH_PIN 5
#define TEMP_THR_LOW 35
#define TEMP_THR_HI 37

//alarms
byte alarm_flags = 0;
#define ALARM_REST 0
#define LOW_TEMP_ALARM 	1
#define HIGH_TEMP_ALARM 2
#define LOW_HUMI_ALARM 	4
#define HIGH_HUMI_ALARM 8
#define FAN_STUCK_ALARM 16
#define ALARM_WARNING 128
unsigned long alarm_timer = 0;
#define BUZZER_PIN A1
#define BUZZER_TONE 880 //A5

//RTC device
RTC_DS1307 rtc;

//fans
#define FAN_OUT_PWM 6
#define FAN_OUT_CHECK 7

#define FAN_IN_PWM 9
#define FAN_IN_CHECK 8

byte fan_out_speed = 255;
byte fan_in_speed = 255;



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
#define BACK_BUTTON     0x00100000
#define RESET_BUTTON  	0x01000000
#define ERROR			0x10000000

//display
bool data_changed = false;
enum DisplayState {IDLE, MENU, DATE_SETUP, THR_SETUP, LNG_SETUP, WARNING};
DisplayState d_state = IDLE;
#define USE_I2C_DISPLAY	//comment this if you want to use the serial output
#ifdef USE_I2C_DISPLAY
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
#else
#include <LiquidCrystal_Serial.h>
LiquidCrystal_Serial lcd;
#endif

//menu
byte menu_index = 1;


//main functions
void readSensors();
void fixValues();
void acquireInput();
void showMenu();
void logData();

void setup()
{
	Serial.begin(9600);
	Serial.println("Starting BOB...");
	
	Wire.begin();
	
	//setting up the RTC
	rtc.begin();
	if (! rtc.isrunning()) rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
	
	//setting up DHT
	dht1.begin();
	dht2.begin();
	dht3.begin();
	
	//setting up the pad relay
	pinMode(TH_PIN, OUTPUT);
	
	//setting up the fans
	pinMode(FAN_OUT_CHECK, INPUT);
	pinMode(FAN_OUT_PWM, OUTPUT);
	analogWrite(FAN_OUT_PWM, fan_out_speed);
	
	pinMode(FAN_IN_CHECK, INPUT);
	pinMode(FAN_IN_PWM, OUTPUT);
	analogWrite(FAN_IN_PWM, fan_in_speed);
	
	//setting up the buzzer pin
	pinMode(A1, OUTPUT);
	
	//setting up the SD card reader
	pinMode(CS_PIN, OUTPUT);
	logger.begin(CS_PIN);
	DateTime now = rtc.now();
	memset(file_path, '\0', MAX_FILENAME);
	memset(row_buffer, '\0', MAX_ROW_LENGTH);
	if (!logger.exists("logs")) logger.mkdir("logs");
	sprintf(file_path, "/logs/%02d%02d%d", now.year(), now.month(), now.day());
	if (!logger.exists(file_path)) logger.mkdir(file_path);
	sprintf(file_path, "/logs/%02d%02d%d/%02d%02%02.log", now.year(), now.month(), now.day(), now.hour(), now.minute(), now.second());
	logFile = logger.open(file_path, FILE_WRITE);
	logFile.close();
	
	//setting up the display
	lcd.begin(20,4);
	lcd.home();
	lcd.print(F("  Open Bio Medical  "));
    lcd.setCursor(0,1);
    lcd.print(F("    Baby On Board   "));
	delay(1000);
	lcd.clear();
}

void loop()
{
	//read sensors
	if (millis() > sensor_timer)
	{
		//first try: naive average, might take around 750 ms
		temp_avg = dht1.readTemperature() + dht2.readTemperature() + dht3.readTemperature();
		temp_avg /= 3.0f;
		
		hum_avg = dht1.readHumidity() + dht2.readHumidity() + dht3.readHumidity();
		hum_avg /= 3.0f;;
		
		sensor_timer += SENSOR_INTERVAL;
		data_changed = true;
	}
	
	
	//alarms
	if (alarm_flags != ALARM_REST && millis() > alarm_timer)  //the warning is still on after 5 seconds
	{
		//turn on the buzzer
		//digitalWrite(BUZZER_PIN, HIGH);
		tone(BUZZER_PIN, BUZZER_TONE);
		alarm_timer += 5000;
		d_state = WARNING;
		//emergency display purge	
		#ifdef USE_I2C_DISPLAY
		lcd.clear();
		lcd.setCursor(0,1);
		lcd.print(F(" WARNING == WARNING "));
		#endif
	} 
	else if (alarm_flags == ALARM_REST )
	{
		//turn off the alarm
		//digitalWrite(BUZZER_PIN, LOW);
		noTone(BUZZER_TONE);
	}
	
	
	
	
	//fix values
	if (temp_avg <= TEMP_THR_LOW) 
	{
		digitalWrite(TH_PIN, HIGH);
		alarm_flags |= ALARM_WARNING;
		
	}
	else if (temp_avg >= TEMP_THR_HI) 
	{
		digitalWrite(TH_PIN, LOW);
		alarm_flags |= ALARM_WARNING;
	}
	
	if (alarm_flags != ALARM_REST) 
		alarm_timer = millis() + 5000;
		
	//get inputs
	input_value = analogRead(A0);
	/*switch(input_value)
	{
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
	}*/

        if ( input_value > ENTER_VALUE - 5 && input_value < ENTER_VALUE + 5 ) pressed_button = ENTER_BUTTON;
        else if ( input_value > DOWN_VALUE - 5 && input_value < DOWN_VALUE + 5 ) pressed_button = DOWN_BUTTON;
        else if ( input_value > LEFT_VALUE - 5 && input_value < LEFT_VALUE + 5 ) pressed_button = LEFT_BUTTON;
        else if ( input_value > UP_VALUE - 5 && input_value < UP_VALUE + 5 ) pressed_button = UP_BUTTON;
        else if ( input_value > RIGHT_VALUE - 5 && input_value < RIGHT_VALUE + 5 ) pressed_button = RIGHT_BUTTON;
        else if ( input_value > BACK_VALUE - 5 && input_value < BACK_VALUE + 5 ) pressed_button = BACK_BUTTON;
        else if ( input_value > RESET_VALUE - 5 && input_value < RESET_VALUE + 5 ) pressed_button = RESET_BUTTON;
        else if ( input_value < RESET_VALUE -5 || input_value > ENTER_VALUE + 5) pressed_button = ERROR;

	delay(350);
        Serial.print("Input received:");
        Serial.println(input_value, DEC);
        Serial.println(pressed_button, DEC);
        if (pressed_button != ERROR && pressed_button != 0) 
        {
          tone(A1, 440);
          delay(25);
          noTone(A1);
        }
	//display
	/*
	switch(d_state)
	{
		case IDLE:
			if (pressed_button != ENTER_BUTTON)
			{
				char display_line[20];
				DateTime now = rtc.now();

				memset(display_line, '\0', 20);
				sprintf(display_line, "%02d/%02d/%d %02d:%02d:%02d", now.day(), now.month(), now.year(), now.hour(), now.minute(), now.second());
				
				#ifdef USE_I2C_DISPLAY
				lcd.setCursor(0,0);
				lcd.print(display_line);
				#endif
				
				if (data_changed)
				{
					//prints temperature
					memset(display_line, '\0', 20);
					sprintf(display_line, "Temperature:%02.2f C", temp_avg);
								
					#ifdef USE_I2C_DISPLAY
					lcd.setCursor(0,1);
					lcd.print(display_line);
					#endif
				
					//prints humidity
					memset(display_line, '\0', 20);
					sprintf(display_line, "Humidity:%02.2f C", hum_avg);
					
					#ifdef USE_I2C_DISPLAY
					lcd.setCursor(0,2);
					lcd.print(display_line);
					#endif
					
					data_changed = false;
				}
			}
			else if (pressed_button == ENTER_BUTTON)
			{
				d_state = MENU;
				#ifdef USE_I2C_DISPLAY
				lcd.clear();
				lcd.setCursor(0,0);
				lcd.print(F("Main menu:"));
				lcd.setCursor(0,1);
				lcd.print(F(">Set date"));
				lcd.setCursor(0,2);
				lcd.print(F(" Set thresholds"));
				lcd.setCursor(0,3);
				lcd.print(F(" Set language"));
				
				#endif
				menu_index = 1;
				
				01234567890123456789
				Main menu:
				>Set date
				 Set thresholds
				 Set language
				
			}
			break;
		case MENU:
			switch(pressed_button)
			{
				case BACK_BUTTON:
					d_state = IDLE;
                                        lcd.clear();
                                        break;
				case UP_BUTTON:
					//moves the index upward
					if ( menu_index > 1 )
					{
						lcd.setCursor(0,menu_index--);
						lcd.print(" ");
						lcd.setCursor(0, menu_index);
						lcd.print(">");
					}
					break;
				case DOWN_BUTTON:
					//moves the menu index downward
					if ( menu_index < 3 )
					{
						lcd.setCursor(0,menu_index++);
						lcd.print(" ");
						lcd.setCursor(0, menu_index);
						lcd.print(">");
					}
					break;
				case ENTER_BUTTON:
					if (menu_index == 1) d_state = DATE_SETUP;
					else if (menu_index == 2) d_state = THR_SETUP;
					else if (menu_index == 3) d_state = LNG_SETUP;
                                        lcd.clear();
					break;

			}
			break;
		case WARNING:
			//DUMMY STATE
			if (pressed_button == RESET_BUTTON)
			{
				d_state = IDLE;
				alarm_flags = ALARM_REST;
                                lcd.clear();
			}
			break;
		default:
			#ifdef USE_I2C_DISPLAY
			lcd.clear();
			lcd.home();
			lcd.print(F("Unimplemented state"));
			lcd.setCursor(0,2);
			lcd.print(F("Sorry :^)"));
			delay(1500);
			d_state = IDLE;
                        lcd.clear();
			#endif
			break;
	}
        */
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
