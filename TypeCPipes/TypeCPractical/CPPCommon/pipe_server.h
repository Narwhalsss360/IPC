#pragma once
#include <string>
#include <Windows.h>

struct pipe_server
{
	HANDLE pipe_handle;

	std::string pipe_name;

	pipe_server(std::string pipe_name);

	void connect();

	void disconnect();

	bool connected();

	~pipe_server() noexcept;

private:
	bool _connected;
};