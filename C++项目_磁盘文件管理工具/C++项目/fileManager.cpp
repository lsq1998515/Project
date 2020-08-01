#pragma once
#define _CRT_SECURE_NO_WARNINGS 1
#include "fileManger.h"
#include "fileutil.h"

//扫描目录
void FileManager::scannerDir(const std::string& path)
{
	//清理容器
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
		//计算新文件之前重置
		_md5.reset();
		_md5toFiles.insert(make_pair(_md5.getFileMD5(f.c_str()), f));
	}
}

//扫盘： 获取重复的文件 只保留重复文件的结果
void FileManager::getCopyList()
{
	_filestoMd5.clear();
	//不用范围for:涉嫌删除操作 迭代器失效
	auto it = _md5toFiles.begin();  
	while (it != _md5toFiles.end())
	{
		//查找每一个md5对应的所有文件结果
		if (_md5toFiles.count(it->first)>1)//md5值对应多个文件
		{
			//equal_range 返回值 ： pair<beginIt,endIt>: -->[begin,end)-->迭代器遍历时连续
			auto pairIt = _md5toFiles.equal_range(it->first);
			auto begin = pairIt.first;
			while (begin != pairIt.second)
			{
				//只删除重复文件的映射关系
				_filestoMd5.insert(make_pair(begin->second, begin->first));
				++begin;
			}
			//下一个不同md5值的起始位置 更新迭代器
			it = pairIt.second;
		}
		else
		{
			_files.erase(it->second);
			//erase返回值：被删除元素的下一个位置
			it = _md5toFiles.erase(it);
		}
	}
}



//下面所有删除，都要保证一个文件不存在副本

//按照文件名删除,保留的是指定的文件
void FileManager::deleteByName(const std::string& name)
{
	if (_filestoMd5.count(name) == 0)
	{
		std::cout << name << " not exist! " << std::endl;
		return;
	}
	//拿到文件对应的md5
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
	//更新
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


//按照MD5值进行删除 //两个版本
void FileManager::deleteByMD5(const std::string& md5)
{
	//md5  --> files
	if (_md5toFiles.count(md5) == 0)//不存在
	{
		std::cout<<md5 << " not exist! " << std::endl;
		return;
	}
	//删除是需要保留一份，保留第一个文件
	auto paitIt = _md5toFiles.equal_range(md5);
	std::cout << md5 << "--->" <<_md5toFiles.count(md5) << std::endl;
	auto curIt = paitIt.first;
	++curIt;
	int count = 0;
	while (curIt != paitIt.second)
	{
		_files.erase(curIt->second);
		_filestoMd5.erase(curIt->second);
		//_md5toFiles.erase(curIt);//迭代器失效 容易出错
		//文件从此磁盘中删除
		deleteFile(curIt->second.c_str());
		++curIt;
		++count;
	}
	//更新md5 ---> files
	curIt = paitIt.first;
	++curIt;
	//erase(first,last) -- >删除区域值【first,last)
	_md5toFiles.erase(curIt, paitIt.second);
	std::cout <<  "delete files :" << count<< std::endl;
}

void FileManager::deleteByMD5V2(const std::string& md5)
{
	//md5  --> files
	if (_md5toFiles.count(md5) == 0)//不存在
	{
		std::cout << md5 << " not exist! " << std::endl;
		return;
	}
	auto it = _md5toFiles.find(md5);
	deleteByName(it->second);
}


//删除所有文件的副本
//每个重复文件只保留一个
void FileManager::deleteAllCopy()
{
	//先拿MD5集合
	std::unordered_set<std::string> md5set;
	//找出所有的md5值
	for (const auto& p : _md5toFiles)
	{
		md5set.insert(p.first);
	}
	for (const auto& md5 : md5set)
	{
		deleteByMD5(md5);
	}
}

//模糊删除： 删除所有模糊匹配matchName所有文件的副本
void FileManager::deleteByMatchName(const std::string& matchName)
{
	//遍历所有的文件
	std::unordered_set<std::string> allFiles;
	for (const auto& f : _files)
	{
		if (f.find(matchName)!= std::string::npos)
		{
			allFiles.insert(f);
		}
	}
	//按照文件名删除
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
		//md5相同的显示在一起
		int idx = 1;
		auto pairIt = _md5toFiles.equal_range(it->first);
		auto curIt = pairIt.first;
		std::cout << "cur MD5； " << it->first << std::endl;
		while (curIt != pairIt.second)
		{
			std::cout << "第" << idx << "个文件";
			std::cout << curIt->second << std::endl;
			++idx;
			++curIt;
			count++; 
		}
		it = pairIt.second;
	}
	std::cout << "文件总数：" << total << "\t" << count << std::endl;
}
void FileManager::showAllFile()
{
	for (const auto& f : _files)
	{
		std::cout << f << std::endl;
	}
	std::cout << "file count: " << _files.size() << std::endl;
}
