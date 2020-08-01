#define _CRT_SECURE_NO_WARNINGS 1
#pragma  once
#include "fileutil.h"


//path:目录
//搜索当前目录都有哪些文件
void searchDir(const std::string& path, std::unordered_set<std::string>& subFiles)
{
	std::string matchFile = path + "\\" + "*.*";
	_finddata_t fileAttr;//输出型参数
	long handle = _findfirst(matchFile.c_str(), &fileAttr);
	if (handle == -1)//-1 没找到
	{
		perror("search failed! ");
		std::cout << matchFile << std::endl;
		return;
	}
	//0 找到了 

	do
	{
		if (fileAttr.attrib & _A_SUBDIR)//目录
		{
			if (strcmp(fileAttr.name, ".") != 0 && strcmp(fileAttr.name, "..") != 0)
			{
				//当前为目录，继续搜索  递归调用
				searchDir(path + "\\" + fileAttr.name, subFiles);
			}
		}
		else//文件
		{
			subFiles.insert(path + "\\" + fileAttr.name);
		}
	} while (_findnext(handle, &fileAttr) == 0);
	_findclose(handle);
}



//删除文件
void deleteFile(const char* filename)
{
	if (remove(filename) == 0)//删除成功
	{
		std::cout << "delete file: " << filename << "success! " << std::endl;
	}
	else
	{
		perror("delete file failed! ");
	}
}