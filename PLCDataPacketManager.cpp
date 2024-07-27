#include "StdAfx.h"
#include "PLCDataPacketManager.h"


CPLCDataPacketManager::CPLCDataPacketManager(void)
{
	m_pSendConnection = nullptr;
	m_pReceiveConnection = nullptr;

	m_pOrderPacket = nullptr;
	m_pReadDataPacket = nullptr;
}


CPLCDataPacketManager::~CPLCDataPacketManager(void)
{
	release();
}

CPLCOrderDataPacket* CPLCDataPacketManager::getOrderPacket() const
{
	return m_pOrderPacket;
}

CPLCWriteDataPacket* CPLCDataPacketManager::getWritePacket(int nPacketId)
{
	CPLCWriteDataPacket *pWriteDataPacket = nullptr;
	
	if ((m_writeDataPacketMap.find(nPacketId) != m_writeDataPacketMap.end()) || createWritePacket(nPacketId))
	{
		pWriteDataPacket = m_writeDataPacketMap[nPacketId];
	}

	return pWriteDataPacket;
}

CPLCReadDataPacket* CPLCDataPacketManager::getReadPacket() const
{
	return m_pReadDataPacket;
}

void CPLCDataPacketManager::releaseWritePacket()
{
	if (!m_writeDataPacketMap.empty())
	{
		for (auto i = m_writeDataPacketMap.begin(); i != m_writeDataPacketMap.end(); ++i)
		{
			if (i->second != nullptr)
			{
				delete i->second;
				i->second = nullptr;
			}
		}

		m_writeDataPacketMap.clear();
	}
}

void CPLCDataPacketManager::releasePacket()
{
	releaseWritePacket();

	if (m_pOrderPacket)
	{
		delete m_pOrderPacket;
		m_pOrderPacket = nullptr;
	}

	if (m_pReadDataPacket)
	{
		delete m_pReadDataPacket;
		m_pReadDataPacket = nullptr;
	}
}

void CPLCDataPacketManager::releaseConnect()
{
	if (m_pSendConnection)
	{
		m_pSendConnection->closeConnectPLC();
		delete m_pSendConnection;
		m_pSendConnection = nullptr;
	}

	if (m_pReceiveConnection)
	{
		m_pReceiveConnection->closeConnectPLC();
		delete m_pReceiveConnection;
		m_pReceiveConnection = nullptr;
	}
}

void CPLCDataPacketManager::release()
{
	releasePacket();
	__super::release();
}