#pragma  once
#include <io.h>
#include<string>
#include <vector>
#include <unordered_set>
#include <iostream>

//path:Ŀ¼
//������ǰĿ¼������Щ�ļ�
void searchDir(const std::string& path, std::unordered_set<std::string>& subFiles);

//ɾ���ļ�
void deleteFile(const char* filename);
