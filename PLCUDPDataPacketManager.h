/*----------------------------------------------------------------
 * ��Ȩ���� (c) �����е�Ƶ���װ�����޹�˾ ��������Ȩ����
 * ��˾���ƣ������е�Ƶ���װ�����޹�˾
 * 
 * �����ߣ���Ƚ
 * ����ʱ�䣺2024-7-27
 * �汾��V1.0.0
 * ������UDP ���ݰ������� ��ʼ����Ҫ���ý��ա����͵�IP��ַ�Ͷ˿ں�
 *
 * ----------------------------------------------------------------
 * �޸��ˣ�
 * ʱ�䣺
 * �޸�˵����
 *
 * �汾��V1.0.0
 *----------------------------------------------------------------*/
#pragma once
#include "PLCDataPacketManager.h"

class CPLCUDPDataPacketManager: public CPLCDataPacketManager
{
public:
	CPLCUDPDataPacketManager(PLCConnectInfo receiveInfo, PLCConnectInfo sendInfo);
	~CPLCUDPDataPacketManager(void);

	/**
	 * ���ܣ���������
	 * �����
			��bool���Ƿ���³ɹ�
	 */ 
	bool update();

	/**
	 * ���ܣ�����PLC
	 * �����
			��bool���Ƿ����ӳɹ�
	 */ 
	bool connectPLC();

private:
	/**
	 * ���ܣ���ʼ��PLC���Ӷ���
	 * �����
			��bool���Ƿ��ʼ���ɹ�
	 */ 
	bool initConnection();

	/**
	 * ���ܣ���ʼ�����ݰ�����
	 * �����
			��bool���Ƿ��ʼ���ɹ�
	 */ 
	bool initPacket();

	/**
	 * ���ܣ������������ݰ�����
	 * ���룺
			param1: ��int�����ݰ�ID
	 * �����
			��bool���Ƿ񴴽��ɹ�
	 */ 
	bool createWritePacket(int nPacketId);

	/**
	 * ���ܣ����ַ�������
	 * �����
			��bool���Ƿ�������ַ���
	 */ 
	bool firstSend();

	/**
	 * ���ܣ��ж��Ƿ�Ͽ�����
	 * �����
			��bool���Ƿ�Ͽ�����
	 */ 
	bool isDisconnectPLC();

private:
	// ����
	long m_oldHeartbeat;
	// �Ƿ�Ϊ�״η���
	bool m_isFirst;
	int m_iTime;
	
	PLCConnectInfo m_sReceiveInfo;
	PLCConnectInfo m_sSendInfo;
};