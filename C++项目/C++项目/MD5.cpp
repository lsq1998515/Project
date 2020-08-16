#pragma  once
#define _CRT_SECURE_NO_WARNINGS 1
#include "md5.h"
#include<iostream>
#include <math.h>
#include <string.h>
#include <fstream>

//��ʼ��static��Ա
int MD5::_leftShift[64] = { 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7,12, 17, 22, 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20,
4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 6, 10,15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21 };

MD5::MD5()//�޲ι���
{
	Init();
}

void MD5::Init()
{
	//K[i]:K[i] = floor(2^(32) * abs(sin(i + 1)))
	//��ʼ��K[i]
	for (int i = 0; i < 64; ++i)
	{
		//static_cast��һ��c++������������ǰ�һ�����ʽת��Ϊĳ�����ͣ�
		//��û������ʱ���ͼ������֤ת���İ�ȫ�ԡ�
		_k[i] = static_cast<uint32>(abs(sin(i + 1.0))*pow(2.0, 32));
	}

	//��ʼ���ĸ����α���ABCD
	reset();
}





void MD5::reset()
{
	//��ʼ��ABCD �̶�ֵ
	_a = 0x67452301;
	_b = 0xefcdab89;
	_c = 0x98badcfe;
	_d = 0x10325476;

	//��ʼ��
	memset(_chunk, 0, CHUNK_BYTE);

	_totalByte = _lastByte = 0;
}
 



//һ��chunk��MD5����
void MD5::calMD5(uint32* chunk)
{
	int a = _a;
	int b = _b;
	int c = _c;
	int d = _d;
	int f, g;
	//4byteΪһ������Ԫ
	//��ִ��64��
	/*
	if (0 <= i < 16) g = i;
	if (16 <= i < 32) g = (5 * i + 1) % 16;
	if (32 <= i < 48) g = (3 * i + 5) % 16;
	if(48 <= i < 63) g = (7 * i) % 16;
	*/
	for (int i = 0; i < 64;++i)
	{
		
		//λ���� F��G��H��I
		//0 --- 15 : F
		if ( 0 <= i && i <= 15)
		{
			 f = F(b, c, d);
			 g = i;
		}
		//16 --- 31 : G
		 else if (16 <= i && i <= 31)
		{
			 f = G(b, c, d);
			 g = (5 * i + 1) % 16;
		}
		//32 --- 47 : H
		 else if (32 <= i && i <= 47)
		 {
			 f = H(b, c, d);
			 g = (3 * i + 5) % 16;
		 }
		 //48  --- 63 �� I
		 else
		 {
			 f = I(b, c, d);
			 g = (7 * i) % 16;
		 }
		 int tmp = d;
		 d = c;
		 c = b;
		 b = b + leftShift((a + f + _k[i] + chunk[g]), _leftShift [i]);
		 a = tmp;
	}
	/*
		A = a + A;
		B = b + B;
		C = c + C;
		D = d + D;
	*/
	//����A,B,C��D
	_a = _a + a;
	_b = _b + b;
	_c = _c + c;
	_d = _d + d;
}


//������䣬�ڽ���MD5����
void MD5::calFinalMD5()
{
	//���������Ϣ����һbitλ��1��ʣ����0
	//�κ�����¶���Ҫ�������1�ֽ�byte��������Ϣ

	//��ȡ��һ��������λ��
	char* p=_chunk + _lastByte;
	//�������1byte��������Ϣ��1000 0000
	*p++ = 0x80; //����-128
	int remainByte=CHUNK_BYTE-_lastByte-1;
	//���ʣ���ֽ�������8Byte--->64bit�����ܹ���䳤����Ϣ
	//�ȴ���һ�����ݣ���ȥ����һ���µ����ݿ飬ǰ448��0�������64
	//λԭ64λ������Ϣ
	if (remainByte < 8)  
	{
		//ʣ��λȫ����0����һ��
		memset(p, 0, remainByte);
		calMD5((uint32*)_chunk);
		//����һ���µ����ݿ�
		memset(_chunk, 0, CHUNK_BYTE);
	}
	else
	{
		//ʣ��λȫ����0
		memset(p, 0, remainByte);
	}
	//�����ݿ����64λ���ԭʼ������Ϣ
	unsigned long long totalBits = _totalByte;//long long ����8�ֽ�
	totalBits *= 8;
	((unsigned long long *)_chunk)[7] = totalBits;
	calMD5((uint32*)_chunk);
}


