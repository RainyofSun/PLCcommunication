#pragma once

/// PLC 协议类型
enum PLCProtocolType 
{
	PLC_TCP = 0,
	PLC_UDP,
	PLC_MODBUS
};

/// PLC 连接信息
struct PLCConnectInfo
{
	CString ip;
	int port;

	void initialization() 
	{
		ip = "";
		port = 0;
	}

	PLCConnectInfo()
	{
		initialization();
	}
};

// *****UDP IP和端口号*****
#define UDP_SEND_IP "192.168.1.250"			// UDP发送IP
#define UDP_RECEIVE_IP "192.168.1.20"		// UDP接收IP
#define UDP_PORT 5050						// UDP端口号

/////////////////////////////////// 命令数据包定义 ///////////////////////////////////////
#define ORDER_PACKET_ID 1					// 命令数据包ID
#define ORDER_PACKET_SIZE 512				// 命令数据包数据最大量

enum machine_type
{
	machine_type_test,
	machine_type_6100 = 1,		// 6100 UDP通信方式（命令包与数据包合成一起发送）
	machine_type_802,			// 1201 ADS通信方式（快速包与其他数据一起发送方式）
	machine_type_1201,			// 1201 UDP通信方式（参数数据包按需发送，命令包一直发送）
	machine_type_1221,			// 1221 UDP通信方式（参数数据包按需发送，命令包一直发送）
};

// 命令数据包
struct OrderPackage
{
	char data[ORDER_PACKET_SIZE];	// 数据

	void initParam()
	{
		for (int i=0; i<ORDER_PACKET_SIZE; i++)
		{
			data[i] = 0;
		}
	}

	OrderPackage()
	{
		initParam();
	}

	OrderPackage operator=(const OrderPackage &other)
	{
		memcpy(data, other.data, sizeof(data));

		return *this;
	}
};

///////////////////////////////// 参数数据包定义 /////////////////////////////////////////
// 数据包 int 类型数据最大量
#define  PACKAGE_MAX_SIZE				512
// 数据包数据类型字节大小
#define  PACKAGE_DATA_TYPE_BYTE_SIZE	4
// 通讯时float 数据与int数据转换精度
#define CONVERT_VALUE_ACCURACY			10000
// 初始数据包ID
#define PACKET_ID_NONE					0
// int 值位数
#define BIT_SIZE						32

// 发送数据ID
enum write_packet_id
{
	write_packet_id_1 = 2,
	write_packet_id_2,
	write_packet_id_3,
	write_packet_id_4,
	write_packet_id_5
};

// 接收数据包ID
enum read_packet_id
{
	read_packet_id = 1,
	read_packet_quick = 2
};

// 参数数据包
struct PLCPackage 
{
	int data[PACKAGE_MAX_SIZE];

	void initialization()
	{
		for (int i = 0; i < PACKAGE_MAX_SIZE; i ++)
		{
			data[i] = 0;
		}
	}

	PLCPackage()
	{
		initialization();
	}

	PLCPackage operator=(const PLCPackage &other)
	{
		memcpy(data, other.data, sizeof(data));
		return *this;
	}
};
