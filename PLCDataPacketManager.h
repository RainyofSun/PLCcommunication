/**
数据包模板基类
*/
#pragma once
#include <map>
#include "PLCDataManager.h"
#include "PLCReadDataPacket.h"
#include "PLCWriteDataPacket.h"
#include "PLCOrderDataPacket.h"

using namespace std;

class CPLCDataPacketManager: public CPLCDataManager
{
public:
	CPLCDataPacketManager(void);
	~CPLCDataPacketManager(void);

	/**
	 * 功能：获取命令数据包对象
	 * 输出：
			（OrderPacket *）命令数据包对象
	 */ 
	CPLCOrderDataPacket* getOrderPacket() const;

	/**
	功能：获取发送数据包对象
	输入：
		param1：（int nPacketId）数据包ID
	输出：
		（CPLCWriteDataPacket*）数据包对象
	*/
	CPLCWriteDataPacket* getWritePacket(int nPacketId);

	/**
	功能：获取接收数据包对象
	输出：
		(CPLCReadDataPacket*) 数据包对象	
	*/
	CPLCReadDataPacket* getReadPacket() const;

	/*
	功能：初始化数据包对象
	输出：
		(bool)：是否初始化成功
	*/
	virtual bool initPacket()=0;
	
	/*
	功能：创建发送数据包对象
	输入：
		param1:(int)数据包ID
	输出：
		（bool）：是否创建成功
	*/
	virtual bool createWritePacket(int nPacketId)=0;

	/**
	功能：释放所有发送数据包对象
	*/
	void releaseWritePacket();
	
	/**
	功能：释放所有数据包对象
	*/
	void releasePacket();
	void releaseConnect();
	void release();

protected:
	// 发送数据连接对象（UDP接收与发送IP不同）
	CPLCConnection *m_pSendConnection;
	// 接收数据对象
	CPLCConnection *m_pReceiveConnection;

	// 命令数据包对象
	CPLCOrderDataPacket *m_pOrderPacket;
	// 接收数据包对象
	CPLCReadDataPacket *m_pReadDataPacket;
	// 发送数据包对象容器
	map<int, CPLCWriteDataPacket*> m_writeDataPacketMap;
};

