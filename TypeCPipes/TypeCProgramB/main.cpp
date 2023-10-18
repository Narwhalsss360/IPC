#include <Windows.h>
#include <TypeCCommon.h>
#include <iostream>

using std::cout;
using std::cin;
using std::endl;

HANDLE pipeHandle = nullptr;

void main_atexit()
{
	if (pipeHandle)
		CloseHandle(pipeHandle);
}

int main()
{
	atexit(main_atexit);
	cout << "===Pipe Reader===\nAttempting to connect to pipe...\n";

retryConnection:
	pipeHandle = CreateFileA(PIPE_NAME, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

	if (GetLastError() == ERROR_PIPE_BUSY || GetLastError() == ERROR_FILE_NOT_FOUND)
	{
		goto retryConnection;
	}

	if (!pipeHandle || pipeHandle == INVALID_HANDLE_VALUE)
	{
		cout << "There was an error opening the pipe...\n";
		if (pipeHandle)
			CloseHandle(pipeHandle);
		return GetLastError();
	}

	DWORD mode = PIPE_READMODE_BYTE | PIPE_WAIT;
	bool success = SetNamedPipeHandleState(pipeHandle, &mode, NULL, NULL);

	if (!success)
	{
		CloseHandle(pipeHandle);
		return GetLastError();
	}

	cout << "Connected to pipe.\n";

	bool readSuccess;
	int err = 0;
	while (true)
	{
		unsigned char buf;
		readSuccess = ReadFile(pipeHandle, &buf, 1, nullptr, nullptr);
		if (readSuccess)
		{
			cout << buf;
		}
		else
		{
			err = GetLastError();
			if (err != ERROR_MORE_DATA)
			{
				break;
			}
		}
	}

	CloseHandle(pipeHandle);

	if (err == ERROR_BROKEN_PIPE)
	{
		cout << "The pipe has closed.\n";
	}
	return err;
}