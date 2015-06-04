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
#include "Daemon.h"

byte Daemon::_daemonCounter = 0;

MessageQueue& getMessageBus()
{
    static MessageQueue messageBus;
    return messageBus;
}

Daemon::Daemon()
{
    //ctor
    _daemonID = _daemonCounter++;
    _daemonState = D_RUNNING;
}

void Daemon::step()
{
    switch(_daemonState)
    {
        case D_RUNNING:
            //if there's a message in the inbox, switch to WAITING
            if (!_inbox.isEmpty()) _daemonState = D_WAITING;
            else _run();
            break;
        case D_WAITING:
            //if the inbox is not empty, switch to EXECUTE
            if (!_inbox.isEmpty()) _daemonState = D_EXECUTING;
            break;
        case D_EXECUTING:
            if (_inbox.isEmpty()) _daemonState = D_RUNNING;
            else _execute(_inbox.dequeue());
            break;
    }
}

 void Daemon::receiveMessage(const Message& msg)
 {
     _inbox.enqueue(msg);
 }

 
 void Daemon::sendMessage(const byte& targetID, const byte* dataArray)
 {
	Message toSend;
	toSend.senderID = _daemonID;
	toSend.receiverID = targetID;
	if (dataArray != NULL)
		for (byte i = 0; i < MESSAGE_SIZE; i++) 
			toSend.data[i] = dataArray[i];
	getMessageBus().enqueue(toSend);
 }