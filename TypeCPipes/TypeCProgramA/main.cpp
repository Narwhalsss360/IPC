#include <Windows.h>
#include <iostream>
#include <string>
#include <algorithm>
#include <TypeCCommon.h>

using std::cout;
using std::cin;
using std::getline;
using std::string;
using std::transform;

int main()
{
	int exitCode = 0;
	bool connected = false;
	DWORD bytesWritten;
	bool written;

	HANDLE pipeHandle = CreateNamedPipeA(PIPE_NAME, PIPE_ACCESS_DUPLEX | FILE_FLAG_FIRST_PIPE_INSTANCE, PIPE_TYPE_MESSAGE | PIPE_WAIT, PIPE_UNLIMITED_INSTANCES, 0, 0, 0, NULL);

	if (pipeHandle == NULL || pipeHandle == INVALID_HANDLE_VALUE)
	{
		exitCode = GetLastError();
		goto programExit;
	}

	connected = ConnectNamedPipe(pipeHandle, nullptr);

	if (!connected)
	{
		exitCode = GetLastError();
		if (exitCode != ERROR_PIPE_CONNECTED)
			goto programExit;
		exitCode = 0;
	}

	while (true)
	{
		cout << "Enter message to send (type \"exit\" to exit):";
		string line;
		getline(cin, line);
		if (line.length() == 0)
			continue;

		string exitCheck = line;
		transform(exitCheck.begin(), exitCheck.end(), exitCheck.begin(), toupper);
		if (exitCheck == "EXIT")
			break;

		line += "\n\r";
		written = WriteFile(pipeHandle, line.c_str(), line.length(), NULL, NULL);
		if (!written)
			cout << "\nThere was an error writing to the pipe. " << GetLastError();
	}

programExit:
	if (pipeHandle)
		CloseHandle(pipeHandle);

	if (exitCode)
	{
		cout << "uint32_t error code: " << *(uint32_t*)&exitCode << '\n';
	}

	return exitCode;
}