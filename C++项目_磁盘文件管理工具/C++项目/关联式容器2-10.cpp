//#if 0
//
//
//#define _CRT_SECURE_NO_WARNINGS 1
//#include<iostream>
//#include<map>
//#include <functional>
//#include<unordered_map>
//#include<unordered_set>
//using namespace std;
//
////map  multimap:���Ҵ�ӡ���ǰ���Keyֵ��С����������е�
//void testMulMap()//��������
//{
//	multimap<char, int>mp;//keyֵ�����ظ�
//	mp.insert(make_pair('b', 20));
//	mp.insert(make_pair('b', 30));
//	mp.insert(make_pair('b', 40));
//	mp.insert(make_pair('b', 50));
//	mp.insert(make_pair('b', 60));
//	mp.insert(make_pair('a', 10));
//	mp.insert(make_pair('c', 70));
//	cout << "multimap" << endl;
//	for (const auto& p : mp)
//	{
//		cout << p.first << "--->" << p.second << endl;
//	}
//	//ret��������Χ�� b-->20, ....., c-->70 ����ҿ�
//	pair<multimap<char, int>::const_iterator, multimap<char, int>::const_iterator>ret = mp.equal_range('b');
//	auto ret1 = mp.equal_range('b');
//	cout << "key == b����Ԫ��" << endl;
//	auto begin = ret.first;
//	while (begin != ret.second)
//	{
//		cout << begin->first << "-->" << begin->second << endl;
//		++begin;
//	}
//	
//	cout << "find key==b" << endl;
//	//auto ret2 = mp.find('b');
//	multimap<char,int>::iterator ret2 = mp.find('b');
//	cout << ret2->first << "-->" << ret2->second << endl;
//}
//
//void testMap()//��������
//{
//	map<char, int> mp;//keyֵ�����ظ�
//	mp.insert(make_pair('b', 20));//��Ч����
//	mp.insert(make_pair('b', 30));
//	mp.insert(make_pair('b', 40));
//	mp.insert(make_pair('b', 50));
//	mp.insert(make_pair('b', 60));
//	mp.insert(make_pair('a', 10));//��Ч����
//	mp.insert(make_pair('c', 70));//��Ч����    key�����ظ�
//	cout << "map" << endl;
//	for (const auto& p : mp)
//	{
//		cout << p.first << "--->" << p.second << endl;
//	}
//}
//
//void testMap2()
//{
//	//������������ �º������ͣ��ṩ�ȽϹ���
//	map<char, int, greater<char>> mp;//���������key�Ӵ�С
//	mp.insert(make_pair('b', 20));
//	mp.insert(make_pair('b', 30));
//	mp.insert(make_pair('b', 40));
//	mp.insert(make_pair('b', 50));
//	mp.insert(make_pair('b', 60));
//	mp.insert(make_pair('a', 10));
//	mp.insert(make_pair('c', 70));
//	cout << "map" << endl;
//	for (const auto& p : mp)
//	{
//		cout << p.first << "--->" << p.second << endl;
//	}
//}
//
//
//void testUnordMulMap()//����������
//{
//	unordered_multimap<char, int>mp;//keyֵ�����ظ�
//	mp.insert(make_pair('b', 20));
//	mp.insert(make_pair('b', 30));
//	mp.insert(make_pair('b', 40));
//	mp.insert(make_pair('b', 50));
//	mp.insert(make_pair('b', 60));
//	mp.insert(make_pair('a', 10));
//	mp.insert(make_pair('c', 70));
//	cout << "UnordMultimap" << endl;
//	for (const auto& p : mp)
//	{
//		cout << p.first << "--->" << p.second << endl;
//	}
//}
//
//void testUnordMap()//��������
//{
//	unordered_map<char, int> mp;//keyֵ�����ظ�
//	mp.insert(make_pair('b', 20));//��Ч����
//	mp.insert(make_pair('b', 30));
//	mp.insert(make_pair('b', 40));
//	mp.insert(make_pair('b', 50));
//	mp.insert(make_pair('b', 60));
//	mp.insert(make_pair('a', 10));//��Ч����
//	mp.insert(make_pair('c', 70));//��Ч����    key�����ظ�
//	cout << "UnordMap" << endl;
//	for (const auto& p : mp)
//	{
//		cout << p.first << "--->" << p.second << endl;
//	}
//}
//
//int main()
//{
//	//cout << "key��С�������" << endl;
//	//testMap();
//
//	//testMulMap();
//
//	//cout << "key�Ӵ�С���" << endl;
//	//testMap2();
//
//	//testMap();//����
//	//testUnordMap();//����
//
//	testMulMap();//������ظ�
//	testUnordMulMap();//������ظ�
//	
//	system("pause");
//	return 0;
//}
//
//#endif