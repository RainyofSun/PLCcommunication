/*----------------------------------------------------------------
 * ��Ȩ���� (c) �����е�Ƶ���װ�����޹�˾ ��������Ȩ����
 * ��˾���ƣ������е�Ƶ���װ�����޹�˾
 * 
 * �����ߣ���Ƚ
 * ����ʱ�䣺2024-7-29
 * �汾��V1.0.0
 * ���������� PLC ͨѶ������
 *
 * ----------------------------------------------------------------
 * �޸��ˣ�
 * ʱ�䣺
 * �޸�˵����
 *
 * �汾��V1.0.0
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
	// ��ȡ��������
	static CPLCCommunicationManager *&getInstance();
	// �ͷŵ����������˳�ʱ����
	static void deleteInstance();

	/**
	 * ���ܣ�����������Ϣ
	 * ���룺
			param1: connectInfo PLC���յ�IP��Port ��, UDP ��һλ Ϊ����IP��Port���ڶ���Ϊ����IP��Port
			param2: type		PLCЭ��(Ĭ��ΪUDPЭ��)
	 * �����
			��bool���Ƿ����óɹ�
	 */ 
	bool setPLCCommunicationInfo(std::vector<PLCConnectInfo> connectInfos, PLCProtocolType type = PLC_UDP);

	/**
	 * ���ܣ��Ƿ��ʼ���ɹ� 
	 * �����
			��bool���Ƿ��ʼ���ɹ�
	 */ 
	bool isInitialized() const;

	/**
	 * ���ܣ��Ƿ�������PLC
	 * �����
			��bool���Ƿ�������PLC
	 */ 
	bool isConnected() const;

	/**
	 * ���ܣ��Ƿ�����״�����ͨѶ
	 * �����
			��bool���Ƿ�����״�����ͨ��
	 */ 
	bool isFinishFirstCommunication() const;

	/**
	 * ���ܣ���ȡ�������ݰ�����
	 * �����
			��CPLCOrderDataPacket*���������ݰ�����
	 */ 
	CPLCOrderDataPacket* getOrderPacket() const;
	
	/**
	 * ���ܣ���ȡ�������ݰ�����
	 * ���룺
			param1: ��int�����ݰ�ID
	 * �����
			��CPLCWriteDataPacket*�����ݰ�����
	 */ 
	CPLCWriteDataPacket* getWritePacket(int nPacketId) const;

	/**
	 * ���ܣ���ȡ�������ݰ�����
	 * ���룺
			param1: ��int�����ݰ�ID
	 * �����
			��CPLCReadDataPacket*�����ݰ�����
	 */ 
	CPLCReadDataPacket* getReadPacket() const;

	/**
	 * ���ܣ���������
	 * �����
			��bool���Ƿ���³ɹ�
	 */ 
	bool update() const;

	////////////////////////////////// Modbus �ӿ� ////////////////////////////////////////
	/**
	 * ���ܣ���ȡboolֵ
	 * ���룺
			param1: ��CString����ַ "PLC0$M$102"
			param2: ��bool*����ȡֵ
	 * �����
			��bool���Ƿ��ȡ�ɹ�
	 */ 
	bool readBoolValue(CString address, bool *bValue);

	/**
	 * ���ܣ�д��Boolֵ
	 * ���룺
			param1: ��CString����ַ "PLC0$M$102"
			param2: ��bool��д��ֵ
	 * �����
			��bool���Ƿ�д��ɹ�
	 */ 
	bool writeBoolValue(CString address, bool bValue);

	/**
	 * ���ܣ���ȡShortֵ
	 * ���룺
			param1: ��CString����ַ "PLC0$M$102"
			param2: ��short*����ȡֵ
	 * �����
			��bool���Ƿ��ȡ�ɹ�
	 */ 
	bool readShortValue(CString address, short *shValue);

	/**
	 * ���ܣ�д��Shortֵ
	 * ���룺
			param1: ��CString����ַ "PLC0$M$102"
			param2: ��short��д��ֵ
	 * �����
			��bool���Ƿ�д��ɹ�
	 */ 
	bool writeShortValue(CString address, short shValue);
	
	/**
	 * ���ܣ���ȡIntֵ
	 * ���룺
			param1: ��CString����ַ "PLC0$M$102"
			param2: ��int*����ȡֵ
	 * �����
			��bool���Ƿ��ȡ�ɹ�
	 */ 
	bool readIntValue(CString address, int *iValue);

	/**
	 * ���ܣ�д��Intֵ
	 * ���룺
			param1: ��CString����ַ "PLC0$M$102"
			param2: ��int��д��ֵ
	 * �����
			��bool���Ƿ�д��ɹ�
	 */ 
	bool writeIntValue(CString address, int iValue);

	/**
	 * ���ܣ���ȡFloatֵ
	 * ���룺
			param1: ��CString����ַ "PLC0$M$102"
			param2: ��float*����ȡֵ
	 * �����
			��bool���Ƿ��ȡ�ɹ�
	 */ 
	bool readFloatValue(CString address, float *fValue);

	/**
	 * ���ܣ�д��Floatֵ
	 * ���룺
			param1: ��CString����ַ "PLC0$M$102"
			param2: ��float��д��ֵ
	 * �����
			��bool���Ƿ�д��ɹ�
	 */ 
	bool writeFloatValue(CString address, float fValue);

	static void release();

private:
	// ���캯������Ϊ˽�У���ֹ�ⲿ���������
	CPLCCommunicationManager(void);
	~CPLCCommunicationManager(void);
	// ���俽������͸�ֵ���캯��Ϊ˽�к�������ֹ�ⲿ�����͸�ֵ
	CPLCCommunicationManager(const CPLCCommunicationManager &manager);
	const CPLCCommunicationManager &operator = (const CPLCCommunicationManager &manager);

	// ����PLC�̺߳���
	static DWORD __stdcall PLCConnectThread(LPVOID lpParameter);
	// �ڲ��� -- ����
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
	// Ψһ��ʵ������ָ��
	static CPLCCommunicationManager *m_instance;
	static CMutex m_mutex;

	// �Ƿ��ʼ���ɹ�
	bool m_bIsInitialized;
	// ʵ�ʹ������
	CPLCDataManager *m_pPLCManager;
	// PLC Э��(Ĭ��UDPЭ��)
	PLCProtocolType m_protocol;
};

