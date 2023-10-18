#include <Windows.h>
#include <iostream>
#include <string>

#define MAPPED_OBJECT_NAME L"ICTestMappedObject"
#define FILE_NAME L"ICTest"
#define FULL_FILE_PATH L"E:\\ICTest"
#define OBJECT_SIZE 128

HANDLE fileHandle;
HANDLE mappedFileHandle;
DWORD lastError;
void* object;
unsigned char* objectByteArray;

std::string getExecutableDirectory()
{
	char buffer[MAX_PATH];
	GetModuleFileNameA(NULL, buffer, MAX_PATH);
	std::string::size_type position = std::string(buffer).find_last_of("\\/");
	return std::string(buffer).substr(0, position);
}

bool fileExists(const wchar_t* path)
{
	DWORD fileAttributes = GetFileAttributes(path);
	return (fileAttributes != INVALID_FILE_ATTRIBUTES && !(fileAttributes & FILE_ATTRIBUTE_DIRECTORY));
}

bool startObjectSharing()
{
	if (fileExists(FULL_FILE_PATH))
		DeleteFile(FULL_FILE_PATH);

	fileHandle = CreateFile(FULL_FILE_PATH, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	lastError = GetLastError();

	if (fileHandle == INVALID_HANDLE_VALUE || fileHandle == NULL || lastError)
		return false;

	mappedFileHandle = CreateFileMapping(fileHandle, NULL, PAGE_READWRITE, 0, OBJECT_SIZE, MAPPED_OBJECT_NAME);
	lastError = GetLastError();

	if (mappedFileHandle == INVALID_HANDLE_VALUE || mappedFileHandle == NULL || lastError)
		return false;

	object = MapViewOfFile(mappedFileHandle, FILE_MAP_ALL_ACCESS, 0, 0, OBJECT_SIZE);
	lastError = GetLastError();

	if (object == NULL || lastError)
		return false;

	objectByteArray = static_cast<unsigned char*>(object);
	return true;
}

void testWrite()
{
	for (size_t i = 0; i < OBJECT_SIZE; i++)
	{
		objectByteArray[i] = i + 1;
	}
}

void watch()
{
	bool debuggerBreak = true;
	while (objectByteArray[OBJECT_SIZE - 1] != 0x00 && debuggerBreak);
}

void finishObjectSharing()
{
	if (fileHandle == INVALID_HANDLE_VALUE || fileHandle == NULL)
		return;

	UnmapViewOfFile(object);
	CloseHandle(fileHandle);

	if (mappedFileHandle == INVALID_HANDLE_VALUE || mappedFileHandle == NULL)
		return;
	CloseHandle(mappedFileHandle);

	DeleteFile(FULL_FILE_PATH);
}

int main()
{
	bool success = startObjectSharing();

	if (!success)
		return -1;

	testWrite();
	watch();

	finishObjectSharing();
	return 1;
}