#include "PracticalCommon.h"

std::string filePath;
PracticalObject* practicalObject;
FileMapData fileMap;
DWORD lastError = 0;
byte* objectByteArray;

bool beginFileMap()
{
	if (fileExists(filePath.c_str()))
		DeleteFileA(filePath.c_str());

	fileMap.h_file = CreateFileA(filePath.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	lastError = GetLastError();

	if (lastError || fileMap.h_file == NULL || fileMap.h_file == INVALID_HANDLE_VALUE)
		return false;

	fileMap.h_map = CreateFileMappingA(fileMap.h_file, NULL, PAGE_READWRITE, 0, OBJECT_SIZE, OBJECT_NAME);
	lastError = GetLastError();

	if (lastError || fileMap.h_map == NULL || fileMap.h_map == INVALID_HANDLE_VALUE)
		return false;

	fileMap.p_object = MapViewOfFile(fileMap.h_map, FILE_MAP_ALL_ACCESS, 0, 0, OBJECT_SIZE);
	practicalObject = (PracticalObject*)fileMap.p_object;
	practicalObject->dw_indentifier = PO_IDENTIFIER;
	practicalObject->p_host = &fileMap;
	objectByteArray = (byte*)practicalObject;

	return true;
}

void wait()
{
	while (practicalObject->dw_state == S_WORKING);
}

void endFileMap()
{
	if (fileMap.h_file == INVALID_HANDLE_VALUE || fileMap.h_file == NULL)
		return;
	UnmapViewOfFile(fileMap.p_object);
	CloseHandle(fileMap.h_file);

	if (fileMap.h_map == INVALID_HANDLE_VALUE || fileMap.h_map == NULL)
		return;
	CloseHandle(fileMap.h_map);

	DeleteFileA(filePath.c_str());
}

int main()
{
	filePath = getTempDirectory() + '\\' + FILE_NAME;
	if (!beginFileMap())
		return -1;

	wait();

	std::cout << "Other process is done with the value: " << practicalObject->dw_value << '\n';

	endFileMap();
}