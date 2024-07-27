/*----------------------------------------------------------------
 * 版权所有 (c) 北京中电科电子装备有限公司 保留所有权利。
 * 公司名称：北京中电科电子装备有限公司
 * 
 * 创建者：刘冉
 * 创建时间：2024-7-27
 * 版本：V1.0.0
 * 描述：命令数据包类，更新数据包时将数据发送到PLC
 *
 * ----------------------------------------------------------------
 * 修改人：
 * 时间：
 * 修改说明：
 *
 * 版本：V1.0.0
 *----------------------------------------------------------------*/
#pragma once
#include "PLCDataPacket.h"

class CPLCOrderDataPacket:public CPLCDataPacket 
{
public:
	CPLCOrderDataPacket(CPLCConnection *connect);
	~CPLCOrderDataPacket(void);

	/**
	 * 功能：设置命令数据包
	 * 输入：
			param1: (OrderPackage) 命令数据包 
	 */ 
	void setPackage(const OrderPackage &sOrderPackage);

	/**
	 * 功能：获取命令数据包
	 * 输入：
			param1: （int）数据包ID
	 * 输出：
			（OrderPackage）命令数据包
	 */ 
	OrderPackage getPackage() const;

	/**
	 * 功能：设置命令
	 * 输入：
			param1: （int）命令码
			param2: （int）值
	 * 输出：
			（bool）是否发送成功
	 */ 
	bool setOrderValue(int id, int nValue = -1000);

	/**
	 * 功能：获取单个数据
	 * 输入：
			param1: （int）命令码 
	 * 输出：
			（int）数据值
	 */ 
	int getOrderValue(int nOrderNum);

	/**
	 * 功能：更新数据
	 * 输出：
			（bool）数据是否更新成功
	 */ 
	bool updatePacket();

private:
	// 命令数据包
	OrderPackage m_sPackage;
	// 更新时的buff区
	char m_buff[sizeof(OrderPackage)];
};

