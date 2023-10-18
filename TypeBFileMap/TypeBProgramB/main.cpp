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

bool fileExists(const wchar_t* path)
{
	DWORD fileAttributes = GetFileAttributes(path);
	return (fileAttributes != INVALID_FILE_ATTRIBUTES && !(fileAttributes & FILE_ATTRIBUTE_DIRECTORY));
}

int startObjectSharing()
{
	if (!fileExists(FULL_FILE_PATH))
		return false;

	//fileHandle = CreateFile(FULL_FILE_PATH, GENERIC_READ | GENERIC_WRITE, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	//lastError = GetLastError();

	//if (fileHandle == INVALID_HANDLE_VALUE || fileHandle == NULL || lastError)
	//	return false;

	mappedFileHandle = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, MAPPED_OBJECT_NAME);
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

void finishObjectSharing()
{
	if (fileHandle == INVALID_HANDLE_VALUE || fileHandle == NULL)
		return;

	UnmapViewOfFile(object);
	CloseHandle(fileHandle);

	if (mappedFileHandle == INVALID_HANDLE_VALUE || mappedFileHandle == NULL)
		return;
	CloseHandle(mappedFileHandle);
}

int main()
{
	bool success = startObjectSharing();

	if (!success)
		return -1;

	objectByteArray[OBJECT_SIZE - 1] = 0x00;

	finishObjectSharing();
	return 1;
}