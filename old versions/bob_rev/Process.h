#ifndef PROCESS_H
#define PROCESS_H

#include "DeviceManager.h"
#include "MessageQueue.h"

enum PROCESS_ID { 	//MENU = 0, 
					SENSORS = 0, 
					//LOGGER, 
					PROC_COUNT };
					
MessageQueue& getMessageBus();

enum ProcessState {P_WAITING, P_RUNNING, P_EXECUTING};

class Process 
{
    public:
        Process(byte ID);
        void step();
        void receiveMessage(const Message& msg);
    protected:
        virtual void _run() = 0;
        virtual void _execute(const Message& msg) = 0;
        MessageQueue _inbox;
		void sendMessage(const byte targetID);
		void pushMessageData(byte data);
		void clearMessageData();
	private:
		byte _processID;
		ProcessState _processState;
		static Message _outMsg;
		byte _dataIndex;
};

#endif