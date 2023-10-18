#include "PracticalCommon.h"

std::string filePath;
FileMapData fileMap;
PracticalObject* sharedObject;
DWORD lastError = 0;

void done()
{
	sharedObject->dw_state = S_DONE;
}

bool joinObjectSharing()
{
	if (!fileExists(filePath.c_str()))
		return false;

	fileMap.h_map = OpenFileMappingA(FILE_MAP_ALL_ACCESS, FALSE, OBJECT_NAME);
	lastError = GetLastError();

	if (lastError || fileMap.h_map == INVALID_HANDLE_VALUE || fileMap.h_map == NULL)
		return false;

	sharedObject = (PracticalObject*)MapViewOfFile(fileMap.h_map, FILE_MAP_ALL_ACCESS, 0, 0, OBJECT_SIZE);
	lastError = GetLastError();

	if (lastError || sharedObject == NULL)
		return false;

	return true;
}

void work()
{
	Sleep(6000);
	sharedObject->dw_value = rand();
}

void endObjectSharing()
{
	UnmapViewOfFile(sharedObject);

	if (lastError || fileMap.h_map == INVALID_HANDLE_VALUE || fileMap.h_map == NULL)
		return;
	CloseHandle(fileMap.h_map);
}

int main()
{
	filePath = getTempDirectory() + '\\' + FILE_NAME;
	joinObjectSharing();
	work();
	done();
	endObjectSharing();
	return 0;
}