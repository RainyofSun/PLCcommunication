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
#include "PLCModuleDefine.h"
#include "PLCOrderDataPacket.h"
#include "PLCWriteDataPacket.h"
#include "PLCReadDataPacket.h"
#include "PLCUDPDataPacketManager.h"

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
			param1: receiveInfo PLC���յ�IP��Port
			param2: sendInfo	PLC���͵�IP��Port
			param3: type		PLCЭ��(Ĭ��ΪUDPЭ��)
	 * �����
			��bool���Ƿ����óɹ�
	 */ 
	bool setPLCCommunicationInfo(PLCConnectInfo receiveInfo, PLCConnectInfo sendInfo, PLCProtocolType type = PLC_UDP);

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

