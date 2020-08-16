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
////map  multimap:查找打印都是按照Key值从小到大进行排列的
//void testMulMap()//遍历有序
//{
//	multimap<char, int>mp;//key值可以重复
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
//	//ret迭代器范围： b-->20, ....., c-->70 左闭右开
//	pair<multimap<char, int>::const_iterator, multimap<char, int>::const_iterator>ret = mp.equal_range('b');
//	auto ret1 = mp.equal_range('b');
//	cout << "key == b所有元素" << endl;
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
//void testMap()//遍历有序
//{
//	map<char, int> mp;//key值不可重复
//	mp.insert(make_pair('b', 20));//有效插入
//	mp.insert(make_pair('b', 30));
//	mp.insert(make_pair('b', 40));
//	mp.insert(make_pair('b', 50));
//	mp.insert(make_pair('b', 60));
//	mp.insert(make_pair('a', 10));//有效插入
//	mp.insert(make_pair('c', 70));//有效插入    key不可重复
//	cout << "map" << endl;
//	for (const auto& p : mp)
//	{
//		cout << p.first << "--->" << p.second << endl;
//	}
//}
//
//void testMap2()
//{
//	//第三个参数： 仿函数类型，提供比较规则。
//	map<char, int, greater<char>> mp;//中序遍历，key从大到小
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
//void testUnordMulMap()//遍历：无序
//{
//	unordered_multimap<char, int>mp;//key值可以重复
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
//void testUnordMap()//遍历无序
//{
//	unordered_map<char, int> mp;//key值不可重复
//	mp.insert(make_pair('b', 20));//有效插入
//	mp.insert(make_pair('b', 30));
//	mp.insert(make_pair('b', 40));
//	mp.insert(make_pair('b', 50));
//	mp.insert(make_pair('b', 60));
//	mp.insert(make_pair('a', 10));//有效插入
//	mp.insert(make_pair('c', 70));//有效插入    key不可重复
//	cout << "UnordMap" << endl;
//	for (const auto& p : mp)
//	{
//		cout << p.first << "--->" << p.second << endl;
//	}
//}
//
//int main()
//{
//	//cout << "key从小到大输出" << endl;
//	//testMap();
//
//	//testMulMap();
//
//	//cout << "key从大到小输出" << endl;
//	//testMap2();
//
//	//testMap();//有序
//	//testUnordMap();//无序
//
//	testMulMap();//有序可重复
//	testUnordMulMap();//无序可重复
//	
//	system("pause");
//	return 0;
//}
//
//#endif