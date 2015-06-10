#ifndef INPUT_D_H
#define INPUT_D_H

#include "Daemon.h"

class InputDaemon : public Daemon
{
	public:
		InputDaemon(byte ID) : Daemon(ID){}
        virtual void setup();
	protected:
		virtual void _run();
        virtual void _execute(const Message& msg);
	private:
		byte _inputBuffer[8];
		byte _bufferIndex;
};

#endif  INPUT_D_H
