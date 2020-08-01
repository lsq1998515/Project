#define _CRT_SECURE_NO_WARNINGS 1
#pragma  once
#include "fileutil.h"


//path:Ŀ¼
//������ǰĿ¼������Щ�ļ�
void searchDir(const std::string& path, std::unordered_set<std::string>& subFiles)
{
	std::string matchFile = path + "\\" + "*.*";
	_finddata_t fileAttr;//����Ͳ���
	long handle = _findfirst(matchFile.c_str(), &fileAttr);
	if (handle == -1)//-1 û�ҵ�
	{
		perror("search failed! ");
		std::cout << matchFile << std::endl;
		return;
	}
	//0 �ҵ��� 

	do
	{
		if (fileAttr.attrib & _A_SUBDIR)//Ŀ¼
		{
			if (strcmp(fileAttr.name, ".") != 0 && strcmp(fileAttr.name, "..") != 0)
			{
				//��ǰΪĿ¼����������  �ݹ����
				searchDir(path + "\\" + fileAttr.name, subFiles);
			}
		}
		else//�ļ�
		{
			subFiles.insert(path + "\\" + fileAttr.name);
		}
	} while (_findnext(handle, &fileAttr) == 0);
	_findclose(handle);
}



//ɾ���ļ�
void deleteFile(const char* filename)
{
	if (remove(filename) == 0)//ɾ���ɹ�
	{
		std::cout << "delete file: " << filename << "success! " << std::endl;
	}
	else
	{
		perror("delete file failed! ");
	}
}