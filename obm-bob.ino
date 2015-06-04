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

#include "Message.h"
#include "MessageQueue.h"
#include "Daemon.h"

#include "Daemons.h"

byte _daemonCounter = 0;
Daemon* daemons[DAEMON_COUNT];

SerialDaemon serial_d;
RTCDaemon rtc_d;


void setup()
{
	daemons[RTC_D] = &rtc_d;
	daemons[SERIAL_D] = &serial_d;
}

void loop()
{
    //execute the current daemon
    daemons[_daemonCounter]->step();
    _daemonCounter = (++_daemonCounter) % 2;
    //dispatcher sends messages from the message queue to the daemons
    //std::cout << "Dispatching phase...\n";
    while (!getMessageBus().isEmpty())
    {
        Message currentMsg = getMessageBus().dequeue();
        daemons[currentMsg.receiverID]->receiveMessage(currentMsg);
    }
}
