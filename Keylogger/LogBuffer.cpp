#include "LogBuffer.h"

using namespace lgbuff;

// 
int getStringSize(char * input)
{
	int size = 0;
	if (input == nullptr)
		return 0;

	while (input[size] != '\0')
		size++;

	return size;
}

// constructor
LogBuffer::LogBuffer()
{
	//open and append or create log file 
	std::time_t t = std::time(0);   // get time now
	std::tm* actualTime = std::localtime(&t);

	char fileName[32]; // file name format log_dd-mm
	sprintf(fileName, "log_%d-%d", actualTime->tm_mday, actualTime->tm_mon + 1);

	outFile = fopen(fileName, "a+");
	if (outFile == NULL) // serious problem with logging files, end process
		exit(0);

	// Get disk sector size for buffer size
	DWORD dwSectPerClust, dwBytesPerSect, dwFreeClusters, dwTotalClusters;

	// look for cluster size in classical C:\ partition
	const char  szDrive[4] = { 'C', ':', '\\', '\0' };  // '\' must be in trail 
	if (GetDiskFreeSpace(szDrive, &dwSectPerClust, &dwBytesPerSect, &dwFreeClusters, &dwTotalClusters))
		clusterSize = dwSectPerClust * dwBytesPerSect;
	else // if there is no succes in finding out C partition sector size set default one
		clusterSize = DEFAUL_BUFF_SIZE;

	// PLACEHOLDER FOR TESTING !!
	//clusterSize = 10;
	printf("BUFFER SIZE %d\n", clusterSize);

	// create logging buffer and initialize to default value 
	buffer = new char[clusterSize];
	memset(buffer, '\0', clusterSize);
	actPosition = 0;

	// initial log message after opening a file
	char  initialMessage[64];
	memset(initialMessage, '\0', 64);
	sprintf(initialMessage, "\n\n#Logging started at %d:%d:%d\n\n", actualTime->tm_hour, actualTime->tm_min, actualTime->tm_sec);
	putString(initialMessage, getStringSize(initialMessage));
}

int LogBuffer::flushBuff()
{
	// PLACEHOLDER
	printf("%s", buffer);

	fwrite(buffer, sizeof(char), clusterSize, outFile);
	//  Ensure the buffer's written to the OS ...
	fflush(outFile);
	//  Tell the OS to sync it with the real device.
	FlushFileBuffers((HANDLE)_fileno(outFile));

	actPosition = 0;

	return 0;
}

// put character in buffer
void LogBuffer::putChar(const char input)
{
	buffer[actPosition++] = input;
	if (actPosition == clusterSize) {
		buffer[actPosition] = '\0';
		flushBuff();
	}
}

// put string in buffer
int LogBuffer::putString(const char * input, unsigned int size)
{
	// not calling putChar for performance issues
	for (size_t x = 0; x < size; x++) {
		buffer[actPosition++] = input[x];
		if (actPosition == clusterSize) {
			buffer[actPosition] = '\0';
			flushBuff();
		}
	}

	return 0;
}

// deconstructor
LogBuffer::~LogBuffer()
{
	// last buffer flush before exit
	flushBuff();
	delete buffer;
	fclose(outFile);
}