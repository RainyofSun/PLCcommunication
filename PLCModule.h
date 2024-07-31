/*----------------------------------------------------------------
 * ��Ȩ���� (c) �����е�Ƶ���װ�����޹�˾ ��������Ȩ����
 * ��˾���ƣ������е�Ƶ���װ�����޹�˾
 * 
 * �����ߣ���Ƚ
 * ����ʱ�䣺2024-7-30
 * �汾��V1.0.0
 * ������������ ֻ����PLC�Ľ��������ݸ���
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

#define DLL_EXPORT __declspec(dllexport)
class DLL_EXPORT CPLCModule
{
public:
	/**
	 * ���ܣ���ȡΨһʵ��
	 * �����
			��CPLCModule*��ʵ��
	 */ 
	static CPLCModule *getInstance();

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
	 * ���ܣ���������ˢ���߳�
	 */ 
	void startThread();

	/**
	 * ���ܣ��Ƿ��ʼ���ɹ� 
	 * �����
			��bool���Ƿ��ʼ���ɹ�
	 */ 
	bool isInitialized() const;

	/**
	 * ���ܣ��ͷ���Դ
	 */ 
	static void release();
	
	/**
	 * ���ܣ��������ݰ�д������
	 * ���룺
			param1: ��id������id
			param2����int��д�������
	 * �����
			��bool���Ƿ�д��ɹ�
	 */
	bool orderPacketSetValue(int dataId, int writeValue) const;

	/**
	 * ���ܣ��������ݰ���ȡ����
	 * ���룺
			param1: ��id������id
			parma2����int����ȡ��ֵ
	 * �����
			��BOOL���Ƿ��ȡ�ɹ�
	 */ 
	bool orderPacketGetValue(int dataId, int readValue) const;

	/**
	 * ���ܣ��������ݰ�д������
	 * ���룺
			param1: ��int�����ݰ�ID
			param2: ��id������id
			param3����int��д�������
	 * �����
			��bool���Ƿ�д��ɹ�
	 */
	bool writePacketSetValue(int nPacketId, int dataId, int writeValue) const;

	/**
	 * ���ܣ��������ݰ���ȡ����
	 * ���룺
			param1: ��int�����ݰ�ID
			param2: ��id������id
			parma3����int����ȡ��ֵ
	 * �����
			��bool���Ƿ��ȡ�ɹ�
	 */ 
	bool writePacketGetValue(int nPacketId, int dataId, int readValue) const;

	/**
	 * ���ܣ���ȡ�������ݰ�����ֵ
	 * ���룺
			param1: ��int������ID
			parma2����int����ȡ��ֵ
	 * �����
			��bool���Ƿ��ȡ�ɹ�
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

private:// �߳�
	static DWORD __stdcall ThreadUpdate(LPVOID lpParameter);

	static bool m_bIsCloseThread;							// �Ƿ�ر��߳�(��������ʱʹ��)
	static HANDLE m_hThread;								// ״̬�߳̾��
	static int m_sleepTime;									// PLCģ���̼߳��ʱ�䣨��λ��ms��
	static bool m_bIsStartThread;							// �Ƿ�ʼ�߳�
};

