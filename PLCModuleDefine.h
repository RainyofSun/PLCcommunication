#pragma once

/// PLC Э������
enum PLCProtocolType 
{
	PLC_TCP = 0,
	PLC_UDP,
	PLC_MODBUS
};

/// PLC ������Ϣ
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

// *****UDP IP�Ͷ˿ں�*****
#define UDP_SEND_IP "192.168.1.250"			// UDP����IP
#define UDP_RECEIVE_IP "192.168.1.20"		// UDP����IP
#define UDP_PORT 5050						// UDP�˿ں�

/////////////////////////////////// �������ݰ����� ///////////////////////////////////////
#define ORDER_PACKET_ID 1					// �������ݰ�ID
#define ORDER_PACKET_SIZE 512				// �������ݰ����������

enum machine_type
{
	machine_type_test,
	machine_type_6100 = 1,		// 6100 UDPͨ�ŷ�ʽ������������ݰ��ϳ�һ���ͣ�
	machine_type_802,			// 1201 ADSͨ�ŷ�ʽ�����ٰ�����������һ���ͷ�ʽ��
	machine_type_1201,			// 1201 UDPͨ�ŷ�ʽ���������ݰ����跢�ͣ������һֱ���ͣ�
	machine_type_1221,			// 1221 UDPͨ�ŷ�ʽ���������ݰ����跢�ͣ������һֱ���ͣ�
};

// �������ݰ�
struct OrderPackage
{
	char data[ORDER_PACKET_SIZE];	// ����

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

///////////////////////////////// �������ݰ����� /////////////////////////////////////////
// ���ݰ� int �������������
#define  PACKAGE_MAX_SIZE				512
// ���ݰ����������ֽڴ�С
#define  PACKAGE_DATA_TYPE_BYTE_SIZE	4
// ͨѶʱfloat ������int����ת������
#define CONVERT_VALUE_ACCURACY			10000
// ��ʼ���ݰ�ID
#define PACKET_ID_NONE					0
// int ֵλ��
#define BIT_SIZE						32

// ��������ID
enum write_packet_id
{
	write_packet_id_1 = 2,
	write_packet_id_2,
	write_packet_id_3,
	write_packet_id_4,
	write_packet_id_5
};

// �������ݰ�ID
enum read_packet_id
{
	read_packet_id = 1,
	read_packet_quick = 2
};

// �������ݰ�
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
