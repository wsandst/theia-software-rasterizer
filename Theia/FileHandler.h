#pragma once
#include <fstream>
#include <string>
#include <filesystem>
#include <iostream>

#include "Object.h"
#include <vector>
#include "Face.h"

#include "Primitives.h"

namespace windows
{
#undef _MSC_EXTENSIONS
#define _WIN32_WINNT 0x0601
#include <windows.h>
}

using namespace std;

class FileHandler
{
public:
	static string getRunningDir();
	static Object loadObjectFromFile(string dir);
	static vector<string> split(string input, char delimiter);
private:
	FileHandler();
	~FileHandler();
};

