#pragma once
#include <stdint.h>

///////////////////////////////// Modbus 定义 /////////////////////////////////////////
#define  PLCConfigPath  "..\\Data\\Config\\AddressValid.dat"

// 定义PLC从站位置 -- 会根据枚举数开辟缓存
enum PLCMARK 
{
	PLC_0,
	PLC_1,
	PLC_MARK_NUM // 枚举数，不可直接使用
};

// 四种不同的变量储存区 X Y M D
enum AddrType
{
	Addr_X = 0,
	Addr_Y,
	Addr_M,
	Addr_D,
	ADDR_TYPE_SUM
};

enum PLCValueType
{
	PLC_Value_bool = 0,
	PLC_Value_int,
	PLC_Value_short,
	PLC_Value_float,
	PLC_Value_double,
	PLC_Value_uint8,
	PLC_Value_uint16,
};

enum PLCOperateType
{
	PLC_Operation_Read = 0,
	PLC_Operation_Write,
};

// 每次通讯 读取的地址数
#define  COMM_READ_LENGTH 100
// 最大读取次数
#define  COMM_READ_MAX 80

// 最大地址 （地址从0开始）
#define  COMM_MAX_ADDR 8000
#define  COMM_MAX_XY_ADDR 80

struct sData
{ 
	bool b;
	int i; 
	short sh; 
	float f; 
	double db;
	uint8_t uint8;
	uint16_t uint16;
};

// VM Get/Set Value 时，使用的Key长度 PLC1-D-0015 （ Length == 11 )
#define PLC_KEY_NAME_LENGTH 11

struct sSetData
{ 
	PLCMARK mark;
	AddrType type;
	PLCValueType valueType;
	bool b;
	int i; 
	short sh; 
	float f; 
	double db;
	uint8_t uint8;
	uint16_t uint16;
};

struct SplitDataStruct 
{
	PLCMARK mark;
	AddrType type;
	int address;
	bool isValid;
};

//ERR DEFINE
#define    PLC_SUCCESS                       0     //成功
#define    PLC_CONNECT_FAIL                 -10   //网络连接失败
#define    PLC_READ_FAIL                    -11   //读数据失败

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
	// PLC标识，仅modbus使用
	PLCMARK mark;

	void initialization() 
	{
		ip = "";
		port = 0;
		mark = PLC_0;
	}

	PLCConnectInfo()
	{
		initialization();
	}
};

/////////////////////////////////// 命令数据包定义 ///////////////////////////////////////
#define ORDER_PACKET_ID 1					// 命令数据包ID
#define ORDER_PACKET_SIZE 512				// 命令数据包数据最大量

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