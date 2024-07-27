#include "StdAfx.h"
#include "UDPConnection.h"


CUDPConnection::CUDPConnection(void)
{
	m_isInitialized = false;
	m_isConnect = false;
}


CUDPConnection::~CUDPConnection(void)
{
	closeConnectPLC();
}

void CUDPConnection::prepareToConnect(PLCProtocolType plcType, PLCConnectInfo info)
{
	// 给定协议标准号（IPV4）
	m_address.sin_family = AF_INET;
	// 配置发送目的端口号
	m_address.sin_port = htons(info.port);
	// 配置发送目的IP地址
	m_address.sin_addr.s_addr = inet_addr(info.ip.GetString());
	m_addressSize = sizeof(m_address);
	
	if (m_isConnect)
	{
		closeConnectPLC();
	}

	// 初始化网络通信功能
	WSADATA wsaData;
	if (0 != WSAStartup(MAKEWORD(2,2), &wsaData))
	{
		m_errorCode = WSAGetLastError();
		return;
	}

	// 打开UDP网络接口
	m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (m_socket == INVALID_SOCKET)
	{
		m_errorCode = WSAGetLastError();
		return;
	}

	// 将接口设置为非阻塞方式，否则程序将长时间等待接收数据
	int iMode = 1;
	if (SOCKET_ERROR == ioctlsocket(m_socket, FIONBIO, reinterpret_cast<u_long*>(&iMode)))
	{
		m_errorCode = WSAGetLastError();
		return;
	}

	m_isInitialized = true;
}

bool CUDPConnection::startConnectPLC()
{
	if (!m_isInitialized)
	{
		m_errorCode = 1;
		return false;
	}

	if (m_isConnect)
	{
		return true;
	}

	// 绑定接收到接口
	if (SOCKET_ERROR == bind(m_socket, reinterpret_cast<SOCKADDR*>(&m_address), m_addressSize))
	{
		m_errorCode = WSAGetLastError();
	}
	else
	{
		m_isConnect = true;
	}

	return m_isConnect;
}

void CUDPConnection::closeConnectPLC()
{
	closesocket(m_socket);
	m_isConnect = false;
}

CString CUDPConnection::plcError()
{
	CString err;
	switch(m_errorCode)
	{
	case 0:
		err = _T("");
		break;
	case 1:
		err = _T("未设置参数/未初始化socket");
		break;
	case 2:
		err = _T("未绑定端口");
		break;
	case 10004:
		err = _T("阻塞状态被打断");
		break;
	case 10022:
		err = _T("未绑定");
		break;
	case 10035:
		err = _T("非阻塞SOCKET的操作不能立即执行");
		break;
	case 10038:
		err = _T("socket不存在");
		break;
	case 10040:
		err = _T("缓冲区不够");
		break;
	case 10048:
		err = _T("端口已占用");
		break;
	case 10049:
		err = _T("不能分配请求的地址");
		break;
	case 10093:
		err = _T("网络通信功能WSAStartup未初始化");
		break;
	default:
		err.Format(_T("未知错误:%d"), m_errorCode);
		break;
	}

	return err;
}

bool CUDPConnection::sendMessage(char *buff, int nSize)
{
	bool isSuccess = false;

	if (!m_isInitialized)
	{
		m_errorCode = 1;
		return isSuccess;
	}

	if (!m_isConnect)
	{
		m_errorCode = 2;
		return isSuccess;
	}

	/*
	int PASCAL FAR sendto( SOCKET s, const char FAR* buf, int len, int flags, const struct sockaddr FAR* to,int tolen);
		s：一个标识套接口的描述字。
		buf：包含待发送数据的缓冲区。
		len：buf缓冲区中数据的长度。
		flags：调用方式标志位。
		to：（可选）指针，指向目的套接口的地址。
		tolen：to所指地址的长度。
	*/
	const int nSendLen = sendto(m_socket, buff, nSize, 0, reinterpret_cast<SOCKADDR*>(&m_address), m_addressSize);
	if (SOCKET_ERROR == nSendLen)
	{
		m_errorCode = WSAGetLastError();
		isSuccess = false;
	}
	else
	{
		isSuccess = true;
	}

	return isSuccess;
}

bool CUDPConnection::receiveMessage(char *buff, int nSize)
{
	bool isSuccess = false;
	if (!m_isInitialized)
	{
		m_errorCode = 1;
		return isSuccess;
	}

	if (!m_isConnect)
	{
		m_errorCode = 2;
		return isSuccess;
	}

	/*
	int recvfrom(int s, void *buf, int len, unsigned int flags, struct sockaddr *from,int *fromlen);
	　	s：socket描述符；
		buf：UDP数据报缓存区（包含所接收的数据）； 
		len：缓冲区长度。 
		flags：调用操作方式（一般设置为0）。 
		from：指向发送数据的客户端地址信息的结构体（sockaddr_in需类型转换）；
		fromlen：指针，指向from结构体长度值。
	*/
	const int nReceiveLen = recvfrom(m_socket, buff, nSize, 0, reinterpret_cast<SOCKADDR*>(&m_address), &m_addressSize);
	if (SOCKET_ERROR == nReceiveLen)
	{
		m_errorCode = WSAGetLastError();
		isSuccess = false;
	}
	else
	{
		isSuccess = true;
	}

	return isSuccess;
}