/*
int -->16�����ַ���
���磺0x23456789-->"89674523"
	1.��ȡÿһ���ֽڵ����ݣ�char*ָ�룬λ������
	2.ÿ4��bitλת��һ��16�����ַ���������16�����ַ�ӳ�䣺 [ʮλ��/16]  [��λ��%16]��
	3.ƴ���ַ�
*/
std::string MD5::changeHex(uint32 n)
{
	static std::string strMap = "0123456789abcdef";
	std::string ret;
	//��ȡһ���ֽ�����
	for (int i = 0; i < 4; ++i)
	{
		//��ȡһ���ֽڵ�����
		int  curByte = (n >> (i * 8)) & 0xff;
		//����ת��16����
		std::string curRet;
		//��16��ȡ��λ   ��ģ16��ȡ��λ  �ֽ��ڲ�����
		curRet += strMap[curByte / 16];
		curRet += strMap[curByte % 16];
		//�ֽ�֮������
		ret += curRet;
	}
	return ret;
} 


std::string MD5::getStringMD5(const std::string& str)
{
	//Ϊ��
	if (str.empty())
	{
		return changeHex(_a).append(changeHex(_b)).append(changeHex(_c)).append(changeHex(_d));
	}
	//��Ϊ��
	_totalByte = str.size();
	uint32 chunkNum = _totalByte / CHUNK_BYTE;
	const char* strPtr = str.c_str();
	for (int i = 0; i < chunkNum; ++i)
	{
		memcpy(_chunk, strPtr+CHUNK_BYTE*i, CHUNK_BYTE);
		calMD5((uint32*)_chunk);
	}
	//�������һ�����ݣ� ��Ҫ���
	_lastByte = _totalByte  %  CHUNK_BYTE;
	memcpy(_chunk, strPtr + chunkNum*CHUNK_BYTE, _lastByte);
	calFinalMD5();
	return changeHex(_a).append(changeHex(_b)).append(changeHex(_c)).append(changeHex(_d));
}

std::string MD5::getFileMD5(const char* filePath)
{
	std::ifstream fin(filePath ,std::ifstream::binary);
	if (!fin.is_open())
	{
		std::cout << filePath;
		perror("file open failed!");
		return "";
	}
	//������
	while (!fin.eof())
	{

		//1.ȫ�������� seekg, tellg��ȡ�ļ���С �ռ任ʱ�䣬ʱ��Ч�ʸ�  �ʺ�С�ļ�
		/*fin.seekg(0,fin.end);
		uint32 length = fin.tellg();
		fin.seekg(0, fin.beg);
		char* totalData = new char(length);
		fin.read(totalData, length);*/

		//2.ÿ��ֻ��ȡһ�����ݣ� ʱ�任�ռ�   �ʺϴ��ļ�
		fin.read(_chunk,CHUNK_BYTE );//ÿ�ζ�64�ֽ����ݣ�Ӧ�ü���¿��Զ�����?
		//�Ƿ��ȡ��64�ֽ�����
		if (CHUNK_BYTE != fin.gcount())//gcount : �ϴβ�����ȡ�˶����ֽ���
		{
			//˵��Ϊ���һ������
			break;
		}
		_totalByte += CHUNK_BYTE;
		calMD5((uint32*)_chunk);
	}
	//gcount : ���Ե��ö�� ��ʼ�շ������һ�ζ�����ֽ���
	_lastByte = fin.gcount();
	_totalByte += _lastByte;
	calFinalMD5();
	return changeHex(_a).append(changeHex(_b)).append(changeHex(_c)).append(changeHex(_d));
}