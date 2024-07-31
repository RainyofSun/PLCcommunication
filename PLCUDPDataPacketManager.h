/*----------------------------------------------------------------
 * 版权所有 (c) 北京中电科电子装备有限公司 保留所有权利。
 * 公司名称：北京中电科电子装备有限公司
 * 
 * 创建者：刘冉
 * 创建时间：2024-7-27
 * 版本：V1.0.0
 * 描述：UDP 数据包管理类 初始化需要配置接收、发送的IP地址和端口号
 *
 * ----------------------------------------------------------------
 * 修改人：
 * 时间：
 * 修改说明：
 *
 * 版本：V1.0.0
 *----------------------------------------------------------------*/
#pragma once
#include "PLCDataPacketManager.h"

class CPLCUDPDataPacketManager: public CPLCDataPacketManager
{
public:
	CPLCUDPDataPacketManager(PLCConnectInfo receiveInfo, PLCConnectInfo sendInfo);
	~CPLCUDPDataPacketManager(void);

	/**
	 * 功能：更新数据
	 * 输出：
			（bool）是否更新成功
	 */ 
	bool update();

	/**
	 * 功能：连接PLC
	 * 输出：
			（bool）是否连接成功
	 */ 
	bool connectPLC();

private:
	/**
	 * 功能：初始化PLC连接对象
	 * 输出：
			（bool）是否初始化成功
	 */ 
	bool initConnection();

	/**
	 * 功能：初始化数据包对象
	 * 输出：
			（bool）是否初始化成功
	 */ 
	bool initPacket();

	/**
	 * 功能：创建发送数据包对象
	 * 输入：
			param1: （int）数据包ID
	 * 输出：
			（bool）是否创建成功
	 */ 
	bool createWritePacket(int nPacketId);

	/**
	 * 功能：首轮发送数据
	 * 输出：
			（bool）是否完成首轮发送
	 */ 
	bool firstSend();

	/**
	 * 功能：判断是否断开连接
	 * 输出：
			（bool）是否断开连接
	 */ 
	bool isDisconnectPLC();

private:
	// 心跳
	long m_oldHeartbeat;
	// 是否为首次发送
	bool m_isFirst;
	int m_iTime;
	
	PLCConnectInfo m_sReceiveInfo;
	PLCConnectInfo m_sSendInfo;
};