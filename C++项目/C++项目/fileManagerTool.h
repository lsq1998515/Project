#pragma  once
#include "fileManger.h"
#include <iostream>
using std::cout;
using std::cin;
using std::endl;

class FileManagerTool
{
public:
	void scanner();

	void deletebyMD5();

	void deletebyName();

	void deleteAllCopy();

	//Ä£ºýÆ¥Åä
	void  deleteFuzzyMatchCopy();

	void showCopy();

	void showAllFile();
private:
	FileManager _fm;
};