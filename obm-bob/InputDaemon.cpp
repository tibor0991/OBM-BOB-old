#include "InputDaemon.h"

void InputDaemon::setup()
{
	Serial.println("Starting InputDaemon...");
}

void InputDaemon::_run()
{
	_bufferIndex = 0;
	while (Serial.available() > 0 && _bufferIndex < 8)
	{
		byte input = Serial.read();
		if ( //this is wrong, but currently is the only right method
			input == 'w' ||	//up
			input == 'a' ||	//left
			input == 's' ||	//down
			input == 'd' ||	//right
			input == 'q' || //back
			input == 'e' )  //ok
		_inputBuffer[_bufferIndex++] = input;
	}
	
	if (_bufferIndex > 0) // at least one input received
	{
		_bufferIndex = 0;
		while (_bufferIndex < 6)
		{
			pushMessageData(_inputBuffer[_bufferIndex]);
			_bufferIndex++;
		}
			
		sendMessage(MENU_D);
		clearMessageData();
	}
}

void InputDaemon::_execute(const Message& msg)
{

}