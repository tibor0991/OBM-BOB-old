#include <Arduino.h>
#include "Device.h"
#include <DHT.h>

#ifndef DHT_DEVICE_H
#define DHT_DEVICE_H

struct DHT_Values
{
	float temp;
	float hum;
};

class DHT_Device : public Device , private DHT
{
	public:
		DHT_Device(byte pin, byte type) : DHT(pin, type) {}
		virtual bool connect();
		virtual bool isConnected();
		virtual void disconnect();
		virtual void readDevice(void* dest);
		virtual void writeDevice(void* src);
	private:
};

#endif