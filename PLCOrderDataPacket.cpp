#include "StdAfx.h"
#include "PLCOrderDataPacket.h"


CPLCOrderDataPacket::CPLCOrderDataPacket(CPLCConnection *connect): CPLCDataPacket(connect)
{
	m_sPackage.data[0] = 1;
	m_nByteSize = sizeof(OrderPackage);
	memcpy(m_buff, &m_sPackage, m_nByteSize);
}

CPLCOrderDataPacket::~CPLCOrderDataPacket(void)
{
}

void CPLCOrderDataPacket::setPackage(const OrderPackage &sOrderPackage)
{
	m_sPackage = sOrderPackage;
}

OrderPackage CPLCOrderDataPacket::getPackage() const
{
	return m_sPackage;
}

bool CPLCOrderDataPacket::setOrderValue( int nOrderNum, int nValue/*=-1000*/ )
{
	if (nOrderNum<0 || nOrderNum>=ORDER_PACKET_SIZE)
	{
		return false;
	}

	if (-1000 == nValue)
	{
		if (-1 == m_sPackage.data[nOrderNum])
		{
			m_sPackage.data[nOrderNum] += 2;

		}
		else
		{
			m_sPackage.data[nOrderNum] += 1;
		}

	}
	else
	{
		if (nValue < -128 || nValue >127)
		{
			return false;
		}
		m_sPackage.data[nOrderNum] = nValue;
	}

	return true;
}

int CPLCOrderDataPacket::getOrderValue(int nOrderNum)
{
	int nValue = 0;
	if (nOrderNum >= 0 && nOrderNum < ORDER_PACKET_SIZE)
	{
		nValue = m_sPackage.data[nOrderNum];
	}

	return nValue;
}

bool CPLCOrderDataPacket::updatePacket()
{
	bool isSuccess = false;
	if (m_connect)
	{
		m_sPackage.data[0] = ORDER_PACKET_ID;
		if (m_connect->sendMessage(m_sPackage.data, m_nByteSize))
		{
			isSuccess = true;
		}
		else
		{
			cout << "\n" << "ReadDataPacket update Error:" << m_connect->plcError() << "\n";
		}
	}

	return isSuccess;
}