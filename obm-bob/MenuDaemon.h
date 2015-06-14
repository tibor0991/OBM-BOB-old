#ifndef MENU_D_H
#define MENU_D_H

#include "Daemon.h"

#define IDLE_ROLL_TIME 2000

class MenuDaemon : public Daemon
{
	public:
		MenuDaemon(byte ID) : Daemon(ID){}
        virtual void setup();
	protected:
		virtual void _run();
        virtual void _execute(const Message& msg);
	private:
		enum MENU_STATES {IDLE_SCREEN1 = 0, IDLE_SCREEN2};
		MENU_STATES _currentState;
		
		//IDLE SCREEN vars
		byte _tempInt, _tempDec;
		byte _humInt, _humDec;
		unsigned long int _rollTime;
};

#endif  MENU_D_H