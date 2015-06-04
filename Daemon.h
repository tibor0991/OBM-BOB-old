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
#ifndef DAEMON_H
#define DAEMON_H

/*
A Daemon is a service which runs at the start of the OS and keeps running until the device is turned off.
The main features of a Daemon are:
-Message parsing: parses the informations received in a message
-Message sending: sends messages to other Daemons using the message bus
-Inbox: holds a list of incoming messages
-Run method: implements the logic of the Daemon (activities, programmed tasks, etc...)
-Daemon ID: unique identifier of the Daemon process
*/

#include "MessageQueue.h"

MessageQueue& getMessageBus();

enum DaemonState {D_WAITING, D_RUNNING, D_EXECUTING};

class Daemon
{
    public:
        Daemon();
        void step();
        void receiveMessage(const Message& msg);
    protected:
        virtual void _run() = 0;
        virtual void _execute(const Message& msg) = 0;
        MessageQueue _inbox;
		void sendMessage(const byte& targetID, const byte* dataArray);
    private:
		byte _daemonID;
        static byte _daemonCounter;
		DaemonState _daemonState;
		
};

#endif // DAEMON_H
