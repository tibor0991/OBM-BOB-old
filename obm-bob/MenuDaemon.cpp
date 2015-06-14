#include "MenuDaemon.h"

void MenuDaemon::setup()
{
	Serial.println("Starting InputDaemon...");
	_currentState = IDLE_SCREEN1;
	_rollTime = 0;
}

void MenuDaemon::_run()
{
	/*
	Idle screen example:
	           111111
	 0123456789012345
	|TEMP: 36.5      |
	|HUM: 80%        |
	*/

	switch (_currentState)
	{
		case IDLE_SCREEN1:
			//prints the first idle page
			Serial.print("TEMP: ");
			Serial.print(_tempInt, DEC);
			Serial.print(".");
			Serial.print(_tempDec, DEC);
			Serial.println();
			Serial.print("HUM : ");
			Serial.print(_humInt, DEC);
			Serial.print(".");
			Serial.print(_humDec, DEC);
			Serial.println();
			if (millis() - _rollTime >= IDLE_ROLL_TIME)
			{
				_rollTime+= IDLE_ROLL_TIME;
				_currentState = IDLE_SCREEN2;
			}			
			break;
		case IDLE_SCREEN2:
			//prints data on the second idle page

			if (millis() - _rollTime >= IDLE_ROLL_TIME)
			{
				_rollTime+= IDLE_ROLL_TIME;
				_currentState = IDLE_SCREEN1;
			}
			break;
	}
}

void MenuDaemon::_execute(const Message& msg)
{
	switch(msg.senderID)
	{
		case SENSORS_D:
			Serial.println("Received a message from SENSORS_D");
			_tempInt = msg.data[0];
			_tempDec = msg.data[1];
			_humInt = msg.data[2];
			_humDec = msg.data[3];
			break;
		case INPUT_D:
			//it just prints what input it gets right now, it will be changed soon
			Serial.print("Received a message from INPUT_D :");
			for (byte i=0; i<MESSAGE_SIZE; i++)
			{
				Serial.print("[");
				Serial.write(msg.data[i]);
				Serial.print("] ");
			}
			Serial.println();
			break;
	}
}