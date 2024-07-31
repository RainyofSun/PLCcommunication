/*----------------------------------------------------------------
 * 版权所有 (c) 北京中电科电子装备有限公司 保留所有权利。
 * 公司名称：北京中电科电子装备有限公司
 * 
 * 创建者：刘冉
 * 创建时间：2024-7-29
 * 版本：V1.0.0
 * 描述：单例 PLC 通讯管理类
 *
 * ----------------------------------------------------------------
 * 修改人：
 * 时间：
 * 修改说明：
 *
 * 版本：V1.0.0
 *----------------------------------------------------------------*/
#pragma once
#include <afxmt.h>
#include "PLCModuleDefine.h"
#include "PLCOrderDataPacket.h"
#include "PLCWriteDataPacket.h"
#include "PLCReadDataPacket.h"
#include "PLCUDPDataPacketManager.h"

class CPLCCommunicationManager
{
public:
	// 获取单例对象
	static CPLCCommunicationManager *&getInstance();
	// 释放单例，进程退出时调用
	static void deleteInstance();

	/**
	 * 功能：设置连接信息
	 * 输入：
			param1: receiveInfo PLC接收的IP、Port
			param2: sendInfo	PLC发送的IP、Port
			param3: type		PLC协议(默认为UDP协议)
	 * 输出：
			（bool）是否设置成功
	 */ 
	bool setPLCCommunicationInfo(PLCConnectInfo receiveInfo, PLCConnectInfo sendInfo, PLCProtocolType type = PLC_UDP);

	/**
	 * 功能：是否初始化成功 
	 * 输出：
			（bool）是否初始化成功
	 */ 
	bool isInitialized() const;

	/**
	 * 功能：是否已连接PLC
	 * 输出：
			（bool）是否已连接PLC
	 */ 
	bool isConnected() const;

	/**
	 * 功能：是否完成首次数据通讯
	 * 输出：
			（bool）是否完成首次数据通信
	 */ 
	bool isFinishFirstCommunication() const;

	/**
	 * 功能：获取命令数据包对象
	 * 输出：
			（CPLCOrderDataPacket*）命令数据包对象
	 */ 
	CPLCOrderDataPacket* getOrderPacket() const;
	
	/**
	 * 功能：获取发送数据包对象
	 * 输入：
			param1: （int）数据包ID
	 * 输出：
			（CPLCWriteDataPacket*）数据包对象
	 */ 
	CPLCWriteDataPacket* getWritePacket(int nPacketId) const;

	/**
	 * 功能：获取接收数据包对象
	 * 输入：
			param1: （int）数据包ID
	 * 输出：
			（CPLCReadDataPacket*）数据包对象
	 */ 
	CPLCReadDataPacket* getReadPacket() const;

	/**
	 * 功能：更新数据
	 * 输出：
			（bool）是否更新成功
	 */ 
	bool update() const;

	static void release();

private:
	// 构造函数设置为私有，禁止外部构造和析构
	CPLCCommunicationManager(void);
	~CPLCCommunicationManager(void);
	// 将其拷贝构造和赋值构造函数为私有函数，禁止外部拷贝和赋值
	CPLCCommunicationManager(const CPLCCommunicationManager &manager);
	const CPLCCommunicationManager &operator = (const CPLCCommunicationManager &manager);

	// 连接PLC线程函数
	static DWORD __stdcall PLCConnectThread(LPVOID lpParameter);
	// 内部类 -- 回收
	class DataPacketManagerGarbo
	{
	public:
		~DataPacketManagerGarbo()
		{
			release();
		}
	};

	static DataPacketManagerGarbo garbo;

private:
	// 唯一单实例对象指针
	static CPLCCommunicationManager *m_instance;
	static CMutex m_mutex;

	// 是否初始化成功
	bool m_bIsInitialized;
	// 实际管理对象
	CPLCDataManager *m_pPLCManager;
	// PLC 协议(默认UDP协议)
	PLCProtocolType m_protocol;
};

