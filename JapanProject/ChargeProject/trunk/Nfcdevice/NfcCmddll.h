// AQdeviceDll.cpp : DLL ���� ���α׷��� ���� ������ �Լ��� �����մϴ�.
//

#include "stdafx.h"

//��Ʈ ��ȣ
#define USBPORT	1001
//�ø��� ��Ʈ �ӵ�
#define BAUD		115200

enum NFCErrorCode{
DE_OK									= 0x00,
DE_NO_TAG_ERROR    						= 0x02,
DE_CRC_ERROR       						= 0x03,
DE_EMPTY								= 0x04 ,//(NO IC CARD ERROR)	
DE_AUTHENTICATION_ERROR     			= 0x05,
DE_NO_POWER								= 0x05,
DE_PARITY_ERROR    						= 0x06,
DE_CODE_ERROR      						= 0x07,
DE_SERIAL_NUMBER_ERROR      			= 0x08,
DE_KEY_ERROR       						= 0x09,
DE_NOT_AUTHENTICATION_ERROR    			= 0x0A,
DE_BIT_COUNT_ERROR   					= 0x0B,	
DE_BYTE_COUNT_ERROR 					= 0x0C,		
DE_TRANSFER_ERROR      					= 0x0E,	
DE_WRITE_ERROR       					= 0x0F,	
DE_INCREMENT_ERROR         				= 0x10,	
DE_DECREMENT_ERROR         				= 0x11,	
DE_READ_ERROR         					= 0x12,	
DE_OVERFLOW_ERROR     					= 0x13,	
DE_POLLING_ERROR         				= 0x14,	
DE_FRAMING_ERROR         				= 0x15,	
DE_ACCESS_ERROR        					= 0x16,	
DE_UNKNOWN_COMMAND_ERROR				= 0x17,	
DE_ANTICOLLISION_ERROR         			= 0x18,	
DE_INITIALIZATION_ERROR					= 0x19,	
DE_INTERFACE_ERROR   		 			= 0x1A,	
DE_ACCESS_TIMEOUT_ERROR					= 0x1B,	
DE_NO_BITWISE_ANTICOLLISION_ERROR		= 0x1C,
DE_FILE_ERROR							= 0x1D,
DE_INVAILD_BLOCK_ERROR					= 0x20,
DE_ACK_COUNT_ERROR						= 0x21,
DE_NACK_DESELECT_ERROR					= 0x22,
DE_NACK_COUNT_ERROR						= 0x23,
DE_SAME_FRAME_COUNT_ERROR				= 0x24,
DE_RCV_BUFFER_TOO_SMALL_ERROR			= 0x31,
DE_RCV_BUFFER_OVERFLOW_ERROR			= 0x32,
DE_RF_ERROR								= 0x33,
DE_PROTOCOL_ERROR						= 0x34,
DE_USER_BUFFER_FULL_ERROR				= 0x35,
DE_BUADRATE_NOT_SUPPORTED				= 0x36,
DE_INVAILD_FORMAT_ERROR					= 0x37,
DE_LRC_ERROR							= 0x38,
DE_FRAMERR								= 0x39,
DE_WRONG_PARAMETER_VALUE				= 0x3C,
DE_INVAILD_PARAMETER_ERROR				= 0x3D,
DE_UNSUPPORTED_PARAMETER				= 0x3E,
DE_UNSUPPORTED_COMMAND					= 0x3F,
DE_INTERFACE_NOT_ENABLED				= 0x40,
DE_ACK_SUPPOSED							= 0x41,
DE_NACK_RECEVIED						= 0x42,
DE_BLOCKNR_NOT_EQUAL					= 0x43,
DE_TARGET_SET_TOX						= 0x44,
DE_TARGET_RESET_TOX						= 0x45,
DE_TARGET_DESELECTED					= 0x46,
DE_TARGET_RELEASED						= 0x47,
DE_ID_ALREADY_IN_USE            		= 0x48,
DE_INSTANCE_ALREADY_IN_USE				= 0x49,
DE_ID_NOT_IN_USE						= 0x4A,
DE_NO_ID_AVAILABLE              		= 0x4B,
DE_OTHER_ERROR							= 0x4C,
DE_INVALID_READER_STATE					= 0x4D,
DE_MI_JOINER_TEMP_ERROR					= 0x4C,
DE_NOTYET_IMPLEMENTED					= 0x64,
DE_FIFO_ERROR							= 0x6D,
DE_WRONG_SELECT_COUNT					= 0x72,
DE_WRONG_VALUE							= 0x7B,
DE_VALERR								= 0x7C,
DE_RE_INIT								= 0x7E,
DE_NO_INIT								= 0x7F,
DE_NO_EVENTMSG							= 0xF1,
APP_INVALID_PORT						= 0x03E8,
APP_STX_ERROR							= 0x03E9,
APP_INVALID_LENGTH_ERROR				= 0x03EA,
APP_TIMEOUT_ERROR						= 0x03EB,
APP_CRC_ERROR							= 0x03EC,
APP_LRC_ERROR							= 0x03ED,
APP_RW_ERROR							= 0x03EE,
APP_ETX_ERROR							= 0x03EF,
APP_USB_WRITE_ERROR						= 0x03F0,
APP_USB_READ_ERROR						= 0x03F1,
APP_INVALID_SENDDATA_LEN				= 0x03F2,
APP_INVALID_SENDBUF_SIZE				= 0x03F3,
APP_TOO_SMALL_RECVBUF					= 0x03F4,
APP_SENDBUF_OVERFLOW					= 0x03F5,
APP_MODEM_ERROR_START					= 0x03F6,
APP_LIBLOAD_ERROR						= 0x0F00
};

enum CardFindType{
	NFCCONNECTCARD_ATYPE = 0x41,
	NFCCONNECTCARD_BTYPE = 0x42,
	NFCCONNECTCARD_MTYPE = 0x4D,
	NFCCONNECTCARD_OTHER = 0xFF
};
//Atyep Event Func
typedef DWORD (WINAPI *ATYPEEVENTFUNC)(LPVOID lpThreadParameter, byte* eventdata, int eventdatalen);
typedef ATYPEEVENTFUNC ATYPEEVENTFUNC_ROUTINE;
//Mifare Event Func
typedef DWORD (WINAPI *MTYPEEVENTFUNC)(LPVOID lpThreadParameter, byte* eventdata, int eventdatalen);
typedef MTYPEEVENTFUNC MTYPEEVENTFUNC_ROUTINE;

//ī�� �������� ���� Event Func
typedef DWORD (WINAPI *DISCONNECTEVENTFUNC)(LPVOID lpThreadParameter, byte* eventdata, int eventdatalen);
typedef DISCONNECTEVENTFUNC DISCONNECTEVENTFUNC_ROUTINE;