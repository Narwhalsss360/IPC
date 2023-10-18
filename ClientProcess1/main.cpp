#include <pipe_client.h>
#include <PracticalCommon.h>
#include <system_error>
#include <thread>
#include <chrono>

using namespace std::chrono_literals;

int main(int argc, char* argv[])
{
	pipe_client client = pipe_client(PIPE_OPEN_REQUEST_NAME);
retryConnection:
	try
	{
		client.connect();
	}
	catch (std::system_error e)
	{
		goto retryConnection;
	}

	while (true)
	{
		const char* const msg = "Entire Message To Send";
		WriteFile(client.pipe_handle, msg, sizeof(msg), NULL, NULL);
		std::this_thread::sleep_for(50ms);
	}
}