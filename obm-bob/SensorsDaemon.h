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
#include <DHT.h>
#include "Daemon.h"

#define SENS1 0
#define SENS2 1
#define SENS3 2

#define SENS1_PIN 2
#define SENS2_PIN 3
#define SENS3_PIN 4

#define SENS_N 3

#define SENSOR_INTERVAL 2000



class SensorsDaemon : public Daemon
{
	public:
		SensorsDaemon(byte ID) : Daemon(ID){}
		virtual void setup();
	protected:
		virtual void _run();
		virtual void _execute(const Message& msg);
	private:
		unsigned long int _previousTime = 0; 
		//if we use a standard int, it quickly overflows
		
		//this is horrendous, I need to find a proper DHT library that allows deferred init	
		static DHT sensor1;
		static DHT sensor2;
		static DHT sensor3;		
		
		DHT* sensors[SENS_N];
		byte _currentSensor;
		
		float _tempAvg, _humAvg;
		
};