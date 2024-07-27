/**
共享内存区管理类，创建、删除共享区映射
注意：
	1.关闭共享区，仅关闭当前映射，实际共享区依旧存在
	2.通过openShare()获取的指针对象，不能delete
	3.不用时要先将获取指针对象复制为nullptr,通过closeShare()关闭映射
*/
#pragma once

#include <map>
using namespace std;

class CMemoryShare
{
public:
	CMemoryShare(void);
	~CMemoryShare(void);

	/*
	功能：打开共享区
	输入：
		param1(char *):共享区名称
		param2(int):共享区大小
	输出：
		(LPVOID):共享区无类型指针
	*/
	static LPVOID openShare(const char *name, int nSize);
	static void closeShare(const char *name);

private:
	static void closeAllShare();
	
	// 共享区容器
	static map<CString, HANDLE> m_shareHMap;
	static map<CString, LPVOID> m_shareMap;
};

