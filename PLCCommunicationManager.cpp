#include "StdAfx.h"
#include "PLCCommunicationManager.h"
#include "PLCTool.h"

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

// PLC连接重试10次，未连接弹窗提示
DWORD __stdcall CPLCCommunicationManager::PLCConnectThread(LPVOID lpParameter)
{
	int i = 0;
	while (!CPLCCommunicationManager::getInstance()->m_pPLCManager->connectPLC())
	{
		Sleep(2000);
		if (i ++ > 10)
		{
			AfxMessageBox("PLC Connect Fail!");
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

bool CPLCCommunicationManager::setPLCCommunicationInfo(std::vector<PLCConnectInfo> connectInfos, PLCProtocolType type /* = PLC_UDP */)
{
	if (m_bIsInitialized)
	{
		return true;
	}

	m_protocol = type;

	if (type == PLC_UDP)
	{
		m_pPLCManager = new CPLCUDPDataPacketManager(connectInfos[0], connectInfos[1]);
	}

	if (type == PLC_MODBUS)
	{
		m_pPLCManager = new CPLCModbusDataManager(connectInfos);
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

bool CPLCCommunicationManager::readBoolValue(CString address, bool *bValue)
{
	SplitDataStruct m_splitStruct = spliteAddress(address);
	if (m_splitStruct.isValid == false)
	{
		return false;
	}

	CPLCModbusDataManager *connect = dynamic_cast<CPLCModbusDataManager *>(m_pPLCManager);

	// 地址段是否开启
	bool isValid = connect->addressGroupIsValid(m_splitStruct.mark, m_splitStruct.type, m_splitStruct.address);
	if (!isValid)
	{
		AfxMessageBox("当前地址段未在配置文件中开启");
		return false;
	}

	uint16_t result = connect->getAddressValue(m_splitStruct.mark, m_splitStruct.type, m_splitStruct.address);

	// 转换为bool
	if ((result & 0x01) == 0x01)
	{
		*bValue = true;
	}
	else
	{
		*bValue = false;
	}
	
	return true;
}

bool CPLCCommunicationManager::writeBoolValue(CString address, bool bValue)
{
	SplitDataStruct m_splitStruct = spliteAddress(address);
	if (m_splitStruct.isValid == false)
	{
		return false;
	}

	CPLCModbusDataManager *connect = dynamic_cast<CPLCModbusDataManager *>(m_pPLCManager);

	// 地址段是否开启
	bool isValid = connect->addressGroupIsValid(m_splitStruct.mark, m_splitStruct.type, m_splitStruct.address);
	if (!isValid)
	{
		AfxMessageBox("当前地址段未在配置文件中开启");
		return false;
	}

	sSetData m_sd;
	m_sd.mark = m_splitStruct.mark;
	m_sd.type = m_splitStruct.type;
	m_sd.valueType = PLC_Value_bool;
	m_sd.b = bValue;

	return connect->setValueToPLC(m_splitStruct.address, m_sd);
}

bool CPLCCommunicationManager::readShortValue(CString address, short *shValue)
{
	SplitDataStruct m_splitStruct = spliteAddress(address);
	if (m_splitStruct.isValid == false)
	{
		return false;
	}

	CPLCModbusDataManager *connect = dynamic_cast<CPLCModbusDataManager *>(m_pPLCManager);
	
	// 地址段是否开启
	bool isValid = connect->addressGroupIsValid(m_splitStruct.mark, m_splitStruct.type, m_splitStruct.address);
	if (!isValid)
	{
		AfxMessageBox("当前地址段未在配置文件中开启");
		return false;
	}

	uint16_t uintValue = connect->getAddressValue(m_splitStruct.mark, m_splitStruct.type, m_splitStruct.address);
	*shValue = uintValue;

	return true;
}

bool CPLCCommunicationManager::writeShortValue(CString address, short shValue)
{
	SplitDataStruct m_splitStruct = spliteAddress(address);
	if (m_splitStruct.isValid == false)
	{
		return false;
	}

	CPLCModbusDataManager *connect = dynamic_cast<CPLCModbusDataManager *>(m_pPLCManager);

	// 地址段是否开启
	bool isValid = connect->addressGroupIsValid(m_splitStruct.mark, m_splitStruct.type, m_splitStruct.address);
	if (!isValid)
	{
		AfxMessageBox("当前地址段未在配置文件中开启");
		return false;
	}

	sSetData m_sd;
	m_sd.mark = m_splitStruct.mark;
	m_sd.type = m_splitStruct.type;
	m_sd.valueType = PLC_Value_short;
	m_sd.sh = shValue;

	return connect->setValueToPLC(m_splitStruct.address, m_sd);
}

bool CPLCCommunicationManager::readIntValue(CString address, int *iValue)
{
	SplitDataStruct m_splitStruct = spliteAddress(address);
	if (m_splitStruct.isValid == false)
	{
		return false;
	}

	CPLCModbusDataManager *connect = dynamic_cast<CPLCModbusDataManager *>(m_pPLCManager);

	// 地址段是否开启
	bool isValid = connect->addressGroupIsValid(m_splitStruct.mark, m_splitStruct.type, m_splitStruct.address);
	if (!isValid)
	{
		AfxMessageBox("当前地址段未在配置文件中开启");
		return false;
	}

	uint16_t uintValue = connect->getAddressValue(m_splitStruct.mark, m_splitStruct.type, m_splitStruct.address);
	if (m_splitStruct.address < COMM_MAX_ADDR - 1)
	{
		uint16_t uintNextValue = connect->getAddressValue(m_splitStruct.mark, m_splitStruct.type, m_splitStruct.address + 1);
		*iValue = (((uint32_t)uintNextValue << 16) + uintValue);
	}

	return true;
}

bool CPLCCommunicationManager::writeIntValue(CString address, int iValue)
{
	SplitDataStruct m_splitStruct = spliteAddress(address);
	if (m_splitStruct.isValid == false)
	{
		return false;
	}

	CPLCModbusDataManager *connect = dynamic_cast<CPLCModbusDataManager *>(m_pPLCManager);

	// 地址段是否开启
	bool isValid = connect->addressGroupIsValid(m_splitStruct.mark, m_splitStruct.type, m_splitStruct.address);
	if (!isValid)
	{
		AfxMessageBox("当前地址段未在配置文件中开启");
		return false;
	}

	sSetData m_sd;
	m_sd.mark = m_splitStruct.mark;
	m_sd.type = m_splitStruct.type;
	m_sd.valueType = PLC_Value_int;
	m_sd.i = iValue;

	return connect->setValueToPLC(m_splitStruct.address, m_sd);
}

bool CPLCCommunicationManager::readFloatValue(CString address, float *fValue)
{
	SplitDataStruct m_splitStruct = spliteAddress(address);
	if (m_splitStruct.isValid == false)
	{
		return false;
	}

	CPLCModbusDataManager *connect = dynamic_cast<CPLCModbusDataManager *>(m_pPLCManager);

	// 地址段是否开启
	bool isValid = connect->addressGroupIsValid(m_splitStruct.mark, m_splitStruct.type, m_splitStruct.address);
	if (!isValid)
	{
		AfxMessageBox("当前地址段未在配置文件中开启");
		return false;
	}

	uint16_t uintValue = connect->getAddressValue(m_splitStruct.mark, m_splitStruct.type, m_splitStruct.address);
	if (m_splitStruct.address < COMM_MAX_ADDR - 1)
	{
		uint16_t uintNextValue = connect->getAddressValue(m_splitStruct.mark, m_splitStruct.type, m_splitStruct.address + 1);
		uint32_t uint32value = (((uint32_t)uintNextValue << 16) + uintValue);
		memcpy(fValue, &uint32value, sizeof(float));
	}
	return true;
}

bool CPLCCommunicationManager::writeFloatValue(CString address, float fValue)
{
	SplitDataStruct m_splitStruct = spliteAddress(address);
	if (m_splitStruct.isValid == false)
	{
		return false;
	}

	CPLCModbusDataManager *connect = dynamic_cast<CPLCModbusDataManager *>(m_pPLCManager);

	// 地址段是否开启
	bool isValid = connect->addressGroupIsValid(m_splitStruct.mark, m_splitStruct.type, m_splitStruct.address);
	if (!isValid)
	{
		AfxMessageBox("当前地址段未在配置文件中开启");
		return false;
	}

	sSetData m_sd;
	m_sd.mark = m_splitStruct.mark;
	m_sd.type = m_splitStruct.type;
	m_sd.valueType = PLC_Value_float;
	m_sd.f = fValue;

	return connect->setValueToPLC(m_splitStruct.address, m_sd);
}

AddrType CPLCCommunicationManager::getAddressType(CString type)
{
	if (type == "X")
	{
		return Addr_X;
	}

	if (type == "Y")
	{
		return Addr_Y;
	}

	if (type == "M")
	{
		return Addr_M;
	}

	if (type == "D")
	{
		return Addr_D;
	}

	return ADDR_TYPE_SUM;
}

SplitDataStruct CPLCCommunicationManager::spliteAddress(CString address)
{
	SplitDataStruct sData;
	sData.isValid = false;

	if (m_protocol != PLC_MODBUS)
	{
		return sData;
	}

	if (address.GetLength() != PLC_KEY_NAME_LENGTH)
	{
		return sData;
	}

	std::vector<CString> m_address_split = CPLCTool::SplitCString(address);
	if (m_address_split.size() != 3)
	{
		return sData;
	}

	CString m_temp = m_address_split.front().Mid(3, 1);
	int plc_num = atoi(m_temp);
	if (plc_num >= PLC_MARK_NUM)
	{
		AfxMessageBox("PLC 标志位错误， 标志位 = %d", plc_num);
		return sData;
	}

	PLCMARK mark = static_cast<PLCMARK>(plc_num);
	AddrType add_type = getAddressType(m_address_split[1]);

	if (add_type == ADDR_TYPE_SUM)
	{
		return sData;
	}

	sData.isValid = true;
	sData.address = atoi(m_address_split.back());
	sData.mark = mark;
	sData.type = add_type;

	return sData;
}