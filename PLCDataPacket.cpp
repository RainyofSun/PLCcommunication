#include "StdAfx.h"
#include "PLCDataPacket.h"


CPLCDataPacket::CPLCDataPacket(CPLCConnection *connect)
{
	m_connect = connect;
	m_nByteSize = 0;
}


CPLCDataPacket::~CPLCDataPacket(void)
{
}
