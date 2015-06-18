/*
	OBM-BOB : Firmware for the Open Bio Medical "B.O.B." Project.
	
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

#include "SensorsDaemon.h"


//sensors are connected to digital pins 2,3,4
DHT SensorsDaemon::sensor1(SENS1_PIN, DHT22);
DHT SensorsDaemon::sensor2(SENS2_PIN, DHT22);
DHT SensorsDaemon::sensor3(SENS3_PIN, DHT22);

void SensorsDaemon::setup()
{
	Serial.println(F("Starting SensorsDaemon..."));
	sensors[SENS1] = &sensor1;
	sensors[SENS2] = &sensor2;
	sensors[SENS3] = &sensor3;
		
	for (_currentSensor=0; _currentSensor<SENS_N; _currentSensor++) //i'm lazy
	{
		sensors[_currentSensor]->begin();
		if (sensors[_currentSensor]->read())
		{
		
		}
		else 
		{
			Serial.print(F("ERROR: Sensor not connected: "));
			Serial.println(_currentSensor+1, DEC);
		}
		
	}	
	_currentSensor = 0;
	_tempAvg = 0;
	_humAvg = 0;
	_previousTime = 0;
}

void SensorsDaemon::_run()
{
	/*
	Current problem: sensors take up to 250 ms for a single reading,
	-if we check all sensors in one loop, the system lags for 750 ms (which is pretty noticeable in a real-time system);
	-if we started a deferred check each 5 seconds (each 5 seconds, the next 3 loops read a single sensor) the lag for a single loop is reduced to 
		250ms, but the user would still feel lag overall;
	-we check only one sensor each 2 seconds, exploiting the thermal inertia of the system: this might be the best solution for a small number
		of sensors in a small container (like our incubator) and mitigates the lagging effect when operating the menus and such, since it fractures
		the 750 ms total lag over 6 seconds.
	*/
	//for now, I'll opt for the 3rd solution
	if (millis() - _previousTime >= SENSOR_INTERVAL)
	{
		_previousTime += SENSOR_INTERVAL;
		
		
		if(sensors[_currentSensor]->read())
		{
			//calculate temp sum
			_tempAvg += sensors[_currentSensor]->readTemperature();
		
			//calculate hum sum
			_humAvg += sensors[_currentSensor]->readHumidity();	
		}
		else 
		{
			Serial.print(F("Error reading sensor n. "));
			Serial.print(_currentSensor, DEC);
			Serial.println();	
		}
		_currentSensor++;
		
		if (_currentSensor == SENS_N)
		{
			_currentSensor = 0;
			//send message
			/*
			Message format:
			I already don't like the fact that with this library I have to resort to
			floats, anyway the message is sent structured like this:
			[SENDER ID][RECEIV ID][TEMP_INT][TEMP_DEC][HUM_INT][HUM_DEC][EMPTY][EMPTY]
			That's because:
			-Float operations are slow (all float ops in this daemon will be conveted to int one day)
			-Floats may yield wrong results (eg. : 6.0 / 3.0 doesn't always equal exactly to 2.0)
			-Floats consume much more memory space in Arduino (4 bytes) compared to a pair of bytes for
				the integer part and the decimal one
			-This daemon only needs a small range of numbers, [0-255] is ana acceptable range
			*/
			_tempAvg /= SENS_N;
			byte int_part = _tempAvg; //extracts the integer part from the temperature
			byte dec_part = (_tempAvg - int_part) * 10; //extracts the decimal part from the temperature
			
			pushMessageData(int_part);
			pushMessageData(dec_part);
			
			_humAvg /= SENS_N;
			int_part = _humAvg;
			dec_part = (_humAvg - int_part) * 10;
			
			pushMessageData(int_part);
			pushMessageData(dec_part);
			sendMessage(MENU_D);
			//sendMessage(LOGGER_D);
			clearMessageData();
			_tempAvg = 0;
			_humAvg = 0;
		}
	}
}

void SensorsDaemon::_execute(const Message& msg)
{

}