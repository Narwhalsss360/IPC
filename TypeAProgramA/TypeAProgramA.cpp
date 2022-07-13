#include <Windows.h>

HANDLE portHandle;
COMSTAT comStatus;

int getQueue()
{
	ClearCommError(portHandle, nullptr, &comStatus);
	return comStatus.cbInQue;
}

int setup()
{
	portHandle = CreateFileA("COM8", GENERIC_READ | GENERIC_WRITE, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	PurgeComm(portHandle, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXABORT | PURGE_TXCLEAR);
	if (portHandle == INVALID_HANDLE_VALUE)
		return 1;
	PurgeComm(portHandle, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXABORT | PURGE_TXCLEAR);
	if (portHandle == INVALID_HANDLE_VALUE)
		return 2;
	DCB dcb;
	SecureZeroMemory(&dcb, sizeof(DCB));
	if (!GetCommState(portHandle, &dcb))
		return 3;
	dcb.BaudRate = 1000000;
	dcb.ByteSize = 8;
	dcb.Parity = 0;
	dcb.StopBits = ONESTOPBIT;
	if (!SetCommState(portHandle, &dcb))
		return 4;
	PurgeComm(portHandle, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXABORT | PURGE_TXCLEAR);
}

int main()
{
	int result = setup();

	CloseHandle(portHandle);
	return 0;
}