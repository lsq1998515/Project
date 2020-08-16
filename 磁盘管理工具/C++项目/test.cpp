#pragma  once
#define _CRT_SECURE_NO_WARNINGS 1
#include "md5.h"
#include "fileutil.h"
#include "fileManger.h"
#include "fileManagerTool.h"
using std::cout;
using std::endl;

void testHex()
{
	MD5 md5;
	uint32 n = 0x23456789;
	cout << md5.changeHex(n) << endl;
}

void testStringMD5()
{
	MD5 md5;
	//雪崩效应
	std::string str = "123456789abcdefgh";
	cout << md5.getStringMD5(str) << endl;
	std::string str2 = "123456789abcdefgi";
	cout << md5.getStringMD5(str2) << endl;
}

void testFileMd5()
{
	MD5 md5;
	const char* filepath1 = "md5test1.txt";
	const char* filepath2 = "md5test2.txt";
	cout << "md5test1.txt  "<<md5.getFileMD5(filepath1) << endl;
	//计算下一个文件需要重置
	md5.reset();//必须重置
	cout << "md5test2.txt  "<<md5.getFileMD5(filepath2) << endl;
}

//void testSearch()
//{
//	std::string path;
//	cout << "please input search dir :" << endl;
//	getline(std::cin, path);
//	std::vector<std::string>subFiles;
//	searchDir(path, subFiles);
//	for (const auto& f:subFiles)
//	{
//		cout << f << endl;
//	}
//}

void menu()
{
	cout << "**************************************************" << endl;
	cout << "**  1.scanner      2.show all    3.show sopy    **" << endl;
	cout << "**  4.delete name  5.delete md5  6.delete all   **" << endl;
	cout << "**  7.delete matched             0.exit         **" << endl;
	cout << "**************************************************"<<endl;
}

void testFileManagerTool()
{
	FileManagerTool fmt;
	int input;
	do 
	{
		menu();
		std::string garbage;
		cout << "请输入选项： " << endl;
		cin >> input;
		getline(cin, garbage);
		switch (input)
		{
			case  1:
				fmt.scanner();
				break;
			case 2:
				fmt.showAllFile();
				break;
			case 3:
				fmt.showCopy();
				break;
			case 4:
				fmt.deletebyName();
				break;
			case 5:
				fmt.deletebyMD5();
				break;
			case 6:
				fmt.deleteAllCopy();
				break;
			case 7:
				fmt.deleteFuzzyMatchCopy();
			case  0:
				break;

			default:
				break;
		}	
	} while (input != 0);
}


int main()
{
	//testHex();
	//testStringMD5();
	//testFileMd5();
	//testSearch();
	testFileManagerTool();
	system("pause");
	return 0;
}
