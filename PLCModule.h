/*----------------------------------------------------------------
 * 版权所有 (c) 北京中电科电子装备有限公司 保留所有权利。
 * 公司名称：北京中电科电子装备有限公司
 * 
 * 创建者：刘冉
 * 创建时间：2024-7-30
 * 版本：V1.0.0
 * 描述：单例类 只负责PLC的建立、数据更新
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

#define DLL_EXPORT __declspec(dllexport)
class DLL_EXPORT CPLCModule
{
public:
	/**
	 * 功能：获取唯一实例
	 * 输出：
			（CPLCModule*）实例
	 */ 
	static CPLCModule *getInstance();

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
	 * 功能：开启数据刷新线程
	 */ 
	void startThread();

	/**
	 * 功能：是否初始化成功 
	 * 输出：
			（bool）是否初始化成功
	 */ 
	bool isInitialized() const;

	/**
	 * 功能：释放资源
	 */ 
	static void release();
	
	/**
	 * 功能：命令数据包写入数据
	 * 输入：
			param1: （id）数据id
			param2：（int）写入的数据
	 * 输出：
			（bool）是否写入成功
	 */
	bool orderPacketSetValue(int dataId, int writeValue) const;

	/**
	 * 功能：命令数据包读取数据
	 * 输入：
			param1: （id）数据id
			parma2：（int）读取的值
	 * 输出：
			（BOOL）是否读取成功
	 */ 
	bool orderPacketGetValue(int dataId, int readValue) const;

	/**
	 * 功能：发送数据包写入数据
	 * 输入：
			param1: （int）数据包ID
			param2: （id）数据id
			param3：（int）写入的数据
	 * 输出：
			（bool）是否写入成功
	 */
	bool writePacketSetValue(int nPacketId, int dataId, int writeValue) const;

	/**
	 * 功能：发送数据包读取数据
	 * 输入：
			param1: （int）数据包ID
			param2: （id）数据id
			parma3：（int）读取的值
	 * 输出：
			（bool）是否读取成功
	 */ 
	bool writePacketGetValue(int nPacketId, int dataId, int readValue) const;

	/**
	 * 功能：获取接收数据包数据值
	 * 输入：
			param1: （int）数据ID
			parma2：（int）读取的值
	 * 输出：
			（bool）是否读取成功
	 */
	bool readPacketGetValue(int dataId, int readValue) const;

private:
	CPLCModule(void);
	CPLCModule(const CPLCModule &);
	CPLCModule & operator=(const CPLCModule &);
	~CPLCModule(void);

	static CPLCModule *m_instance;
	static CMutex m_mutex;

	class PLCModuleGarbo
	{
	public:
		~PLCModuleGarbo()
		{
			release();
		}
	};

	static PLCModuleGarbo garbo;

private:// 线程
	static DWORD __stdcall ThreadUpdate(LPVOID lpParameter);

	static bool m_bIsCloseThread;							// 是否关闭线程(对象析构时使用)
	static HANDLE m_hThread;								// 状态线程句柄
	static int m_sleepTime;									// PLC模块线程间隔时间（单位：ms）
	static bool m_bIsStartThread;							// 是否开始线程
};

