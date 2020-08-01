#pragma  once
#define _CRT_SECURE_NO_WARNINGS 1
#define CHUNK_BYTE 64
#include<string>
#include <iostream>
typedef unsigned int uint32;


class MD5{
public:

	/*
	F(x,y,z) = (x & y) | ((~x) & z)
	G(x,y,z) = (x & z) | ( y & (~z))
	H(x,y,z) = x ^ y ^ z
	I(x,y,z) = y ^ (x | (~z))
	*/ 
	//位运算函数 F G H I   内敛函数
	uint32 F(uint32 b, uint32 c, uint32 d)
	{
		return (b & c) | ((~b) & d);
	}

	uint32 G(uint32 b, uint32 c, uint32 d)
	{
		return (b & d) | (c&(~d));
	}
	uint32 H(uint32 b, uint32 c, uint32 d)
	{
		return b^c^d;
	}
	uint32 I(uint32 b, uint32 c, uint32 d)
	{
		return c ^ (b | (~d));
	}

	//循环左移  左移出去的加在右边
	uint32 leftShift(uint32 number, int shiftNumber)
	{
		return (number<< shiftNumber) | (number >> (32 - shiftNumber));
	}

	MD5();//无参构造

	void Init();//初始化

	void reset();//重置


	//一个chunk的MD5计算
	void calMD5(uint32* chunk);
	

	//进行填充，在进行MD5计算
	void calFinalMD5();

	//把一个整形数据转换为对应的16进制字符串
	std::string changeHex(uint32 n);

	std::string getStringMD5(const std::string& str);
	std::string getFileMD5(const char* filePath);

private:
	//循环移位位数->数组 64元素
	static int _leftShift[64];//静态共享,类外初始化

	//K[i]:K[i] = floor(2^(32) * abs(sin(i + 1)))
	uint32 _k[64];

	//数据块：64字节
	char _chunk[CHUNK_BYTE];


	//填充相关的变量  
	//最后一块数据的字节数
	uint32 _lastByte;


	//总字节数
	uint32 _totalByte;

	//MD5信息  A,B,C,D
	uint32 _a;
	uint32 _b;
	uint32 _c;
	uint32 _d;
};


//不能再这里进行static成员的初始化，
//因为当多个.cc,cpp文件包含这个头文件时，会导致static成员的多次初始化
//int MD5::_leftshift[64]={初始化}


