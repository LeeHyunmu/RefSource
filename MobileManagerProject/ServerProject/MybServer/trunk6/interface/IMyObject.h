
#ifndef __IMYOBJECT_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __IMYOBJECT_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//--------------------------------------------------------------
//|
//|IServer 인터페이스
//|
//---------------------------------------------------------------
interface IMyObject
{
	//객체를 생성한다
	virtual BOOL CreateObject(LPVOID _pross)=0;
	//오브젝트 객체를 메모리 해제한다
	virtual void KillObject()=0;
	//메모리 해제 호출
	virtual void Release()=0;
	//디버그 출력 모드
	virtual IOutput* GetOutput()=0;
	//Crc32 
	virtual ICrc* GetCrc32()=0;
	//IOCP 서버
	virtual IServer* GetIocpServer()=0;
	//서버 메니저
	virtual IServerMg* GetServerMg()=0;

	virtual IMysqlDb* GetMysqlDb()=0;
	virtual IMysqlDbtable* GetMysqlDbtable()=0;
	//ADO 객체
	virtual IADODatabase* GetAdoDatabase()=0;
	//Command
	virtual IADOCommand* GetAdoCommand()=0;
	virtual void ReleaseObject(IADOCommand* _obj)=0;
	//Parameter
	virtual IADOParameter* GetAdoParameter()=0;
	virtual void ReleaseObject(IADOParameter* _obj)=0;
	//Recordset
	virtual IADORecordset* GetAdoRecordset()=0;
	virtual void ReleaseObject(IADORecordset* _obj)=0;
	//db 사용자 클래스
	virtual IMobileDbMg* GetMobileDbMg()=0;
	//스래드 풀
	virtual IThreadPool* GetThreadPool()=0;
};

#endif //__IMYOBJECT_74F736E25AB94b5dA789E4730F58F2A1_H__