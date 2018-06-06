#ifndef GUADR_state_machine
#define GUADR_state_machine

#include <stdio.h>
#include <string.h>

namespace stMachn
{

	class StateMachine
	{

		int actualState, finalState;
		char * states;
	public:
		StateMachine(const char * inputStates, size_t size);
		bool stateInput(const char input);
		~StateMachine();

	};

}

#endif // GUADR_state_machine
