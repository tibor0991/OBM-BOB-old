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

#pragma once
#include <Arduino.h>
#include <DHT.h>
#include "ErrorHandler.h"

#define MAX_DHT 2

DHT dht1(2, DHT22);
DHT dht2(3, DHT22);

class HySwitch	//hysteretic switch
{
	public:
		HySwitch(float setPoint, byte tolerance) : 
			_setPoint(setPoint), _tolerance(tolerance)
		{ }
		byte operator()(float input)
		{
			if (input < _setPoint - _tolerance) return 1;
			else if(input > _setPoint + _tolerance) return 0;
		}		
	private:
		float _setPoint;
		byte _tolerance;
};


class SensorManager 
{
	public:
		float getTemperature();
		float getHumidity();
		void setup();
		void update();
	private:
		byte _sensorIndex = 0;
		DHT* _dht[2];
		void _updateDHT();
		float _temps[MAX_DHT], _hums[MAX_DHT];	

		//void _checkTemperature(
};

float SensorManager::getTemperature()
{
	float avg = 0;
	for (int i=0; i<MAX_DHT; i++)
		avg += _temps[i];
	avg /= MAX_DHT;
	return avg;
}

float SensorManager::getHumidity()
{
	float avg = 0;
	for (int i=0; i<MAX_DHT; i++)
		avg += _hums[i];
	avg /= MAX_DHT;
	return avg;
}

void SensorManager::setup()
{
	//blablabla
	_dht[0] = &dht1;
	_dht[1] = &dht2;
	for (int i = 0; i < MAX_DHT; i++)
	{
		_dht[i]->begin();
		_temps[i] = _dht[i]->readTemperature();
		_hums[i] = _dht[i]->readHumidity();
	}
}

void SensorManager::_updateDHT()
{
	_temps[_sensorIndex] = _dht[_sensorIndex]->readTemperature();
	_hums[_sensorIndex] = _dht[_sensorIndex]->readHumidity();
	_sensorIndex = (_sensorIndex++) % MAX_DHT;
}

void SensorManager::update()
{
	_updateDHT();
	//first step: check if the values are floating around the hysteresis point	
	
}