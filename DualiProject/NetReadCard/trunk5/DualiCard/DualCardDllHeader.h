#ifndef _DUALCARDDLL_H_
#define _DUALCARDDLL_H_

#define API_DLL __declspec(dllexport)

#define DEBUGMODE	0		//0: not display debug msg, 1: display debug msg

///////////////////////////////////////////////////////////
//Define
///////////////////////////////////////////////////////////
//return value
#define DE_OK									0
#define DE_NO_TAG_ERROR    						2 //(0x02)
#define DE_CRC_ERROR       						3 //(0x03)
#define DE_EMPTY								4 //(0x04)(NO IC CARD ERROR)	
#define DE_AUTHENTICATION_ERROR     			5 //(0x05)
#define DE_NO_POWER								5 //(0x05)
#define DE_PARITY_ERROR    						6 //(0x06)
#define DE_CODE_ERROR      						7 //(0x07)
#define DE_SERIAL_NUMBER ERROR      			8 //(0x08)
#define DE_KEY_ERROR       						9 //(0x09)
#define DE_NOT_AUTHENTICATION ERROR    			10 //(0x0A)
#define DE_BIT_COUNT_ERROR   					11 //(0x0B)	
#define DE_BYTE_COUNT_ERROR 					12 //(0x0C)		
#define DE_TRANSFER_ERROR      					14 //(0x0E)	
#define DE_WRITE_ERROR       					15 //(0x0F)	
#define DE_INCREMENT_ERROR         				16 //(0x10)	
#define DE_DECREMENT_ERROR         				17 //(0x11)	
#define DE_READ_ERROR         					18 //(0x12)	
#define DE_OVERFLOW_ERROR     					19 //(0x13)	
#define DE_POLLING_ERROR         				20 //(0x14)	
#define DE_FRAMING_ERROR         				21 //(0x15)	
#define DE_ACCESS_ERROR        					22 //(0x16)	
#define DE_UNKNOWN_COMMAND_ERROR				23 //(0x17)	
#define DE_ANTICOLLISION_ERROR         			24 //(0x18)	
#define DE_INITIALIZATION_ERROR					25 //(0x19)	
#define DE_INTERFACE_ERROR   		 			26 //(0x1A)	
#define DE_ACCESS_TIMEOUT_ERROR					27 //(0x1B)	
#define DE_NO_BITWISE_ANTICOLLISION_ERROR		28 //(0x1C)
#define DE_FILE_ERROR							29 //(0x1D)
#define DE_INVAILD_BLOCK_ERROR					32 //(0x20)
#define DE_ACK_COUNT_ERROR						33 //(0x21)
#define DE_NACK_DESELECT_ERROR					34 //(0x22)
#define DE_NACK_COUNT_ERROR						35 //(0x23)
#define DE_SAME_FRAME_COUNT_ERROR				36 //(0x24)
#define DE_RCV_BUFFER_TOO_SMALL_ERROR			49 //(0x31)
#define DE_RCV_BUFFER_OVERFLOW_ERROR			50 //(0x32)
#define DE_RF_ERROR								51 //(0x33)
#define DE_PROTOCOL_ERROR						52 //(0x34)
#define DE_USER_BUFFER_FULL_ERROR				53 //(0x35)
#define DE_BUADRATE_NOT_SUPPORTED				54 //(0x36)
#define DE_INVAILD_FORMAT_ERROR					55 //(0x37)
#define DE_LRC_ERROR							56 //(0x38)
#define DE_FRAMERR								57 //(0x39)
#define DE_WRONG_PARAMETER_VALUE				60 //(0x3C)
#define DE_INVAILD_PARAMETER_ERROR				61 //(0x3D)
#define DE_UNSUPPORTED_PARAMETER				62 //(0x3E)
#define DE_UNSUPPORTED_COMMAND					63 //(0x3F)
#define DE_INTERFACE_NOT_ENABLED				64 //(0x40)
#define DE_ACK_SUPPOSED							65 //(0x41)
#define DE_NACK_RECEVIED						66 //(0x42)
#define DE_BLOCKNR_NOT_EQUAL					67 //(0x43)
#define DE_TARGET_SET_TOX						68 //(0x44)
#define DE_TARGET_RESET_TOX						69 //(0x45)
#define DE_TARGET_DESELECTED					70 //(0x46)
#define DE_TARGET_RELEASED						71 //(0x47)
#define DE_ID_ALREADY_IN_USE            		72 //(0x48)
#define DE_INSTANCE_ALREADY_IN_USE				73 //(0x49)
#define DE_ID_NOT_IN_USE						74 //(0x4A)
#define DE_NO_ID_AVAILABLE              		75 //(0x4B)
#define DE_OTHER_ERROR							76 //(0x4C)
#define DE_INVALID_READER_STATE					77 //(0x4D)
#define DE_MI_JOINER_TEMP_ERROR					78 //(0x4E)
#define DE_NOTYET_IMPLEMENTED					100//(0x64)
#define DE_FIFO_ERROR							109//(0x6D)
#define DE_WRONG_SELECT_COUNT					114//(0x72)
#define DE_WRONG_VALUE							123//(0x7B)
#define DE_VALERR								124//(0x7C)
#define DE_RE_INIT								126//(0x7E)
#define DE_NO_INIT								127//(0x7F)
#define APP_INVALID_PORT						1000
#define APP_STX_ERROR							1001
#define APP_INVALID_LENGTH_ERROR				1002
#define APP_TIMEOUT_ERROR						1003
#define APP_CRC_ERROR							1004
#define APP_LRC_ERROR							1005
#define APP_RW_ERROR							1006
#define APP_ETX_ERROR							1007
#define APP_USB_WRITE_ERROR						1008
#define APP_USB_READ_ERROR						1009
#define APP_INVALID_SENDDATA_LEN				1010
#define APP_INVALID_SENDBUF_SIZE				1011
#define APP_TOO_SMALL_RECVBUF					1012
#define APP_SENDBUF_OVERFLOW					1013
#define APP_MODEM_ERROR_START					1024

