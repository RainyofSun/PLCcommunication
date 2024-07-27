#include "StdAfx.h"
#include "PLCReadDataPacket.h"


CPLCReadDataPacket::CPLCReadDataPacket(CPLCConnection *connect, int nPacketId, char *name /* = nullptr */): CPLCDataPacketParam(connect, nPacketId, name) 
{
}


CPLCReadDataPacket::~CPLCReadDataPacket(void)
{
}

bool CPLCReadDataPacket::updatePacket()
{
	bool isSuccess = false;

	if (m_connect)
	{
		if (m_connect->receiveMessage(m_buff, m_nByteSize))
		{
			memcpy(m_sPackage->data, &m_buff, m_nByteSize);
			isSuccess = true;
		}
		else
		{
			cout << "\n" << "ReadDataPacket update Error:" << m_connect->plcError() << "\n";
		}
	}

	return isSuccess;
}