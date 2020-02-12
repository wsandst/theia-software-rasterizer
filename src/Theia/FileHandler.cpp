#include "FileHandler.h"



string FileHandler::getRunningDir()
{
	char buf[256];
	GetCurrentDirectoryA(256, buf);
	return string(buf);
}

vector<string> FileHandler::split(string input, char delimiter)
{
	vector<string> output;
	size_t foundSymbolAt = 0;
	size_t foundSymbolAtOld = -1;
	int i = 0;
	while (foundSymbolAt != -1)
	{
		foundSymbolAt = input.find(delimiter, (int)(foundSymbolAtOld + 1));
		if (input.substr(foundSymbolAtOld + 1, foundSymbolAt - foundSymbolAtOld - 1) != "")
			output.push_back(input.substr(foundSymbolAtOld + 1, foundSymbolAt - foundSymbolAtOld - 1));
		foundSymbolAtOld = foundSymbolAt;
		i++;
	}
	return output;
}

FileHandler::FileHandler()
{
}


FileHandler::~FileHandler()
{
}
