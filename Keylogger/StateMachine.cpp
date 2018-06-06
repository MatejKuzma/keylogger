#include "StateMachine.h"

namespace stMachn
{

	StateMachine::StateMachine(const char * inputStates, size_t size)
	{
		// allocate place for state machine 
		StateMachine::states = new char[size];

		// copy the states into memory
		for (size_t x = 0; x < size; x++)
			StateMachine::states[x] = inputStates[x];

		StateMachine::finalState = size;
		StateMachine::actualState = 0;
	}

	// resolve machine state based on the input
	bool StateMachine::stateInput(const char input)
	{
		if (input == states[actualState])
			actualState++;
		else {
			actualState = 0;
			return false;
		}

		if (actualState == finalState) {
			actualState = 0;
			return true;
		}
		return false;
	}

	StateMachine::~StateMachine()
	{
		delete StateMachine::states;
	}


}

