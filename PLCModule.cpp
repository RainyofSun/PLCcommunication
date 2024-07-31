#include "StdAfx.h"
#include "PLCModule.h"
#include "PLCCommunicationManager.h"
#include "PLCOrderDataPacket.h"
#include "PLCWriteDataPacket.h"
#include "PLCReadDataPacket.h"

CPLCModule *CPLCModule::m_instance = nullptr;
CMutex CPLCModule::m_mutex;
CPLCModule::PLCModuleGarbo CPLCModule::garbo;

int CPLCModule::m_sleepTime = 2;
bool CPLCModule::m_bIsCloseThread = false;
HANDLE CPLCModule::m_hThread = nullptr;
bool CPLCModule::m_bIsStartThread = false;

CPLCModule::CPLCModule(void)
{
	m_hThread = CreateThread(NULL, 0, ThreadUpdate, this, CREATE_SUSPENDED, NULL);
}


CPLCModule::~CPLCModule(void)
{
}

void CPLCModule::release()
{
	if (m_instance != nullptr)
	{
		m_bIsCloseThread = true;
		DWORD dwExitCode = 0;

		do 
		{
			dwExitCode = 0;
			GetExitCodeThread(m_hThread, &dwExitCode);
		} while (STILL_ACTIVE == dwExitCode);

		m_instance->m_bIsStartThread = false;
		delete m_instance;
		m_instance = nullptr;
	}
}

CPLCModule *CPLCModule::getInstance()
{
	if (m_instance == nullptr)
	{
		m_mutex.Lock();
		if (m_instance == nullptr)
		{
			m_instance = new CPLCModule();
		}
		m_mutex.Unlock();
	}

	return m_instance;
}

DWORD __stdcall CPLCModule::ThreadUpdate(LPVOID lpParameter)
{
	while(!m_bIsCloseThread)
	{
		if (!CPLCCommunicationManager::getInstance()->update())
		{

		}

		Sleep(m_sleepTime);
	}

	return 0;
}

bool CPLCModule::setPLCCommunicationInfo(PLCConnectInfo receiveInfo, PLCConnectInfo sendInfo, PLCProtocolType type /* = PLC_UDP */)
{
	if (m_bIsStartThread)
	{
		return true;
	}

	return CPLCCommunicationManager::getInstance()->setPLCCommunicationInfo(receiveInfo, sendInfo, type);
}

void CPLCModule::startThread()
{
	if (!m_bIsStartThread)
	{
		m_bIsStartThread = true;
		ResumeThread(m_hThread);
	}
}

bool CPLCModule::isInitialized() const
{
	return CPLCCommunicationManager::getInstance()->isFinishFirstCommunication();
}

bool CPLCModule::orderPacketSetValue(int dataId, int writeValue) const 
{
	bool isSuccess = false;
	CPLCOrderDataPacket *orderPacket = CPLCCommunicationManager::getInstance()->getOrderPacket();
	if (orderPacket != nullptr)
	{
		isSuccess = orderPacket->setOrderValue(dataId, writeValue);
	}

	return isSuccess;
}

bool CPLCModule::orderPacketGetValue(int dataId, int readValue) const
{
	bool isSuccess = false;
	CPLCOrderDataPacket *orderPacket = CPLCCommunicationManager::getInstance()->getOrderPacket();
	if (orderPacket != nullptr)
	{
		readValue = orderPacket->getOrderValue(dataId);
		isSuccess = true;
	}

	return isSuccess;
}

bool CPLCModule::writePacketSetValue(int nPacketId, int dataId, int writeValue) const 
{
	bool isSuccess = false;
	CPLCWriteDataPacket *writePacket = CPLCCommunicationManager::getInstance()->getWritePacket(nPacketId);
	if (writePacket != nullptr)
	{
		isSuccess = writePacket->setValue(dataId, writeValue);
	}
	
	return isSuccess;
}

bool CPLCModule::writePacketGetValue(int nPacketId, int dataId, int readValue) const 
{
	bool isSuccess = false;
	CPLCWriteDataPacket *writePacket = CPLCCommunicationManager::getInstance()->getWritePacket(nPacketId);
	if (writePacket != nullptr)
	{
		readValue = writePacket->getValue(dataId);
		isSuccess = true;
	}

	return isSuccess;
}

bool CPLCModule::readPacketGetValue(int dataId, int readValue) const
{
	bool isSuccess = false;
	CPLCReadDataPacket *readPacket = CPLCCommunicationManager::getInstance()->getReadPacket();
	if (readPacket != nullptr)
	{
		readValue = readPacket->getValue(dataId);
		isSuccess = true;
	}

	return isSuccess;
}