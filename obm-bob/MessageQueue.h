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
#ifndef MESSAGEQUEUE_H
#define MESSAGEQUEUE_H
#include <Arduino.h>
#include "Message.h"

#define MAX_QUEUE_SIZE 8
/*
A MessageQueue holds messages.
Internally, it's a fixed size circular array which holds Message type data.
Two operators are overloaded for this class:
>> (dequeue) : pulls a message out from the queue
<< (enqueue): pushes a message in the queue
If a queue is empty, the >> operator doesn't work and returns a null message;
if a queue is full, the << operator doesn't work and the message is discarded.
*/

class MessageQueue
{
    public:
        MessageQueue();
        void enqueue(const Message& msg);
        const Message& dequeue();
        boolean isEmpty();
        boolean isFull();
    private:
        Message _array[MAX_QUEUE_SIZE];
        byte _qIndex, _qHead;

};

#endif // MESSAGEQUEUE_H
