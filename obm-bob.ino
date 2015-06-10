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
//including main Arduino liraries here, otherwise it complains -.-
#include <Arduino.h>
#include <Wire.h>
#include <RTClib.h>
#include <DHT.h>

#include "Message.h"
#include "MessageQueue.h"
#include "Daemon.h"

#include "Daemons.h"

#include "SerialDaemon.h"
#include "RTCDaemon.h"
#include "SensorsDaemon.h"
#include "MenuDaemon.h"
#include "InputDaemon.h"

byte _daemonCounter = 0;
Daemon* daemons[DAEMON_COUNT];

SerialDaemon serial_d(SERIAL_D);
RTCDaemon rtc_d(RTC_D);
SensorsDaemon sensors_d(SENSORS_D);
MenuDaemon menu_d(MENU_D);
InputDaemon input_d(INPUT_D);


void setup()
{
  //starts the serial interface
  Serial.begin(9600);
  //starts the I2C/Wire interface
  Wire.begin();
  
  //Startup
  Serial.println("Open Bio Medical - B.O.B.");
  Serial.println("======INIT======");
  Serial.println("Loading daemons...");
  daemons[RTC_D] = &rtc_d;
  daemons[SERIAL_D] = &serial_d;
  daemons[SENSORS_D] = &sensors_d;
  daemons[MENU_D] = &menu_d;
  daemons[INPUT_D] = &input_d;
  
  Serial.println("Starting Daemons...");
  for (byte i =0; i<DAEMON_COUNT; i++)
    daemons[i]->setup();
  Serial.println("======READY======");
}

void loop()
{
    //execute the current daemon
    daemons[_daemonCounter]->step();
    _daemonCounter = (++_daemonCounter) % DAEMON_COUNT;
    //dispatcher sends messages from the message queue to the daemons
    //std::cout << "Dispatching phase...\n";
    while (!getMessageBus().isEmpty())
    {
        Message currentMsg = getMessageBus().dequeue();
        daemons[currentMsg.receiverID]->receiveMessage(currentMsg);
    }
}
