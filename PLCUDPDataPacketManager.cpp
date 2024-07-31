#include "StdAfx.h"
#include "PLCUDPDataPacketManager.h"
#include "UDPConnection.h"

// �������Ͽ���
const int HEART_BREAK_MAX_NUMBER = 100;

CPLCUDPDataPacketManager::CPLCUDPDataPacketManager(PLCConnectInfo receiveInfo, PLCConnectInfo sendInfo)
{

	m_sReceiveInfo = receiveInfo;
	m_sSendInfo = sendInfo;

	m_iTime = 0;
	const bool is1 = initConnection();
	const bool is2 = initPacket();

	m_bIsInitialized = (is1 && is2);
	m_oldHeartbeat = -10;
	m_isFirst = true;
	connectPLC();
}


CPLCUDPDataPacketManager::~CPLCUDPDataPacketManager(void)
{
}

bool CPLCUDPDataPacketManager::update()
{
	// ���¶�ȡ���ݰ�
	if (m_pReadDataPacket)
	{
		m_pReadDataPacket->updatePacket();

		if (m_bIsFinishFirstCommunication)
		{
			isDisconnectPLC();
		}
	}

	if (m_iTime <= 2)
	{
		m_iTime ++;
		return true;
	}
	else
	{
		m_iTime = 0;		
	}

	if (m_pReadDataPacket->getValue(250) != 0) // ��ʼ�յ�PLC���͵�����
	{
		// ����������ʱ����Ҫ���������ݰ��·�
		if (m_isFirst)
		{
			m_isFirst = !firstSend();
		}
		else 
		{
			static int nCount = 0;
			// �������еķ������ݰ�
			if (!m_writeDataPacketMap.empty())
			{
				nCount = 0;
				for (auto i = m_writeDataPacketMap.begin(); i != m_writeDataPacketMap.end(); ++i)
				{
					if (i->second->updatePacket())
					{
						nCount ++;
					}
					// ��ֹͬʱ���ͳ���2����
					if (nCount == 2)
					{
						break;
					}
				}
			}
		}

		// �����������ݰ�
		if (m_pOrderPacket)
		{
			m_pOrderPacket->updatePacket();
		}
	}

	return true;
}

bool CPLCUDPDataPacketManager::connectPLC()
{
	if (!m_bIsInitialized)
	{
		return false;
	}

	if (nullptr != m_pReceiveConnection)
	{
		m_bIsConnectPLC = m_pReceiveConnection->startConnectPLC();

		static int nCount = 0;
		if ((!m_bIsConnectPLC) && (nCount ++ > 11))
		{
			CString str;
			str.Format(_T("���ն�UDP����ʧ�ܣ�������Ϣ��%s"), m_pReceiveConnection->plcError());
			AfxMessageBox(str);	
		}
	}

	return m_bIsConnectPLC;
}

bool CPLCUDPDataPacketManager::initConnection()
{
	if (m_sReceiveInfo.port == 0 || m_sSendInfo.port == 0)
	{
		return false;
	}

	m_pSendConnection = new CUDPConnection();
	m_pReceiveConnection = new CUDPConnection();

	m_pSendConnection->prepareToConnect(PLC_UDP, m_sSendInfo);
	m_pReceiveConnection->prepareToConnect(PLC_UDP, m_sReceiveInfo);

	return ((nullptr != m_pSendConnection) && (nullptr != m_pReceiveConnection));
}

bool CPLCUDPDataPacketManager::initPacket()
{
	m_pOrderPacket = new CPLCOrderDataPacket(m_pSendConnection);
	const bool is1 = createWritePacket(write_packet_id_1);
	const bool is2 = createWritePacket(write_packet_id_2);
	m_pReadDataPacket = new CPLCReadDataPacket(m_pReceiveConnection, read_packet_id, "PLC_COMMUNICATION_READ1");

	return (m_pOrderPacket && is1 && is2 && m_pReadDataPacket);
}

bool CPLCUDPDataPacketManager::createWritePacket(int nPacketId)
{
	bool isSuccess = false;
	if (m_writeDataPacketMap.find(nPacketId) == m_writeDataPacketMap.end())
	{
		CPLCWriteDataPacket *pWriteDataPacket = nullptr;
		switch (nPacketId)
		{
		case write_packet_id_1:
			pWriteDataPacket = new CPLCWriteDataPacket(m_pSendConnection, write_packet_id_1, "PLC_COMMUNICATION_WRITE2");
			isSuccess = true;
			break;
		case write_packet_id_2:
			pWriteDataPacket = new CPLCWriteDataPacket(m_pSendConnection, write_packet_id_2, "PLC_COMMUNICATION_WRITE3");
			isSuccess = true;
			break;
		default:
			break;
		}

		if (isSuccess)
		{
			m_writeDataPacketMap[nPacketId] = pWriteDataPacket;
		}
	}

	return isSuccess;
}

bool CPLCUDPDataPacketManager::firstSend()
{
	bool isFinish = false;
	static int nCount = 0;
	static bool isOrderFinish = false;

	// �������еķ������ݰ�
	if (!m_writeDataPacketMap.empty())
	{
		nCount = 0;
		for (auto i = m_writeDataPacketMap.begin(); i != m_writeDataPacketMap.end() ; ++i)
		{
			if (i->second)
			{
				if (i->second->updatePacket())
				{
					nCount ++;
				}
				if (nCount == 2)
				{
					break;
				}
			}
		}
	}

	if (nCount < 2)
	{
		isFinish = true;
		m_bIsFinishFirstCommunication = true;

		HWND pWnd = ::FindWindow("COMMUNICATION", NULL);
		if (pWnd != NULL)
		{	
			CString strMessage = "Connected";
			COPYDATASTRUCT stData;
			stData.cbData= strMessage.GetLength()+1;
			stData.lpData= strMessage.GetBuffer(stData.cbData);		
			::SendMessage(pWnd, WM_COPYDATA, 0, (LPARAM)&stData );
		}
	}

	return isFinish;
}

bool CPLCUDPDataPacketManager::isDisconnectPLC()
{
	static int count = 0;
	bool bIsDisconnect = false;
	int currentHeartbeat = m_pReadDataPacket->getValue(250);

	if (m_oldHeartbeat != currentHeartbeat)
	{
		m_oldHeartbeat = currentHeartbeat;
		count = 0;
	}
	else
	{
		if (count++ > HEART_BREAK_MAX_NUMBER)
		{
			bIsDisconnect = true;
			HWND pWnd = ::FindWindow("COMMUNICATION", NULL);
			if (pWnd != NULL)
			{
				CString strMessage = "Disconnect";
				COPYDATASTRUCT stData;
				stData.cbData= strMessage.GetLength()+1;
				stData.lpData= strMessage.GetBuffer(stData.cbData);		
				::SendMessage(pWnd, WM_COPYDATA, 0, (LPARAM)&stData );
			}
		}
	}

	return bIsDisconnect;
}
