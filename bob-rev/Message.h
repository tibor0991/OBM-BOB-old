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
#ifndef MESSAGE_H
#define MESSAGE_H

#define MESSAGE_SIZE 6 //the total size of a message
#define NULL_ID 255

#include <Arduino.h>

struct Message
{
    byte senderID;
    byte receiverID;
    byte data[MESSAGE_SIZE];
    Message()
    {
        senderID = NULL_ID;
        receiverID = NULL_ID;
        //memset(data, 0, sizeof(data));
        for (byte i=0; i<MESSAGE_SIZE; i++) data[i] = 0;
    }

    Message(const Message& original)
    {
        senderID = original.senderID;
        receiverID = original.receiverID;
        for (byte i=0; i<MESSAGE_SIZE; i++) data[i] = original.data[i];
    }

    Message& operator=(const Message& rhs)
    {
        senderID = rhs.senderID;
        receiverID = rhs.receiverID;
        for (byte i=0; i<MESSAGE_SIZE; i++) data[i] = rhs.data[i];
        return *this;
    }

};

static Message nullMessage;

#endif // MESSAGE_H
