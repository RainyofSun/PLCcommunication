/**
发送数据包类，在更新数据包时将数据包发送到PLC
*/
#pragma once
#include "PLCDataPacketParam.h"

class CPLCWriteDataPacket: public CPLCDataPacketParam
{
public:
	CPLCWriteDataPacket(CPLCConnection *connect, int nPacketId, char *name = nullptr);
	~CPLCWriteDataPacket(void);

	/*
	功能：更新数据
	输出：
		(bool)是否更新成功
	*/
	bool updatePacket();

protected:
	// 发送次数
	int m_nSendTimes;
	// 上一次更新时的buff区 -- 缓存package到内存，与新的package进行对比
	char m_lastBuff[sizeof(PLCPackage)];
};

