#ifndef DEVICEMAN_H
#define DEVICEMAN_H

#include <Arduino.h>
#include "Device.h"
#include "DHT_Device.h"
#include "SD_Device.h"

//list of devices ID
enum DEVICE_ID{ DHT1 = 0, 
				DHT2, 
				DHT3, 
				SD,
				//TH_PAD, 
				//KEYPAD, 
				//FAN1, 
				//FAN2, 
				DEV_COUNT };

//device list

static Device* device_list[DEV_COUNT];
static DHT_Device sensor1(2, DHT22);
static DHT_Device sensor2(3, DHT22);
static DHT_Device sensor3(4, DHT22);
static SD_Device sd_reader;

static Device* getDeviceByID(byte ID)
{
	return device_list[ID];
}

static void setupDevices()
{
	//link devices to the device list
	device_list[DHT1] = &sensor1;
	device_list[DHT2] = &sensor2;
	device_list[DHT3] = &sensor3;
	//device_list[SD] = &sd_reader;
	
	Serial.begin(9600);
	//connect each device
	for (byte i = 0; i < DEV_COUNT; i++)
	{
		if (!device_list[i]->connect())
		{
			Serial.print(F("Device "));	Serial.print(i, DEC); Serial.print(F(" failed to initialize.")); Serial.println();
		}
	}
}



#endif