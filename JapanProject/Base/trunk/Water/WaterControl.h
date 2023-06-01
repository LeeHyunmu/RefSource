
#ifndef __WATERCONTROL_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __WATERCONTROL_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//Reader Event Func
typedef DWORD (WINAPI *READEREVENTFUNC)(LPVOID lpThreadParameter, byte* eventdata, int eventdatalen);
typedef READEREVENTFUNC READEREVENTFUNC_ROUTINE;
//P2P Event Func
typedef DWORD (WINAPI *P2PEVENTFUNC)(LPVOID lpThreadParameter, byte* eventdata, int eventdatalen);
typedef P2PEVENTFUNC P2PEVENTFUNC_ROUTINE;
//Card Event Func
typedef DWORD (WINAPI *CARDEVENTFUNC)(LPVOID lpThreadParameter, byte* eventdata, int eventdatalen);
typedef CARDEVENTFUNC CARDEVENTFUNC_ROUTINE;
class CWaterControl
{
	BOOL bWaterOpen;
	CWaterFormat::AQCmdKind RespFuncFlag;
	//Reader Func
	READEREVENTFUNC_ROUTINE readfunc;
	//P2P Func
	P2PEVENTFUNC_ROUTINE p2pfunc;
	//Reader Func
	CARDEVENTFUNC_ROUTINE cardfunc;
	//이벤트를 받을 클래스 객체
	LPVOID lpEventTaget;

	CSerial nfcserial;
	// 받은 데이터 길이
	byte NfcReceiveData[MAX_COMMANDDATA];
	//받은 데이터 길이
	int NfcReceivelen;
	// 받은 데이터 길이
	byte RespData[MAX_COMMANDDATA];
	//받은 데이터 길이
	int Resplen;
	//Resp 이벤트를 받기 위한 스래드
	//NFC Event thread에서 이벤트 사용(응답 이벤트 Open, 등등 함수 타입으로 만든 객체에서 대기 상황에서 호출)
	CEvent RespEvent;
	//NFC Event thread Command에 대한 모든 이벤트를 받는다
	CThread NfcThread;
	//AQ 디바이스에 Target 이 진입했을 때 이벤트 처리를 위한 스래드
	//NfcThread 스래드를 세우면 안되므로 독립적 스래드로 처리하기 위한 객체
	CThread CmdEventThread;
	//CmdEventThread 객체에서 사용하는 이벤트 객체
	CEvent EventRespEvent;

	//메시지 이벤트 윈도우 핸들
	//HWND hWnd;
	
	//시리얼 컴포트 저장
	CString ActiveComPort;
	//시리얼 포트 찾기
	//
	static size_t GetFindRegKeyEnumValue(HKEY hRKey, TCHAR *pRegName, vector<CString>& valueString);
	//Crc c 코드
	unsigned short UpdateCrc(unsigned char ch, unsigned short* lpwCrc);
	void ComputeCrc(byte *Data, int Length, byte& TransmitFirst, byte& TransmitSecond);
	//Command Data
	UINT makeCommandData(byte stx, byte cmd, byte* data, int datalen, byte* CommandData, int& CommandDatalen);
	//실제 파서할 데이터 추출 함수
	int GetNfcLoddingData(byte* data, int& datalen);
	//이벤트 루프
	BOOL NfcLoop();
	//이벤트 루프
	BOOL NfcEventLoop();
	//Serial Polling Loop
	CWaterFormat::AQErrorCode CWaterControl::NfcResponseLoop(CWaterFormat::AQCmdKind& func, byte* data, int readlen);


	//thread 함수
	static DWORD WINAPI ThreadProcess(LPVOID lpArg);
	//Event thread 함수
	static  DWORD WINAPI EventThreadProcess(LPVOID lpArg);

	void Release();
public:
	void initWater(LPVOID lpThreadParameter);
	//생성자
	CWaterControl(void);
	//소멸자
	~CWaterControl(void);
};
#endif// __WATERCONTROL_74F736E25AB94b5dA789E4730F58F2A1_H__
