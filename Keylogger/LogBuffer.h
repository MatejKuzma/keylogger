/* SINGLETON CLASS - there should always be only one logging buffer instance, because buffer is directly connected to
logging file. Logging buffer creates, writes and closes logging file.
*/

#ifndef _LOG_BUFFER
#define _LOG_BUFFER

#include <windows.h>
#include <fstream>
#include <iostream>
#include <ctime>

#define DEFAUL_BUFF_SIZE 4096;

namespace lgbuff
{

	class LogBuffer
	{
		unsigned int clusterSize; // variable for sector size
		unsigned int actPosition; // actual position in buffer
		char * buffer; // logging buffer
		FILE * outFile; // log file file descriptor 

	private:
		LogBuffer();
		// flush all of buffer data to output file
		int flushBuff();

	public:
		static LogBuffer * getInstance()
		{
			static LogBuffer instance;
			return &instance;
		}

		~LogBuffer();

		// put character into buffer, if treshold exceeds limit than flush the buffer
		void putChar(const char input);

		// put string into buffer, if treshold exceeds limit than flush the buffer
		int putString(const char * input, unsigned int size);
	};

}

#endif // _LOG_BUFFER
