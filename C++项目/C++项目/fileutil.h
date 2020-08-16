#pragma  once
#include <io.h>
#include<string>
#include <vector>
#include <unordered_set>
#include <iostream>

//path:目录
//搜索当前目录都有哪些文件
void searchDir(const std::string& path, std::unordered_set<std::string>& subFiles);

//删除文件
void deleteFile(const char* filename);
