#pragma once
#include <windows.h>
#include <string>
#include <iostream>

#define OBJECT_SIZE sizeof(PracticalObject)
#define FILE_NAME "PRACTICALOBJECT.tmp"
#define OBJECT_NAME "PRACTICALOBJECTIDENTIFIER"
#define PO_IDENTIFIER 65535

enum WORKING_STATES
{
	S_WORKING,
	S_DONE,
	S_UNKERR
};

struct FileMapData
{
	void* p_object;
	HANDLE h_file;
	HANDLE h_map;
};

struct PracticalObject
{
	DWORD dw_indentifier;
	DWORD dw_state;
	DWORD dw_value;
	FileMapData* p_host;
};

bool fileExists(LPCSTR path)
{
	DWORD fileAttributes = GetFileAttributesA(path);
	return (fileAttributes != INVALID_FILE_ATTRIBUTES && !(fileAttributes & FILE_ATTRIBUTE_DIRECTORY));
}

std::string getUserDirectory()
{
#pragma warning(suppress: 4996)
	return std::string(getenv("USERPROFILE"));
}

std::string getTempDirectory()
{
	std::string userDirectory = getUserDirectory();
	userDirectory += "\\AppData\\local\\temp";
	return userDirectory;
}