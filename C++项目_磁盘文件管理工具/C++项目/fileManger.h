#pragma  once
#include<string>
#include "md5.h"
#include <unordered_set>
#include <unordered_map>

class FileManager
{
public:
	//ɨ��Ŀ¼
	void scannerDir(const std::string& path);

	//ɨ�̣� ��ȡ�ظ����ļ�
	void getCopyList();
	void getMd5toFiles();

	//����ɾ������Ҫ��֤һ���ļ������ڸ���

	//�����ļ���ɾ��
	void deleteByName(const std::string& name);

	//����MD5ֵ����ɾ��
	void deleteByMD5(const std::string& md5);
	void deleteByMD5V2(const std::string& md5);

	//ɾ�������ļ��ĸ���
	void deleteAllCopy();

	//ģ��ɾ���� ɾ������ģ��ƥ��matchName�����ļ��ĸ���
	void deleteByMatchName(const std::string& matchName);


	//���� ����
	void showCopyList();
	void showAllFile();


private:
	//vector,list, map,set
	std::unordered_set<std::string> _files;//ɨ�������ļ����������

	//MD5 --> File   MD5��ͬ���ֿ��ܲ�ͬ
	//unordered_muitmap: ���������ͬ��Ԫ��
	std::unordered_multimap<std::string, std::string> _md5toFiles;

	//File -- > MD5  һ���ļ����²�������ļ�����ȫ��ͬ���ļ�
	//unordered_map  : ��������ͬ��Ԫ��
	std::unordered_map<std::string, std::string> _filestoMd5;

	MD5 _md5;
};