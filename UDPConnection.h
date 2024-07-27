#pragma once
#include "PLCConnection.h"

/**
UDP 连接类，socket 使用UDP协议，发送、接收数据
*/
class CUDPConnection: public CPLCConnection
{
public:
	CUDPConnection(void);
	~CUDPConnection(void);

	void prepareToConnect(PLCProtocolType plcType, PLCConnectInfo info);
	bool startConnectPLC();
	void closeConnectPLC();
	CString plcError();
	bool sendMessage(char *buff, int nSize);
	bool receiveMessage(char *buff, int nSize);

private:
	// socket 对象
	SOCKET m_socket;
	// 发送目标地址
	sockaddr_in m_address;
	// 地址大小
	int m_addressSize;
	// 是否已经初始化
	bool m_isInitialized;
};

