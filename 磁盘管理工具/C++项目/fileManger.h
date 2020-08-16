#pragma  once
#include<string>
#include "md5.h"
#include <unordered_set>
#include <unordered_map>

class FileManager
{
public:
	//扫描目录
	void scannerDir(const std::string& path);

	//扫盘： 获取重复的文件
	void getCopyList();
	void getMd5toFiles();

	//所有删除，都要保证一个文件不存在副本

	//按照文件名删除
	void deleteByName(const std::string& name);

	//按照MD5值进行删除
	void deleteByMD5(const std::string& md5);
	void deleteByMD5V2(const std::string& md5);

	//删除所有文件的副本
	void deleteAllCopy();

	//模糊删除： 删除所有模糊匹配matchName所有文件的副本
	void deleteByMatchName(const std::string& matchName);


	//测试 无用
	void showCopyList();
	void showAllFile();


private:
	//vector,list, map,set
	std::unordered_set<std::string> _files;//扫描所有文件保存的容器

	//MD5 --> File   MD5相同名字可能不同
	//unordered_muitmap: 允许存在相同的元素
	std::unordered_multimap<std::string, std::string> _md5toFiles;

	//File -- > MD5  一个文件夹下不会存在文件名完全相同的文件
	//unordered_map  : 不存在相同的元素
	std::unordered_map<std::string, std::string> _filestoMd5;

	MD5 _md5;
};