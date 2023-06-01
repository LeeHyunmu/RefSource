
#ifndef __DBDATASENDPOOLOBJ_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __DBDATASENDPOOLOBJ_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//======================================================//
//			전역 객체 선언
//======================================================//

//=======================================================//

class CDbDataSendPoolObj : public  IThreadObjectBase
{
public:
	//생성자
	CDbDataSendPoolObj();
	//소멸자
	~CDbDataSendPoolObj();

//======================================================//
//			일반함수 선언
//======================================================//
	BOOL RequestSendData(SOCKETINFO* info, DWORD totalstructlen, DWORD code, HANDLE hEvent=NULL, BOOL bafterdata=FALSE);
//=================스래드 함수===========================//

//=======================================================//
//			객체 포인터 변수
//======================================================//
	IMyObject* obj;
	IOutput* debug;
	IADORecordset* record;
	IServer* iocp;
//=================inner class===========================//
//=======================================================//
//			inner class
//======================================================//

//=======================================================//
//=======================================================//
//		 멤버	변수 선언
//======================================================//
public:
	static LONG s_lNext;
	static LONG s_lCount;
	ULONG	m_n;
	DWORD	thdid;
	HANDLE		hSendEvent;
	SOCKET		sock;
//======================================================//
//			인터페이스 선언 재 정의 함수
//======================================================//
	virtual void Release();
	//초기화
	virtual BOOL initThreadObject(IMyObject* _obj);
	virtual void Run(IThreadPoolThreadCallback &pool);
	virtual void Done();
	//DB 데이타 축출 함수
	virtual HANDLE SetDbDataInfo(IServerMg* servermg, SOCKET _sock, CString _where="");
//=======================================================//

};
#endif// __DBDATASENDPOOLOBJ_74F736E25AB94b5dA789E4730F58F2A1_H__
