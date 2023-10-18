#pragma once
#include <string>
#include <Windows.h>
#include <vector>
#include <iterator>

namespace named_pipes
{
	class pipe_server
	{
	public:
		pipe_server(std::string pipe_name);

		void create(std::string pipe_name = std::string());

		void send(void* buffer, size_t length);

		void send(const char* const c_str);

		void send(std::string str);

		void connect();

		void disconnect();

		void destroy();

		HANDLE handle();

		~pipe_server();
	private:
		HANDLE _pipe_handle;

		std::string _pipe_name;

		bool _connected;

		bool _created;
	};
}

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