#include "StdAfx.h"
#include "PLCCommunicationManager.h"
#include "PLCUDPDataPacketManager.h"

// 初始化静态成员变量
CPLCCommunicationManager *CPLCCommunicationManager::m_instance = NULL;
CPLCCommunicationManager::DataPacketManagerGarbo CPLCCommunicationManager::garbo;
CMutex CPLCCommunicationManager::m_mutex;

CPLCCommunicationManager::CPLCCommunicationManager(void)
{
	m_protocol = PLC_UDP;
	m_bIsInitialized = false;
	m_pPLCManager = nullptr;
}

CPLCCommunicationManager::~CPLCCommunicationManager(void)
{
}

CPLCCommunicationManager *&CPLCCommunicationManager::getInstance()
{
	if (m_instance == NULL)
	{
		CSingleLock lock = CSingleLock(&m_mutex);
		lock.Lock();
		if (lock.IsLocked() && m_instance == NULL)
		{
			m_instance = new (std::nothrow) CPLCCommunicationManager;
			lock.Unlock();
		}
	}

	return m_instance;
}

void CPLCCommunicationManager::deleteInstance()
{
	CSingleLock lock = CSingleLock(&m_mutex);
	lock.Lock();
	if (lock.IsLocked() && m_instance != NULL)
	{
		delete m_instance;
		m_instance = NULL;
		lock.Unlock();
	}
}

DWORD __stdcall CPLCCommunicationManager::PLCConnectThread(LPVOID lpParameter)
{
	int i = 0;
	while (!CPLCCommunicationManager::getInstance()->m_pPLCManager->connectPLC())
	{
		Sleep(2000);
		if (i ++ > 10)
		{
			break;
		}
	}

	return 0;
}

void CPLCCommunicationManager::release()
{
	if (m_instance)
	{
		if (m_instance->m_pPLCManager)
		{
			delete m_instance->m_pPLCManager;
			m_instance->m_pPLCManager = nullptr;
			m_instance->m_bIsInitialized = false;
		}

		delete m_instance;
		m_instance = nullptr;
	}
}

bool CPLCCommunicationManager::isInitialized() const 
{
	return m_bIsInitialized;
}

bool CPLCCommunicationManager::isConnected() const 
{
	return m_pPLCManager->isConnect();
}

bool CPLCCommunicationManager::isFinishFirstCommunication() const
{
	return m_pPLCManager->isFinishFirstCommunction();
}

bool CPLCCommunicationManager::setPLCCommunicationInfo(PLCConnectInfo receiveInfo, PLCConnectInfo sendInfo, PLCProtocolType type /* = PLC_UDP */)
{
	if (m_bIsInitialized)
	{
		return true;
	}

	m_protocol = type;

	if (type == PLC_UDP)
	{
		m_pPLCManager = new CPLCUDPDataPacketManager(receiveInfo, sendInfo);
	}

	if (nullptr != m_pPLCManager)
	{
		m_bIsInitialized = m_pPLCManager->isInitialized();
		if (m_bIsInitialized && !m_pPLCManager->isConnect())
		{
			CreateThread(NULL, 0, PLCConnectThread, this, 0, NULL);
		}
	}

	return m_bIsInitialized;
}

CPLCOrderDataPacket* CPLCCommunicationManager::getOrderPacket() const
{
	CPLCOrderDataPacket *pPacket = nullptr;

	if (m_protocol == PLC_UDP)
	{
		CPLCUDPDataPacketManager *pUdpManager = dynamic_cast<CPLCUDPDataPacketManager *>(m_pPLCManager);
		if (nullptr != pUdpManager)
		{
			pPacket = pUdpManager->getOrderPacket();
		}
	}

	return pPacket;
}

CPLCWriteDataPacket* CPLCCommunicationManager::getWritePacket(int nPacketId) const
{
	CPLCWriteDataPacket *pPacket = nullptr;
	if (m_protocol == PLC_UDP)
	{
		CPLCUDPDataPacketManager *pUdpManager = dynamic_cast<CPLCUDPDataPacketManager *>(m_pPLCManager);
		if (nullptr != pUdpManager)
		{
			pPacket = pUdpManager->getWritePacket(nPacketId);
		}
	}

	return pPacket;
}

CPLCReadDataPacket* CPLCCommunicationManager::getReadPacket() const
{
	CPLCReadDataPacket *pPacket = nullptr;
	if (m_protocol == PLC_UDP)
	{
		CPLCUDPDataPacketManager *pUdpManager = dynamic_cast<CPLCUDPDataPacketManager *>(m_pPLCManager);
		if (nullptr != pUdpManager)
		{
			pPacket = pUdpManager->getReadPacket();
		}
	}

	return pPacket;
}

bool CPLCCommunicationManager::update() const
{
	bool isSuccess = false;
	if (m_pPLCManager)
	{
		try
		{
			isSuccess = m_pPLCManager->update();
		}
		catch (CMemoryException* e)
		{
			TCHAR szError[1024];
			e->GetErrorMessage(szError, 1024);
			::AfxMessageBox(szError);
		}
		catch (...)
		{
			::AfxMessageBox("数据包更新出错了 ----");
		}
	}

	return isSuccess;
}