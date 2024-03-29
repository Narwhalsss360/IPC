#pragma once

#include <string>
#include <Windows.h>

struct pipe_client
{
	HANDLE pipe_handle;

	std::string pipe_name;

	pipe_client(std::string pipe_name);

	void connect();

	int sendMessage(void* buffer, size_t length);

	~pipe_client() noexcept;

private:
	bool _connected;
};