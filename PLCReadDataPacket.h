/**
接收PLC数据包类，更新数据包时将从PLC获取数据
*/
#pragma once
#include "PLCDataPacketParam.h"

class CPLCReadDataPacket: public CPLCDataPacketParam
{
public:
	CPLCReadDataPacket(CPLCConnection *connect, int nPacketId, char *name = nullptr);
	~CPLCReadDataPacket(void);

	/*
	功能：更新数据
	输出：
		(bool)是否更新成功
	*/
	bool updatePacket();
};

