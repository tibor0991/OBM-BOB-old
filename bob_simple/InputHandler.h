//input values
#define ENTER_VALUE  895
#define DOWN_VALUE   767
#define LEFT_VALUE   639
#define UP_VALUE     510
#define RIGHT_VALUE  382
#define BACK_VALUE	 254
#define RESET_VALUE  126
#define REST_VALUE   1

//button IDs
#define ENTER_BUTTON 	0x00000001
#define DOWN_BUTTON  	0x00000010
#define LEFT_BUTTON   	0x00000100
#define UP_BUTTON     	0x00001000
#define RIGHT_BUTTON  	0x00010000
#define BACK_BUTTON     0x00100000
#define RESET_BUTTON  	0x01000000
#define REST_BUTTON		0x10000000

#define INPUT_TOL	 1
#define MAX_BUTTONS		8
#define LOCKDOWN_TIMER 250

#include <Arduino.h>

struct Button
{
	byte ID;	//the button ID
	int value;	//the median input value assigned to this button
};


#define PASTER(x, y) x##y
#define ASSIGN_BUTTON(index, NAME) \
			_buttonList[index].ID = PASTER(NAME,_BUTTON); \
			_buttonList[index].value = PASTER(NAME,_VALUE);

class InputHandler
{
	public:
		InputHandler(byte pin) : 
			_pressed(false), 
			_currentButton(REST_BUTTON),
			_inputPin(pin),
			_lockdown(0)
		{
			pinMode(pin, INPUT);

			//assign button values and inputs
			ASSIGN_BUTTON(0, UP)
			ASSIGN_BUTTON(1, LEFT)
			ASSIGN_BUTTON(2, DOWN)
			ASSIGN_BUTTON(3, RIGHT)
			ASSIGN_BUTTON(4, ENTER)
			ASSIGN_BUTTON(5, BACK)
			ASSIGN_BUTTON(6, RESET)
			ASSIGN_BUTTON(7, REST)
		}
		byte acquireInput();
	private:
		bool _checkButton(byte inputValue, Button& button);
		bool _pressed;
		byte _currentButton;
		byte _inputPin;
		Button _buttonList[MAX_BUTTONS];
		unsigned long int _lockdown;
};

byte InputHandler::acquireInput()
{
	int inputValue = analogRead(_inputPin);
	if (!_pressed)
	{	
		for (byte index = 0; index < MAX_BUTTONS; index++)
		{	
			if (_checkButton(inputValue, _buttonList[index]))
			{
				_currentButton = _buttonList[index].ID;
				_pressed = true;
				_lockdown = millis() + LOCKDOWN_TIMER;
				return _currentButton;
			}
		}
	}
	else if ( millis() >= _lockdown)
	{	
		_pressed = false;
		_currentButton = REST_BUTTON;
		return _currentButton;
	}
	else
	{
		return REST_BUTTON;
	}
}


bool InputHandler::_checkButton(byte inputValue, Button& button)
{
	return (
		inputValue <= button.value+INPUT_TOL 
		&& inputValue >= button.value-INPUT_TOL);

}


