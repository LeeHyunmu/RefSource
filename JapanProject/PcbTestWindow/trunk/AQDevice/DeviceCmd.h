
#ifndef __DEVICECMD_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __DEVICECMD_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//Reader Event Func
typedef DWORD (WINAPI *WATERCHECKFUNC)(LPVOID lpThreadParameter, CAQCmdFormat::WaterCheck check);
typedef WATERCHECKFUNC WATERCHECKFUNC_ROUTINE;
typedef DWORD (WINAPI *WATERDEBUGFUNC)(LPVOID lpThreadParameter, byte* data, int datalen, bool bsend);
typedef WATERDEBUGFUNC WATERDEBUGFUNC_ROUTINE;
class CDeviceCmd
{
	//싱글턴 객체
	static CDeviceCmd* AQdeviceSingle;

	//watercheck Func
	WATERCHECKFUNC_ROUTINE watercheckfunc;
	//debug func
	WATERDEBUGFUNC_ROUTINE waterdebug;
	//이벤트를 받을 클래스 객체
	LPVOID lpEventTaget;

	//시리얼 통신 클래스
	CSerial nfcserial;
	//Resp Func Code
	CAQCmdFormat::WaterCheck RespFuncFlag;
	//Resp Error Code
	CAQCmdFormat::WaterErrorCode RespErrorCode;
	// 받은 데이터 길이
	byte NfcReceiveData[MAX_COMMANDDATA];
	//받은 데이터 길이
	int NfcReceivelen;
	// 받은 데이터 길이
	byte RespData[MAX_COMMANDDATA];
	//받은 데이터 길이
	int Resplen;
	// 받은 데이터 길이
	byte RespErrorData[MAX_COMMANDDATA];
	int RespErrorlen;
	//Resp 이벤트를 받기 위한 스래드
	//NFC Event thread에서 이벤트 사용(응답 이벤트 Open, 등등 함수 타입으로 만든 객체에서 대기 상황에서 호출)
	CLeeEvent RespEvent;
	//NFC Event thread Command에 대한 모든 이벤트를 받는다
	CThread NfcThread;
	//AQ 디바이스에 Target 이 진입했을 때 이벤트 처리를 위한 스래드
	//NfcThread 스래드를 세우면 안되므로 독립적 스래드로 처리하기 위한 객체
	CThread CmdEventThread;
	//CmdEventThread 객체에서 사용하는 이벤트 객체
	CLeeEvent EventRespEvent;

	//메시지 이벤트 윈도우 핸들
	//HWND hWnd;
	
	//시리얼 컴포트 저장
	CString ActiveComPort;

	//시리얼 포트 찾기
	//
	static size_t GetFindRegKeyEnumValue(HKEY hRKey, TCHAR *pRegName, vector<CString>& valueString);

	void ComputeCrc(byte *Data, int Length, byte& chBlock);
	//Command Data
	UINT makeCommandData(CAQCmdFormat::WateAddress cmd, byte* data, int datalen, byte* CommandData, int& CommandDatalen);
	//실제 파서할 데이터 추출 함수
	int GetNfcLoddingData(byte* data, int& datalen);
	//이벤트 루프
	BOOL NfcLoop();
	//이벤트 루프
	BOOL NfcEventLoop();
	//Serial Polling Loop
	CAQCmdFormat::WaterErrorCode NfcResponseLoop(CAQCmdFormat::WaterCheck& func, byte* data, int readlen);

	//클래스 객체를 초기 상태로 복귀
	void Release(void);

	//thread 함수
	static DWORD WINAPI ThreadProcess(LPVOID lpArg);
	//Event thread 함수
	static  DWORD WINAPI EventThreadProcess(LPVOID lpArg);
public:
	//AQ device 클래스 인스턴스 생성
	static CDeviceCmd* getInstance();
	//AQ device 클래스 인스턴스 객체 소멸
	static void ReleaseInstance();
	//생성자
	CDeviceCmd(void);
	//소멸자
	~CDeviceCmd(void);
	//NFC 기능 생성 메니저
	CAQCmdFormat::WaterErrorCode initAQNFC(LPVOID lpThreadParameter, WATERCHECKFUNC_ROUTINE _watercheckfunc, WATERDEBUGFUNC_ROUTINE _debugfunc=NULL);

	//디바이스 다시 초기화(시리얼 포트까지 초기화)
	BOOL ModeReset_init();

	//integer 리틀엔디안 빅엔디안 변경 byte type으로
	void integerEndianChangebyte(int _value, byte* _data);
	//byte type 리틀엔디안 빅엔디안 변경 integer type으로
	void byteEndianChangeinteger(byte* _data, int& _value);
	//short 리틀엔디안 빅엔디안 변경 byte type으로
	void shortEndianChangebyte(int _value, byte* _data);
	//byte type 리틀엔디안 빅엔디안 변경 short type으로
	void byteEndianChangeshort(byte* _data, int& _value);
	//명렁어 보내기
	void CommandSend(byte data);
};
#endif// __DEVICECMD_74F736E25AB94b5dA789E4730F58F2A1_H__
