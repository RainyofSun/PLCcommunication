/**
PLC 数据包基类
*/
#pragma once
#include <iostream>
#include "PLCConnection.h"
#include "PLCModuleDefine.h"

using namespace std;

class CPLCDataPacket
{
public:
	CPLCDataPacket(CPLCConnection *connect);
	~CPLCDataPacket(void);

	/**
	功能：更新数据包
	输出：
		（bool）是否更新成功
	*/
	virtual bool updatePacket()=0;

protected:
	// PLC 连接对象
	CPLCConnection *m_connect;
	// 更新时的buff大小
	int m_nByteSize;
};

