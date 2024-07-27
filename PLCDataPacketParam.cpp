#include "StdAfx.h"
#include "PLCDataPacketParam.h"
#include "MemoryShare.h"
#include "PLCTool.h"

CPLCDataPacketParam::CPLCDataPacketParam(CPLCConnection *connect, int nPacketId, char *name /* = nullptr */): CPLCDataPacket(connect)
{
	m_nPacketID = nPacketId;
	m_nSize = PACKAGE_MAX_SIZE;
	m_nByteSize = sizeof(PLCPackage);
	memcpy(m_buff, &m_sPackage, m_nByteSize);

	m_isOpenShare = false;

	if (name && ("" != name))
	{
		m_sPackage = static_cast<PLCPackage*>(CMemoryShare::openShare(name, m_nByteSize));
		m_isOpenShare = true;
	}
	else
	{
		m_sPackage = new PLCPackage;
	}
}


CPLCDataPacketParam::~CPLCDataPacketParam(void)
{
	if (!m_isOpenShare)
	{
		delete m_sPackage;
	}

	m_sPackage = nullptr;
}

void CPLCDataPacketParam::setPacketSize(int nSize)
{
	if (nSize > 0 && nSize <= PACKAGE_MAX_SIZE)
	{
		m_nSize = nSize;
		m_nByteSize = m_nSize * PACKAGE_DATA_TYPE_BYTE_SIZE; // byte ת long
	}
}

bool CPLCDataPacketParam::setPackage(int nPackageID, const PLCPackage &sPackage) const
{
	bool isSuccess = false;

	if (nullptr != m_sPackage)
	{
		if (m_nPacketID == nPackageID)
		{
			memcpy(m_sPackage, &sPackage, m_nByteSize);
			isSuccess = true;
		}
	}

	return isSuccess;
}

PLCPackage CPLCDataPacketParam::getPackage() const
{
	return *m_sPackage;
}

bool CPLCDataPacketParam::setValue(int id, int nValue) const
{
	bool isSuccess = false;
	if (nullptr != m_sPackage)
	{
		if (id >= 0 && id < PACKAGE_MAX_SIZE)
		{
			m_sPackage->data[id] = nValue;
			isSuccess = true;
		}
	}

	return isSuccess;
}

int CPLCDataPacketParam::getValue(int id) const
{
	int nValue = 0;
	if (nullptr != m_sPackage)
	{
		if (id >= 0 && id < PACKAGE_MAX_SIZE)
		{
			nValue = m_sPackage->data[id];
		}
	}

	return nValue;
}

bool CPLCDataPacketParam::setBit(int id, int bitID, bool bValue) const
{
	bool isSuccess = false;

	if (nullptr != m_sPackage)
	{
		if (id >= 0 && id < PACKAGE_MAX_SIZE && bitID >= 0 && bitID < BIT_SIZE)
		{
			m_sPackage->data[id] = CPLCTool::reviseBit(m_sPackage->data[id], bitID, bValue);
			isSuccess = true;
		}
	}

	return isSuccess;
}

int CPLCDataPacketParam::getBit(int id, int bitId) const
{
	if (id < 0 || id >= PACKAGE_MAX_SIZE || bitId < 0 || bitId >= BIT_SIZE)
	{
		return -1;
	}
	
	if (nullptr == m_sPackage)
	{
		return -1;
	}

	return CPLCTool::getBit(id, bitId);
}

bool CPLCDataPacketParam::setOrderValue( int id, int nValue) const
{
	if (-1000 == nValue)
	{
		if (m_sPackage->data[id] < -128 || m_sPackage->data[id] > 127)
		{
			m_sPackage->data[id] = -128;
		}
		if (-1 == m_sPackage->data[id])
		{
			m_sPackage->data[id] += 2;

		}else
		{
			m_sPackage->data[id] += 1;
		}

	}else
	{
		if (nValue < -128 || nValue > 127)
		{
			return false;
		}
		m_sPackage->data[id] = nValue;
	}

	return true;
}