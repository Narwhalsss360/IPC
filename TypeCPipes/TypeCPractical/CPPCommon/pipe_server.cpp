#include <pipe_server.h>
#include <exception>
#include <system_error>
#include <stdexcept>

using std::vector;
using std::string;
using std::exception;
using std::logic_error;
using std::system_error;
using std::error_code;
using std::generic_category;

named_pipes::pipe_server::pipe_server(string pipe_name)
	: _pipe_handle(NULL), _pipe_name(pipe_name), _connected(false), _created(false)
{
}

void named_pipes::pipe_server::create(string pipe_name)
{
	if (_created)
		throw logic_error("Pipe was already created.");
	if (pipe_name.length())
		_pipe_name = pipe_name;

	_created = true;
}

void named_pipes::pipe_server::send(void* buffer, size_t length)
{
	if (!_connected)
		throw logic_error("Pipe is not connected.");
}

void named_pipes::pipe_server::send(const char* const c_str)
{
	send((void*)c_str, strnlen_s(c_str, 255));
}

void named_pipes::pipe_server::send(string str)
{
	send((void*)str.c_str(), str.length());
}

void named_pipes::pipe_server::connect()
{
	if (!_created)
		throw logic_error("Pipe was not created.");
	if (_connected)
		throw logic_error("Pipe was already connected");

	_connected = true;
}

void named_pipes::pipe_server::disconnect()
{
	if (!_connected)
		throw logic_error("Pipe was already disconnected.");
}

void named_pipes::pipe_server::destroy()
{
	_created = false;
}

HANDLE named_pipes::pipe_server::handle()
{
	return _pipe_handle;
}

named_pipes::pipe_server::~pipe_server()
{
}

pipe_server::pipe_server(string pipe_name)
	: pipe_handle(INVALID_HANDLE_VALUE), pipe_name("\\\\.\\pipe\\" + pipe_name), _connected(false)
{
	
}

void pipe_server::create()
{
	pipe_handle = CreateNamedPipeA(pipe_name.c_str(), PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE | PIPE_WAIT, PIPE_UNLIMITED_INSTANCES, 0, 0, 0, NULL);
	if (pipe_handle == INVALID_HANDLE_VALUE)
	{
		throw system_error(error_code(GetLastError(), generic_category()));
	}
}

void pipe_server::connect()
{
	if (_connected)
	{
		throw exception("pipe was already connected");
	}

	if (pipe_handle == INVALID_HANDLE_VALUE)
	{
		throw exception("pipe_handle was invalid");
	}

	bool result = ConnectNamedPipe(pipe_handle, NULL);

	if (!result)
	{
		DWORD err = GetLastError();
		if (err != ERROR_PIPE_CONNECTED)
		{
			CloseHandle(pipe_handle);
			pipe_handle = INVALID_HANDLE_VALUE;
			throw system_error(error_code(err, generic_category()));
		}
	}

	_connected = true;
}

int pipe_server::sendMessage(void* buffer, size_t length)
{
	int errorCode = 0;
	bool success = WriteFile(pipe_handle, buffer, length, NULL, NULL);
	if (!success)
		errorCode = GetLastError();
	return errorCode;
}

vector<uint8_t> pipe_server::readMessage()
{
	vector<uint8_t> readBytes;

	DWORD bytesToRead;
	PeekNamedPipe(pipe_handle, NULL, NULL, NULL, NULL, &bytesToRead);
	if (bytesToRead)
	{
		int errorCode;
		readBytes.resize(bytesToRead);
		bool result = ReadFile(pipe_handle, &readBytes[0], bytesToRead, NULL, NULL);
		if (!result)
		{
			errorCode = GetLastError();
			if (errorCode != ERROR_MORE_DATA)
			{
				throw system_error(error_code(errorCode, generic_category()));
			}
		}
	}

	return readBytes;
}

void pipe_server::disconnect()
{
	if (!_connected)
	{
		throw exception("pipe was already disconnected");
	}

	if (pipe_handle == INVALID_HANDLE_VALUE)
	{
		throw exception("pipe_handle was invalid");
	}

	DisconnectNamedPipe(pipe_handle);
	CloseHandle(pipe_handle);
}

bool pipe_server::connected()
{
	return _connected;
}

pipe_server::~pipe_server()
{
	if (connected() && pipe_handle != INVALID_HANDLE_VALUE)
		disconnect();
}