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
#include <vector>
#include "PLCModuleDefine.h"
#include "PLCOrderDataPacket.h"
#include "PLCWriteDataPacket.h"
#include "PLCReadDataPacket.h"
#include "PLCUDPDataPacketManager.h"
#include "PLCModbusDataManager.h"

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
			param1: connectInfo PLC接收的IP、Port 组, UDP 第一位 为接收IP、Port，第二组为发送IP、Port
			param2: type		PLC协议(默认为UDP协议)
	 * 输出：
			（bool）是否设置成功
	 */ 
	bool setPLCCommunicationInfo(std::vector<PLCConnectInfo> connectInfos, PLCProtocolType type = PLC_UDP);

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

	////////////////////////////////// Modbus 接口 ////////////////////////////////////////
	/**
	 * 功能：获取bool值
	 * 输入：
			param1: （CString）地址 "PLC0$M$102"
			param2: （bool*）获取值
	 * 输出：
			（bool）是否获取成功
	 */ 
	bool readBoolValue(CString address, bool *bValue);

	/**
	 * 功能：写入Bool值
	 * 输入：
			param1: （CString）地址 "PLC0$M$102"
			param2: （bool）写入值
	 * 输出：
			（bool）是否写入成功
	 */ 
	bool writeBoolValue(CString address, bool bValue);

	/**
	 * 功能：获取Short值
	 * 输入：
			param1: （CString）地址 "PLC0$M$102"
			param2: （short*）获取值
	 * 输出：
			（bool）是否获取成功
	 */ 
	bool readShortValue(CString address, short *shValue);

	/**
	 * 功能：写入Short值
	 * 输入：
			param1: （CString）地址 "PLC0$M$102"
			param2: （short）写入值
	 * 输出：
			（bool）是否写入成功
	 */ 
	bool writeShortValue(CString address, short shValue);
	
	/**
	 * 功能：获取Int值
	 * 输入：
			param1: （CString）地址 "PLC0$M$102"
			param2: （int*）获取值
	 * 输出：
			（bool）是否获取成功
	 */ 
	bool readIntValue(CString address, int *iValue);

	/**
	 * 功能：写入Int值
	 * 输入：
			param1: （CString）地址 "PLC0$M$102"
			param2: （int）写入值
	 * 输出：
			（bool）是否写入成功
	 */ 
	bool writeIntValue(CString address, int iValue);

	/**
	 * 功能：获取Float值
	 * 输入：
			param1: （CString）地址 "PLC0$M$102"
			param2: （float*）获取值
	 * 输出：
			（bool）是否获取成功
	 */ 
	bool readFloatValue(CString address, float *fValue);

	/**
	 * 功能：写入Float值
	 * 输入：
			param1: （CString）地址 "PLC0$M$102"
			param2: （float）写入值
	 * 输出：
			（bool）是否写入成功
	 */ 
	bool writeFloatValue(CString address, float fValue);

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
	
	AddrType getAddressType(CString type);
	SplitDataStruct spliteAddress(CString address);

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