///////////////////////////////////////////////////////////
//Function
///////////////////////////////////////////////////////////
//common
extern "C" API_DLL int WINAPI GetErrMsg(int errcode,char* retmsg);
extern "C" API_DLL int WINAPI DE_InitPort(int nPort,int nBaud);
extern "C" API_DLL void WINAPI DE_ClosePort(int nPort);
extern "C" API_DLL int WINAPI DE_Polling(int nPort, int datalen, LPBYTE data, LPINT outlen, LPBYTE lpRes, int timeout = 3000);//default timeout(only use for serial communication):3s
extern "C" API_DLL int WINAPI DE_ByPassCommand(int nPort, int datalen, BYTE Cmd, LPBYTE data, LPINT outlen, LPBYTE lpRes, int timeout = 3000);//default timeout:300ms

//Device control command
extern "C" API_DLL int WINAPI DE_RFOn(int nPort);
extern "C" API_DLL int WINAPI DE_RFOff(int nPort);
extern "C" API_DLL int WINAPI DE_RFReset(int nPort);
extern "C" API_DLL int WINAPI DE_BuzzerOff(int nPort);
extern "C" API_DLL int WINAPI DE_BuzzerOn(int nPort);
extern "C" API_DLL int WINAPI DE_GetVersion(int nPort, LPINT outlen, LPBYTE lpRes);

//Type C
extern "C" API_DLL int WINAPI DEC_Transparent(int nPort, int datalen, LPBYTE data, LPINT outlen, LPBYTE lpRes, BYTE tout = 0xB0);//default timeout:300ms
extern "C" API_DLL int WINAPI DEC_Polling_NoENC(int nPort, LPBYTE systemcode, BYTE requestsyscode, BYTE timeslot, LPINT outlen, LPBYTE lpRes, BYTE tout = 0xB0);//default timeout:300ms
extern "C" API_DLL int WINAPI DEC_Read_NoENC(int nPort, LPBYTE IDm, LPBYTE servicecode, BYTE block, LPINT outlen, LPBYTE lpRes, BYTE tout = 0xB0);//default timeout:300ms
extern "C" API_DLL int WINAPI DEC_Write_NoENC(int nPort, LPBYTE IDm, LPBYTE servicecode, BYTE block, LPBYTE blockdata, LPINT outlen, LPBYTE lpRes, BYTE tout = 0xB0);//default timeout:300ms

//Type B
extern "C" API_DLL int WINAPI DEB_Transparent(int nPort, BYTE datalen, LPBYTE data, BYTE TOUT, LPINT outlen, LPBYTE lpRes);
extern "C" API_DLL int WINAPI DEB_TransparentCRC(int nPort, BYTE datalen, LPBYTE data, LPBYTE crc, BYTE TOUT, LPINT outlen, LPBYTE lpRes);
extern "C" API_DLL int WINAPI DEB_BFRAMING(int nPort, BYTE Fvalue);

