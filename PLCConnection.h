#pragma once
#include "PLCModuleDefine.h"
/**
PLC ���ӻ��࣬����PLC�����ӡ�״̬��ء������շ�
*/
class CPLCConnection
{
public:
	CPLCConnection(void);
	~CPLCConnection(void);
	
	/**
	���ܣ���ʼ������
	param1��plcЭ������
	param2��plc���ӵ�ip��ַ���˿ں�
	*/
	void prepareToConnect(PLCProtocolType plcType, PLCConnectInfo info);

	/**
	���ܣ�PLC����״̬
	�����
		��bool������״̬
	*/
	bool plcIsConnect();

	/**
	���ܣ�PLC����
	�����
		��bool��PLC�Ƿ����ӳɹ�
	*/
	virtual bool startConnectPLC()=0;

	/**
	���ܣ�PLC�Ͽ�
	*/
	virtual void closeConnectPLC()=0;
	/**
	���ܣ���ȡ������Ϣ
	�����
		��CString��������Ϣ
	*/
	virtual CString plcError()=0;

	/**
	���ܣ���������
	���룺
		param1:(char *) �����ֽ�����
		param2:(int) �����ֽڴ�С
	�����
		(bool)�Ƿ��ͳɹ�
	*/
	virtual bool sendMessage(char *buff, int nSize)=0;

	/**
	���ܣ���������
	���룺
	param1:(char *) �����ֽ�����
	param2:(int) �����ֽڴ�С
	�����
	(bool)�Ƿ���ճɹ�
	*/
	virtual bool receiveMessage(char *buff, int nSize)=0;

protected:
	// PLC�Ƿ�������
	bool m_isConnect;
	// ������
	int m_errorCode;
	// plc Э������
	PLCProtocolType m_protocolType;
	// plc ������Ϣ
	PLCConnectInfo m_info;
};

