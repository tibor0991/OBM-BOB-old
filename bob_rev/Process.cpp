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

#include "Process.h"

Message Process::_outMsg = nullMessage;

MessageQueue& getMessageBus()
{
    static MessageQueue messageBus;
    return messageBus;
}

Process::Process(byte ID)
{
    //ctor
    _processID = ID;
    _processState = P_RUNNING;
	_dataIndex = 0;
}

void Process::step()
{
    switch(_processState)
    {
        case P_RUNNING:
            //if there's a message in the inbox, switch to WAITING
            if (!_inbox.isEmpty()) _processState = P_WAITING;
            else _run();
            break;
        case P_WAITING:
            //if the inbox is not empty, switch to EXECUTE
            if (!_inbox.isEmpty()) _processState = P_EXECUTING;
            break;
        case P_EXECUTING:
            if (_inbox.isEmpty()) _processState = P_RUNNING;
            else _execute(_inbox.dequeue());
            break;
    }
}

 void Process::receiveMessage(const Message& msg)
 {
     _inbox.enqueue(msg);
 }

 
 void Process::sendMessage(const byte targetID)
 {
	_outMsg.senderID = _processID;
	_outMsg.receiverID = targetID;
	getMessageBus().enqueue(_outMsg);
 }
 
 void Process::clearMessageData()
 {
	//clears the message
	for (_dataIndex = 0; _dataIndex < MESSAGE_SIZE; _dataIndex++)
		_outMsg.data[_dataIndex] = 0;
	_dataIndex = 0;
 }
 
 void Process::pushMessageData(byte data)
 {
	if (_dataIndex < MESSAGE_SIZE)
		_outMsg.data[_dataIndex++] = data;
 }