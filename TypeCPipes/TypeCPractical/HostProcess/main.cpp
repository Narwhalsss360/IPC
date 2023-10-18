#include <thread>
#include <vector>
#include "PracticalCommon.h"
#include <pipe_server.h>
#include <string>
#include <iostream>

using std::thread;
using std::exception_ptr;
using std::vector;
using std::string;
using std::cout;
using namespace std::this_thread;

volatile bool continue_listen = true;

void request_listener()
{
}

int main(int argc, char* argv[])
{
	pipe_server req = pipe_server(PIPE_OPEN_REQUEST_NAME);
	req.create();
	req.connect();

	while (true)
	{
		std::vector<uint8_t> bytesRead = req.readMessage();
		if (bytesRead.size() == 0)
			continue;

		string printOut = string();
		printOut.reserve(bytesRead.size());
		for (const char& c : bytesRead)
			printOut += c;
		cout << printOut << '\n';
	}

	req.disconnect();

	return 0;
	thread request_listener_thread = thread(request_listener);

	continue_listen = false;
	request_listener_thread.join();
}