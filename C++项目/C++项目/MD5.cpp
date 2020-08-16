#pragma  once
#define _CRT_SECURE_NO_WARNINGS 1
#include "md5.h"
#include<iostream>
#include <math.h>
#include <string.h>
#include <fstream>

//初始化static成员
int MD5::_leftShift[64] = { 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7,12, 17, 22, 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20,
4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 6, 10,15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21 };

MD5::MD5()//无参构造
{
	Init();
}

void MD5::Init()
{
	//K[i]:K[i] = floor(2^(32) * abs(sin(i + 1)))
	//初始化K[i]
	for (int i = 0; i < 64; ++i)
	{
		//static_cast是一个c++运算符，功能是把一个表达式转换为某种类型，
		//但没有运行时类型检查来保证转换的安全性。
		_k[i] = static_cast<uint32>(abs(sin(i + 1.0))*pow(2.0, 32));
	}

	//初始化四个整形变量ABCD
	reset();
}





void MD5::reset()
{
	//初始化ABCD 固定值
	_a = 0x67452301;
	_b = 0xefcdab89;
	_c = 0x98badcfe;
	_d = 0x10325476;

	//初始化
	memset(_chunk, 0, CHUNK_BYTE);

	_totalByte = _lastByte = 0;
}
 



//一个chunk的MD5计算
void MD5::calMD5(uint32* chunk)
{
	int a = _a;
	int b = _b;
	int c = _c;
	int d = _d;
	int f, g;
	//4byte为一个处理单元
	//共执行64次
	/*
	if (0 <= i < 16) g = i;
	if (16 <= i < 32) g = (5 * i + 1) % 16;
	if (32 <= i < 48) g = (3 * i + 5) % 16;
	if(48 <= i < 63) g = (7 * i) % 16;
	*/
	for (int i = 0; i < 64;++i)
	{
		
		//位运算 F，G，H，I
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
		 //48  --- 63 ： I
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
	//更新A,B,C，D
	_a = _a + a;
	_b = _b + b;
	_c = _c + c;
	_d = _d + d;
}


//进行填充，在进行MD5计算
void MD5::calFinalMD5()
{
	//填充冗余信息：第一bit位填1，剩余填0
	//任何情况下都需要至少填充1字节byte的冗余信息

	//获取第一个待填充的位置
	char* p=_chunk + _lastByte;
	//首先填充1byte的冗余信息：1000 0000
	*p++ = 0x80; //或者-128
	int remainByte=CHUNK_BYTE-_lastByte-1;
	//如果剩余字节数不够8Byte--->64bit，不能够填充长度信息
	//先处理一块数据，再去构建一个新的数据块，前448填0，最后面64
	//位原64位长度信息
	if (remainByte < 8)  
	{
		//剩余位全部补0处理一次
		memset(p, 0, remainByte);
		calMD5((uint32*)_chunk);
		//构建一个新的数据块
		memset(_chunk, 0, CHUNK_BYTE);
	}
	else
	{
		//剩余位全部给0
		memset(p, 0, remainByte);
	}
	//给数据块最后64位填充原始长度信息
	unsigned long long totalBits = _totalByte;//long long 类型8字节
	totalBits *= 8;
	((unsigned long long *)_chunk)[7] = totalBits;
	calMD5((uint32*)_chunk);
}


/*
int -->16进制字符串
例如：0x23456789-->"89674523"
	1.获取每一个字节的数据（char*指针，位操作）
	2.每4个bit位转换一个16进制字符（数组与16进制字符映射： [十位数/16]  [个位数%16]）
	3.拼接字符
*/
std::string MD5::changeHex(uint32 n)
{
	static std::string strMap = "0123456789abcdef";
	std::string ret;
	//获取一个字节数据
	for (int i = 0; i < 4; ++i)
	{
		//获取一个字节的数据
		int  curByte = (n >> (i * 8)) & 0xff;
		//数据转换16进制
		std::string curRet;
		//除16获取高位   ，模16获取低位  字节内不逆序
		curRet += strMap[curByte / 16];
		curRet += strMap[curByte % 16];
		//字节之间逆序
		ret += curRet;
	}
	return ret;
} 


std::string MD5::getStringMD5(const std::string& str)
{
	//为空
	if (str.empty())
	{
		return changeHex(_a).append(changeHex(_b)).append(changeHex(_c)).append(changeHex(_d));
	}
	//不为空
	_totalByte = str.size();
	uint32 chunkNum = _totalByte / CHUNK_BYTE;
	const char* strPtr = str.c_str();
	for (int i = 0; i < chunkNum; ++i)
	{
		memcpy(_chunk, strPtr+CHUNK_BYTE*i, CHUNK_BYTE);
		calMD5((uint32*)_chunk);
	}
	//计算最后一块数据： 需要填充
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
	//正常打开
	while (!fin.eof())
	{

		//1.全部读进来 seekg, tellg获取文件大小 空间换时间，时间效率高  适合小文件
		/*fin.seekg(0,fin.end);
		uint32 length = fin.tellg();
		fin.seekg(0, fin.beg);
		char* totalData = new char(length);
		fin.read(totalData, length);*/

		//2.每次只读取一块数据： 时间换空间   适合大文件
		fin.read(_chunk,CHUNK_BYTE );//每次读64字节内容，应该检查下可以读够吗?
		//是否读取到64字节内容
		if (CHUNK_BYTE != fin.gcount())//gcount : 上次操作读取了多少字节数
		{
			//说明为最后一块数据
			break;
		}
		_totalByte += CHUNK_BYTE;
		calMD5((uint32*)_chunk);
	}
	//gcount : 可以调用多次 ，始终返回最近一次读入的字节数
	_lastByte = fin.gcount();
	_totalByte += _lastByte;
	calFinalMD5();
	return changeHex(_a).append(changeHex(_b)).append(changeHex(_c)).append(changeHex(_d));
}