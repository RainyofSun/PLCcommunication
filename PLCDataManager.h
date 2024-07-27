/**
数据模板基类
*/
#pragma once
#include "PLCConnection.h"

class CPLCDataManager
{
public:
	CPLCDataManager(void);
	~CPLCDataManager(void);

	/**
	功能：是否初始化成功
	输出：
		（bool）是否初始化成功
	*/
	bool isInitialized() const;

	/**
	功能：是否已连接PLC(物理连接)
	输出：
		（bool）是否已连接PLC
	*/
	bool isConnect();

	/**
	功能：是否完成首次数据通信
	输出：
		（bool）是否完成首次通信
	*/
	bool isFinishFirstCommunction() const;

	/**
	功能：更新数据
	输出：
		（bool）是否更新成功
	*/
	virtual bool update()=0;

	/**
	功能：连接PLC
	输出：
		（bool）是否连接成功
	*/
	virtual bool connectPLC()=0;

protected:
	/**
	功能：初始化PLC连接对象
	输出：
		（bool）是否初始化成功
	*/
	virtual bool initConnection()=0;

	/**
	功能：释放所有连接对象
	*/
	virtual void releaseConnect(){};

	/**
	功能：释放资源
	*/
	virtual void release();

	// 是否初始化成功
	bool m_bIsInitialized;
	// 是否已连接PLC
	bool m_bIsConnectPLC;
	// 是否完成首次通信
	bool m_bIsFinishFirstCommunication;
};

