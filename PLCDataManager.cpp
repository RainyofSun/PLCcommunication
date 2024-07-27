#include "StdAfx.h"
#include "PLCDataManager.h"


CPLCDataManager::CPLCDataManager(void)
{
	m_bIsInitialized = false;
	m_bIsConnectPLC = false;
	m_bIsFinishFirstCommunication = false;
}


CPLCDataManager::~CPLCDataManager(void)
{
	release();
}

bool CPLCDataManager::isInitialized() const
{
	return m_bIsInitialized;
}

bool CPLCDataManager::isConnect()
{
	return m_bIsConnectPLC;
}

bool CPLCDataManager::isFinishFirstCommunction() const
{
	return m_bIsFinishFirstCommunication;
}

void CPLCDataManager::release()
{
	releaseConnect();
}