//Type A
extern "C" API_DLL int WINAPI DEA_Reset(int nPort, LPBYTE lpdelay);
extern "C" API_DLL int WINAPI DEA_Idle_Req(int nPort, LPINT outlen, LPBYTE lpRes);
extern "C" API_DLL int WINAPI DEA_Wakeup_Req(int nPort, LPINT outlen, LPBYTE lpRes);
extern "C" API_DLL int WINAPI DEA_Anticoll(int nPort, LPINT outlen, LPBYTE lpRes);
extern "C" API_DLL int WINAPI DEA_Select(int nPort, LPBYTE uid, LPINT outlen, LPBYTE lpRes);
extern "C" API_DLL int WINAPI DEA_Auth(int nPort, BYTE mode, BYTE keyno, BYTE blockno);
extern "C" API_DLL int WINAPI DEA_Halt(int nPort);
extern "C" API_DLL int WINAPI DEA_Read(int nPort, BYTE blockno, LPINT outlen, LPBYTE lpRes);
extern "C" API_DLL int WINAPI DEA_Write(int nPort, BYTE blockno, int datalen, LPBYTE data);
extern "C" API_DLL int WINAPI DEA_Increment(int nPort, BYTE blockno, LPBYTE value);
extern "C" API_DLL int WINAPI DEA_Decrement(int nPort, BYTE blockno, LPBYTE value);
extern "C" API_DLL int WINAPI DEA_Inc_Transfer(int nPort, BYTE blockno, LPBYTE value, BYTE trblockno);
extern "C" API_DLL int WINAPI DEA_Dec_Transfer(int nPort, BYTE blockno, LPBYTE value, BYTE trblockno);
extern "C" API_DLL int WINAPI DEA_Restore(int nPort, BYTE blockno);
extern "C" API_DLL int WINAPI DEA_Transfer(int nPort, BYTE blockno);
extern "C" API_DLL int WINAPI DEA_Loadkey(int nPort, BYTE mode, BYTE keyno, LPBYTE keydata);
extern "C" API_DLL int WINAPI DEA_Authkey(int nPort, BYTE mode, LPBYTE keydata, BYTE blockno);
extern "C" API_DLL int WINAPI DEA_Req_Auth(int nPort, BYTE requestmode, BYTE authmode, BYTE keyno, BYTE blockno, LPINT outlen, LPBYTE lpRes);
extern "C" API_DLL int WINAPI DEA_Req_Authkey(int nPort, BYTE requestmode, BYTE authmode, BYTE blockno, LPBYTE keydata, LPINT outlen, LPBYTE lpRes);
extern "C" API_DLL int WINAPI DEA_Inc_Transfer2(int nPort, BYTE blockno, LPBYTE value, BYTE trblockno);
extern "C" API_DLL int WINAPI DEA_Dec_Transfer2(int nPort, BYTE blockno, LPBYTE value, BYTE trblockno);
extern "C" API_DLL int WINAPI DEA_Req_AuthRead(int nPort, BYTE requestmode, BYTE authmode, BYTE keyno, BYTE blockno, LPINT outlen, LPBYTE lpRes);
extern "C" API_DLL int WINAPI DEA_Req_AuthkeyRead(int nPort, BYTE requestmode, BYTE authmode, BYTE blockno, LPBYTE keydata, LPINT outlen, LPBYTE lpRes);
extern "C" API_DLL int WINAPI DEA_Req_AuthWrite(int nPort, BYTE requestmode, BYTE authmode, BYTE keyno, BYTE blockno, LPBYTE data, LPINT outlen, LPBYTE lpRes);
extern "C" API_DLL int WINAPI DEA_Req_AuthkeyWrite(int nPort, BYTE requestmode, BYTE authmode, BYTE blockno, LPBYTE keydata, LPBYTE data, LPINT outlen, LPBYTE lpRes);
extern "C" API_DLL int WINAPI DEA_Req_Select(int nPort, BYTE requestmode, LPINT outlen, LPBYTE lpRes);
extern "C" API_DLL int WINAPI DEA_UltraM_Write(int nPort, BYTE address, LPBYTE data);
extern "C" API_DLL int WINAPI DEA_AntiSelLevel(int nPort, LPINT outlen, LPBYTE lpRes); //0x3D
extern "C" API_DLL int WINAPI DEA_AnticollLevel(int nPort, BYTE cmd, BYTE bitcnt, LPINT outlen, LPBYTE lpRes); //0x3E
extern "C" API_DLL int WINAPI DEA_SelectLevel(int nPort, BYTE cmd, LPBYTE Uid, LPINT outlen, LPBYTE lpRes); //0x3F
extern "C" API_DLL int WINAPI DEA_DeviceInfo(int nPort, LPINT outlen, LPBYTE lpRes);
extern "C" API_DLL int WINAPI DEA_Transparent(int nPort, BYTE datalen, LPBYTE data, BYTE TOUT, LPINT outlen, LPBYTE lpRes);
extern "C" API_DLL int WINAPI DEA_TransparentCRC(int nPort, BYTE datalen, LPBYTE data, LPBYTE crc, BYTE TOUT, LPINT outlen, LPBYTE lpRes);
extern "C" API_DLL int WINAPI DEA_BitMode(int nPort, BYTE datalen, BYTE TxByteNo, BYTE TxBitNo, LPBYTE data, LPINT outlen, LPBYTE lpRes);
extern "C" API_DLL int WINAPI DEA_BitModeAnti(int nPort, BYTE datalen, BYTE TxByteNo, BYTE TxBitNo, LPBYTE data, LPINT outlen, LPBYTE lpRes);
extern "C" API_DLL int WINAPI DEA_BitMode2(int nPort, BYTE datalen, BYTE TxByteNo, BYTE TxBitNo, LPBYTE data, LPINT outlen, LPBYTE lpRes);


