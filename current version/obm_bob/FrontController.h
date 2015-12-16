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
#include "SensorManager.h"
#include "ClockManager.h"
//#include "AlarmManager.h"

class FrontController
{
	public:
		static void setup()
		{
			_sensMgr.setup();
			_clockMgr.setup();
		}
		static SensorManager& getSensorsManager();
		static ClockManager& getClockManager();
		static void update()
		{
			_sensMgr.update();
		}
	private: 
		static SensorManager _sensMgr;
		static ClockManager _clockMgr;
};

SensorManager FrontController::_sensMgr;
ClockManager FrontController::_clockMgr;

SensorManager& FrontController::getSensorsManager()
{
	return _sensMgr;
}

ClockManager& FrontController::getClockManager()
{
	return _clockMgr;
}