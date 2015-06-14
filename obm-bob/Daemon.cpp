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

Message Daemon::_outMsg = nullMessage;

MessageQueue& getMessageBus()
{
    static MessageQueue messageBus;
    return messageBus;
}

Daemon::Daemon(byte ID)
{
    //ctor
    _daemonID = ID;
    _daemonState = D_RUNNING;
	_dataIndex = 0;
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

 
 void Daemon::sendMessage(const byte targetID)
 {
	_outMsg.senderID = _daemonID;
	_outMsg.receiverID = targetID;
	getMessageBus().enqueue(_outMsg);
 }
 
 void Daemon::clearMessageData()
 {
	//clears the message
	for (_dataIndex = 0; _dataIndex < MESSAGE_SIZE; _dataIndex++)
		_outMsg.data[_dataIndex] = 0;
	_dataIndex = 0;
 }
 
 void Daemon::pushMessageData(byte data)
 {
	if (_dataIndex < MESSAGE_SIZE)
		_outMsg.data[_dataIndex++] = data;
 }