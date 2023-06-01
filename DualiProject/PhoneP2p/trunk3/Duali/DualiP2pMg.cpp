
#include "stdAfx.h"
#include "DualiP2pMg.h"

#define LOOPFLAGE_ACTIVE	0x00000001 //루프 안쪽 실행
#define LOOPFLAGE_END		0x00000002 //루프 안쪽 실행 안함
#define LOOPFLAGE_CLOSE		0x00000004 //루프 안쪽 실행 안함

#define SENDFLAGE_DATA		0x00000001 //데이타를 보낼 때
#define SENDFLAGE_FILE		0x00000002 //파일을 보낼 때
#define SENDFLAGE_NONE		0x00000004 //아무것도 보내지 않을 때
//--------------------------------------------------------------
//|
//|생성자
//|
//---------------------------------------------------------------
CDualiP2pMg::CDualiP2pMg():
debug(NULL),
bOk(FALSE),
dwSendflage(SENDFLAGE_NONE)
{}
//--------------------------------------------------------------
//|
//|소멸자
//|
//---------------------------------------------------------------
CDualiP2pMg::~CDualiP2pMg()
{
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
void CDualiP2pMg::P2pObject()
{
	////////////////////////////////////////////////////////////////
	//이벤트 객체 생성
	//완료 상태 메시지
	hReadEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	//Send, Recv 진행 메시지
	hWriteEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	// 스레드 생성
	p2pthread.Create(P2pProcess, (LPVOID)this);
	dwLoop=LOOPFLAGE_ACTIVE;
}
//--------------------------------------------------------------
//|
//|카드 감지 및 처리 함수
//|
//---------------------------------------------------------------
void CDualiP2pMg::P2pFunc()
{
	Sleep(10);
	while(TRUE)
	{
		WaitForSingleObject(hWriteEvent, INFINITE);
		if(dwLoop&LOOPFLAGE_ACTIVE)
		{
			if(dwSendflage&SENDFLAGE_DATA)
			{
				bOk = dualicard->OneDataSend(SendData, RecvData);
			}
			else if(dwSendflage&SENDFLAGE_FILE)
			{
				bOk = dualicard->FileDataSend(SendData);
			}
		}
		else if(dwLoop&LOOPFLAGE_CLOSE)
		{
			break;
		}
		SetEvent(hReadEvent);
		debug->Write(_T("다음 준비상태"));
	}
	dwLoop=LOOPFLAGE_END;
	SetEvent(hReadEvent);
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
DWORD WINAPI CDualiP2pMg::P2pProcess(LPVOID lpArg)
{
	IDualiP2pMg *dualip2p = (IDualiP2pMg *)lpArg;
	dualip2p->P2pFunc();
	return 0;
}

//--------------------------------------------------------------
//|
//|사용중인 객체를 메모리에서 제거한다
//|
//---------------------------------------------------------------
void CDualiP2pMg::Release()
{
	//|=============================================
	//|스래드 종료 이벤트
	//|==================================================
	if(hWriteEvent)
	{
		dwLoop=LOOPFLAGE_CLOSE;
		SetEvent(hWriteEvent);
	}
	WaitForSingleObject(hReadEvent, INFINITE);
	CloseHandle(hReadEvent);
	CloseHandle(hWriteEvent);
}
//--------------------------------------------------------------
//|
//|선언된 객체를 생성한다
//|
//---------------------------------------------------------------
void CDualiP2pMg::initInterfaceObject(IMyObject* _obj)
{
	//========================================
	// 오브젝트 관리 생성 관리 클래스
	//========================================
	obj=_obj;
	//========================================
	// 디버그 출력 클래스
	//========================================
	debug=obj->GetOutputObject();
	//========================================
	// 듀얼 아이 라이브러리 클래스
	//========================================
	dualicard = obj->GetDualiCardMgObject();
	//버퍼 초기화
	//송신 데이타
	SendData="";
	//수신 데이타
	RecvData="";
	P2pObject();
}
//--------------------------------------------------------------
//|
//|Send (보낼 데이터가 있을 때
//|
//---------------------------------------------------------------
BOOL CDualiP2pMg::SendDatafn(DWORD _dwSendflage, CString data, DWORD dwTimeout)
{
	bOk=FALSE;
	dwSendflage=_dwSendflage;
	SendData="";
	SendData=data;
	SetEvent(hWriteEvent);
	if(WaitForSingleObject(hReadEvent, dwTimeout)== WAIT_OBJECT_0)
	{
		data=RecvData;
	}
	dwSendflage=SENDFLAGE_NONE;
	SendData="";
	return bOk;
}