//TYPE A/B Common Function
extern "C" API_DLL int WINAPI DE_APDU(int nPort, BYTE datalen, LPBYTE data, LPINT outlen, LPBYTE lpRes);
extern "C" API_DLL int WINAPI DE_FindCard(int nPort, BYTE baud, BYTE cid, BYTE nad, BYTE option, LPINT outlen, LPBYTE lpRes);
extern "C" API_DLL int WINAPI DEAB_RwWrite(int nPort, BYTE Data);
extern "C" API_DLL int WINAPI DEAB_RwRead(int nPort, LPBYTE lpRes);

//Contact card command
extern "C" API_DLL int WINAPI DE_IC_PowerOn(int nPort, BYTE slotno, LPINT outlen, LPBYTE lpRes);
extern "C" API_DLL int WINAPI DE_IC_PPS(int nPort, BYTE slotno, int ppslen, LPBYTE ppsdata, LPINT outlen, LPBYTE lpRes);
extern "C" API_DLL int WINAPI DE_IC_Case1(int nPort, BYTE slotno, int datalen, LPBYTE data, LPINT outlen, LPBYTE lpRes);
extern "C" API_DLL int WINAPI DE_IC_Case2(int nPort, BYTE slotno, int datalen, LPBYTE data, LPINT outlen, LPBYTE lpRes);
extern "C" API_DLL int WINAPI DE_IC_Case3(int nPort, BYTE slotno, int datalen, LPBYTE data, LPINT outlen, LPBYTE lpRes);
extern "C" API_DLL int WINAPI DE_IC_Case4(int nPort, BYTE slotno, int datalen, LPBYTE data, LPINT outlen, LPBYTE lpRes);
extern "C" API_DLL int WINAPI DE_IC_PowerOff(int nPort, BYTE slotno);
extern "C" API_DLL int WINAPI DE_T1Bypass(int nPort, int apdulen, BYTE slotno, BYTE nad, BYTE pcb, BYTE lenth, LPBYTE apdu, BYTE Irc, LPINT outlen, LPBYTE lpRes);
extern "C" API_DLL int WINAPI DE_IC_Speed(int nPort, BYTE slotno, BYTE fidi);
extern "C" API_DLL int WINAPI DE_CARD_APDU(int nPort, BYTE slotno, int datalen, LPBYTE data, LPINT outlen, LPBYTE lpRes);
extern "C" API_DLL int WINAPI DE_CARD_PARITY_ERROR_TEST(int nPort, BYTE Option1, BYTE Option2, LPINT outlen, LPBYTE lpRes);

//ETC
extern "C" API_DLL void WINAPI DE_GetDLLVersion(LPINT outlen, LPBYTE lpRes);
#endif // _DUALCARDDLL_H_