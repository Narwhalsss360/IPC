#include <pipe_client.h>
#include <exception>
#include <system_error>

using std::string;
using std::system_error;
using std::error_code;
using std::generic_category;

pipe_client::pipe_client(string pipe_name)
	: pipe_handle(INVALID_HANDLE_VALUE), pipe_name("\\\\.\\pipe\\" + pipe_name), _connected(false)
{
}

void pipe_client::connect()
{
	pipe_handle = CreateFileA(pipe_name.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (pipe_handle == INVALID_HANDLE_VALUE)
	{
		throw system_error(error_code(GetLastError(), generic_category()));
	}

	DWORD mode = PIPE_READMODE_BYTE | PIPE_WAIT;
	SetNamedPipeHandleState(pipe_handle, &mode, NULL, NULL);
}

int pipe_client::sendMessage(void* buffer, size_t length)
{
	int errorCode = 0;
	bool success = WriteFile(pipe_handle, buffer, length, NULL, NULL);
	if (!success)
		errorCode = GetLastError();
	return errorCode;
}

pipe_client::~pipe_client()
{
	if (pipe_handle != INVALID_HANDLE_VALUE)
		CloseHandle(pipe_handle);
}