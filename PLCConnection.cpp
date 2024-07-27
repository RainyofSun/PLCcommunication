#include "StdAfx.h"
#include "PLCConnection.h"


CPLCConnection::CPLCConnection(void)
{
	m_isConnect = false;
	m_errorCode = 0;
	m_protocolType = PLC_TCP;
	m_info.ip = "";
	m_info.port = 0;
}


CPLCConnection::~CPLCConnection(void)
{
}

void CPLCConnection::prepareToConnect(PLCProtocolType plcType, PLCConnectInfo info)
{
	m_protocolType = plcType;
	m_info = info;
}

bool CPLCConnection::plcIsConnect()
{

	return m_isConnect;
}
