#pragma once
#include <string>
#include <Windows.h>
#include <vector>

struct pipe_server
{
	HANDLE pipe_handle;

	std::string pipe_name;

	pipe_server(std::string pipe_name);

	void create();

	void connect();

	int sendMessage(void* buffer, size_t length);

	std::vector<uint8_t> readMessage();

	void disconnect();

	bool connected();

	~pipe_server() noexcept;

private:
	bool _connected;
};