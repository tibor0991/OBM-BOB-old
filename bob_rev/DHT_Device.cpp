#include "DHT_Device.h"

bool DHT_Device::connect()
{
	begin();
	if (read()) _isConnected = true;
	else _isConnected = false;
	return _isConnected;
}

bool DHT_Device::isConnected()
{
	
}

void DHT_Device::disconnect()
{
	
}
void DHT_Device::readDevice(void* dest)
{
	//treat _dest_ as a DHT_value struct
	((DHT_Values*)dest)->temp = readTemperature();
	((DHT_Values*)dest)->hum = readHumidity();
}
void DHT_Device::writeDevice(void* src)
{
	
}