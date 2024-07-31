#pragma once
#include "PLCModuleDefine.h"
/**
PLC 连接基类，负责PLC的连接、状态监控、数据收发
*/
class CPLCConnection
{
public:
	CPLCConnection(void);
	~CPLCConnection(void);
	
	/**
	功能：初始化操作
	param1：plc协议类型
	param2：plc连接的ip地址、端口号
	*/
	void prepareToConnect(PLCProtocolType plcType, PLCConnectInfo info);

	/**
	功能：PLC连接状态
	输出：
		（bool）连接状态
	*/
	bool plcIsConnect();

	/**
	功能：PLC连接
	输出：
		（bool）PLC是否连接成功
	*/
	virtual bool startConnectPLC()=0;

	/**
	功能：PLC断开
	*/
	virtual void closeConnectPLC()=0;
	/**
	功能：获取错误信息
	输出：
		（CString）错误信息
	*/
	virtual CString plcError()=0;

	/**
	功能：发送数据
	输入：
		param1:(char *) 数据字节数组
		param2:(int) 数据字节大小
	输出：
		(bool)是否发送成功
	*/
	virtual bool sendMessage(char *buff, int nSize)=0;

	/**
	功能：接收数据
	输入：
	param1:(char *) 数据字节数组
	param2:(int) 数据字节大小
	输出：
	(bool)是否接收成功
	*/
	virtual bool receiveMessage(char *buff, int nSize)=0;

protected:
	// PLC是否已连接
	bool m_isConnect;
	// 错误码
	int m_errorCode;
	// plc 协议类型
	PLCProtocolType m_protocolType;
	// plc 连接信息
	PLCConnectInfo m_info;
};

