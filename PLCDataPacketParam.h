/**
PLC 数据包参数配置、功能扩展
*/
#pragma once
#include "PLCDataPacket.h"

class CPLCDataPacketParam: public CPLCDataPacket
{
public:
	/*
	输入：
		param1：（CPLCConnection *）PLC连接对象
		param2：（nPacketId int）数据包包号
		param3：（name char*）共享区名称{默认：nullptr}
	*/
	CPLCDataPacketParam(CPLCConnection *connect, int nPacketId, char *name = nullptr);
	~CPLCDataPacketParam(void);

	/**
	功能：设置数据包数据
	输入：
		param1:(nPackageID int) 数据包ID
		param2:(sPackage PLCPackage) 数据包
	输出：
		(bool) 是否设置成功
	*/
	bool setPackage(int nPackageID, const PLCPackage &sPackage) const; 

	/**
	功能：获取数据包数据
	输出：
		（PLCPackage）数据包
	*/
	PLCPackage getPackage() const;

	/**
	功能：设置数据包大小
	输入：
		param1:(int)大小 范围：0<= x <=PACKET_MAX_SIZE
	*/
	void setPacketSize(int nSize);

	/**
	功能：设置单个数据
	输入：
		param1:(int)数据id
		param2:(int)数据值
	*/
	bool setValue(int id, int nValue) const;

	/*
	功能：获取单个数据
	输入：
		param1:(int)数据id
	输出：
		(int)数据值
	*/
	int getValue(int id) const;
	
	/*
	功能：设置单个数据的bit值
	输入：
		param1:(int)数据id
		param2:(int)位id
		param3:(bool)位值
	*/
	bool setBit(int id, int bitID, bool bValue) const;

	/*
	功能：获取单个bit数据
	输入：
		param1:(int)数据id
		param2:(int)位id
	输出：
		(int)位值(id不正确则返回-1)
	*/
	int getBit(int id, int bitId) const;

	/**
	 * 功能：将运动的order移动至数据包
	 * 输入：
			param1: id
			param2: nValue
	 * 输出：
			bool
	 */ 
	bool setOrderValue(int id, int nValue = -1000) const;

protected:
	// 数据
	PLCPackage *m_sPackage;
	// 包号
	int m_nPacketID;
	// 实际数据大小
	int m_nSize;
	// 更新时的buff区
	char m_buff[sizeof(PLCPackage)];
	// 是否打开共享区，若是析构时不delete
	bool m_isOpenShare;
};

