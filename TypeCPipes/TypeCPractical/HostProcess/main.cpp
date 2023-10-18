#include <thread>
#include <vector>
#include "PracticalCommon.h"
#include <pipe_server.h>

using std::thread;
using std::exception_ptr;
using std::vector;
using namespace std::this_thread;

volatile bool listen = true;

void request_listener()
{
}

int main(int argc, char* argv[])
{
	pipe_server req = pipe_server(PIPE_OPEN_REQUEST_NAME);
	req.connect();

	while (true)
	{
	wait_for_message:
		DWORD bytesToRead;
		PeekNamedPipe(req.pipe_handle, NULL, 0, 0, NULL, &bytesToRead);
		if (bytesToRead == 0)
			goto wait_for_message;

		vector<unsigned char> bytes_read = vector<unsigned char>(bytesToRead);
		ReadFile(req.pipe_handle, &bytes_read[0], bytesToRead, NULL, NULL);
	}

	req.disconnect();

	return 0;
	thread request_listener_thread = thread(request_listener);

	listen = false;
	request_listener_thread.join();
}