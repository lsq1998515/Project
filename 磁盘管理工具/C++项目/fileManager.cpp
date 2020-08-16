#pragma once
#define _CRT_SECURE_NO_WARNINGS 1
#include "fileManger.h"
#include "fileutil.h"

//ɨ��Ŀ¼
void FileManager::scannerDir(const std::string& path)
{
	//��������
	_files.clear();
	searchDir(path, _files);
	//std::cout << "all files" << std::endl;
	//showAllFiles();
	getMd5toFiles();
	//showCopyList();
	getCopyList();
	//std::cout << "copy list " << std::endl;
	//showCopyList();
	//showAllFile()
}

void FileManager::getMd5toFiles()
{
	_md5toFiles.clear();
	for (const auto& f: _files)
	{
		//�������ļ�֮ǰ����
		_md5.reset();
		_md5toFiles.insert(make_pair(_md5.getFileMD5(f.c_str()), f));
	}
}

//ɨ�̣� ��ȡ�ظ����ļ� ֻ�����ظ��ļ��Ľ��
void FileManager::getCopyList()
{
	_filestoMd5.clear();
	//���÷�Χfor:����ɾ������ ������ʧЧ
	auto it = _md5toFiles.begin();  
	while (it != _md5toFiles.end())
	{
		//����ÿһ��md5��Ӧ�������ļ����
		if (_md5toFiles.count(it->first)>1)//md5ֵ��Ӧ����ļ�
		{
			//equal_range ����ֵ �� pair<beginIt,endIt>: -->[begin,end)-->����������ʱ����
			auto pairIt = _md5toFiles.equal_range(it->first);
			auto begin = pairIt.first;
			while (begin != pairIt.second)
			{
				//ֻɾ���ظ��ļ���ӳ���ϵ
				_filestoMd5.insert(make_pair(begin->second, begin->first));
				++begin;
			}
			//��һ����ͬmd5ֵ����ʼλ�� ���µ�����
			it = pairIt.second;
		}
		else
		{
			_files.erase(it->second);
			//erase����ֵ����ɾ��Ԫ�ص���һ��λ��
			it = _md5toFiles.erase(it);
		}
	}
}



//��������ɾ������Ҫ��֤һ���ļ������ڸ���

//�����ļ���ɾ��,��������ָ�����ļ�
void FileManager::deleteByName(const std::string& name)
{
	if (_filestoMd5.count(name) == 0)
	{
		std::cout << name << " not exist! " << std::endl;
		return;
	}
	//�õ��ļ���Ӧ��md5
	std::string curMD5 = _filestoMd5[name];
	std::cout << name << "--->" << _md5toFiles.count(curMD5) << std::endl;
	auto pairIt = _md5toFiles.equal_range(curMD5);
	auto  curIt = pairIt.first;
	int count = 0;
	while (curIt != pairIt.second)
	{
		if (curIt->second != name)
		{
			_filestoMd5.erase(curIt->second);
			_files.erase(curIt->second);
			deleteFile(curIt->second.c_str());
			++count;
		}
		++curIt;
	}
	//����
	curIt = pairIt.first;
	while (curIt != pairIt.second)
	{
		if (curIt->second != name)
		{
			//key --->MD5
			_md5toFiles.erase(curIt);
			pairIt = _md5toFiles.equal_range(curMD5);
			curIt = pairIt.first;
		}
		++curIt;
	}
	std::cout << "delete files :" << count << std::endl;
}


//����MD5ֵ����ɾ�� //�����汾
void FileManager::deleteByMD5(const std::string& md5)
{
	//md5  --> files
	if (_md5toFiles.count(md5) == 0)//������
	{
		std::cout<<md5 << " not exist! " << std::endl;
		return;
	}
	//ɾ������Ҫ����һ�ݣ�������һ���ļ�
	auto paitIt = _md5toFiles.equal_range(md5);
	std::cout << md5 << "--->" <<_md5toFiles.count(md5) << std::endl;
	auto curIt = paitIt.first;
	++curIt;
	int count = 0;
	while (curIt != paitIt.second)
	{
		_files.erase(curIt->second);
		_filestoMd5.erase(curIt->second);
		//_md5toFiles.erase(curIt);//������ʧЧ ���׳���
		//�ļ��Ӵ˴�����ɾ��
		deleteFile(curIt->second.c_str());
		++curIt;
		++count;
	}
	//����md5 ---> files
	curIt = paitIt.first;
	++curIt;
	//erase(first,last) -- >ɾ������ֵ��first,last)
	_md5toFiles.erase(curIt, paitIt.second);
	std::cout <<  "delete files :" << count<< std::endl;
}

void FileManager::deleteByMD5V2(const std::string& md5)
{
	//md5  --> files
	if (_md5toFiles.count(md5) == 0)//������
	{
		std::cout << md5 << " not exist! " << std::endl;
		return;
	}
	auto it = _md5toFiles.find(md5);
	deleteByName(it->second);
}


//ɾ�������ļ��ĸ���
//ÿ���ظ��ļ�ֻ����һ��
void FileManager::deleteAllCopy()
{
	//����MD5����
	std::unordered_set<std::string> md5set;
	//�ҳ����е�md5ֵ
	for (const auto& p : _md5toFiles)
	{
		md5set.insert(p.first);
	}
	for (const auto& md5 : md5set)
	{
		deleteByMD5(md5);
	}
}

//ģ��ɾ���� ɾ������ģ��ƥ��matchName�����ļ��ĸ���
void FileManager::deleteByMatchName(const std::string& matchName)
{
	//�������е��ļ�
	std::unordered_set<std::string> allFiles;
	for (const auto& f : _files)
	{
		if (f.find(matchName)!= std::string::npos)
		{
			allFiles.insert(f);
		}
	}
	//�����ļ���ɾ��
	for (const auto& f:allFiles)
	{
		if (_filestoMd5.count(f) != 0)
		{
			deleteByName(f);
		}
	}
}


void FileManager::showCopyList()
{
	auto it = _md5toFiles.begin();
	int total = _md5toFiles.size();
	int count = 0;
	while (it != _md5toFiles.end())
	{
		//md5��ͬ����ʾ��һ��
		int idx = 1;
		auto pairIt = _md5toFiles.equal_range(it->first);
		auto curIt = pairIt.first;
		std::cout << "cur MD5�� " << it->first << std::endl;
		while (curIt != pairIt.second)
		{
			std::cout << "��" << idx << "���ļ�";
			std::cout << curIt->second << std::endl;
			++idx;
			++curIt;
			count++; 
		}
		it = pairIt.second;
	}
	std::cout << "�ļ�������" << total << "\t" << count << std::endl;
}
void FileManager::showAllFile()
{
	for (const auto& f : _files)
	{
		std::cout << f << std::endl;
	}
	std::cout << "file count: " << _files.size() << std::endl;
}
