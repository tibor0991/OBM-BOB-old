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
#include "MessageQueue.h"

MessageQueue::MessageQueue() : _qIndex(0), _qHead(0)
{
    for (byte i=0; i<MAX_QUEUE_SIZE; i++)
      _array[i] = nullMessage;
}

void MessageQueue::enqueue(const Message& msg)
{
    if ( !isFull() )
    {
        //insert into _qIndex, then increase _qIndex
        _array[_qIndex] = msg;
        _qIndex = (++_qIndex) % MAX_QUEUE_SIZE;
    }
}

const Message& MessageQueue::dequeue()
{
    if ( isEmpty() ) //queue is empty
        return nullMessage;
    else
    {
        byte msgIndex = _qHead;
        _qHead = (++_qHead) % MAX_QUEUE_SIZE;
        return _array[msgIndex];
    }
}

boolean MessageQueue::isEmpty()
{
    return ( _qIndex == _qHead );
}


boolean MessageQueue::isFull()
{
    return ( ( (_qIndex + 1) % MAX_QUEUE_SIZE ) == _qHead );
}
