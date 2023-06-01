
#ifndef __TRACEFILEDEBUG_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __TRACEFILEDEBUG_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//======================================================//
//			���� ��ü ����
//======================================================//
#include <fstream>
//=======================================================//
typedef std::basic_ofstream<TCHAR> _tofstream;
//=======================================================//
//======================================================//
//			
//=======================================================//
class CTraceFileDebug : public IOutput
{
	typedef deque<CString, allocator<CString> > CHARDEQUE;
	typedef queue<CString,CHARDEQUE> CHARQUEUE;
	CHARQUEUE qData;
public:
	//������
	CTraceFileDebug(TCHAR* name);
	//�Ҹ���
	~CTraceFileDebug();
//=================������ �Լ�===========================//
	static DWORD WINAPI OutputProcess(LPVOID lpArg);
//=======================================================//
//			���� ����
//======================================================//
public:
	CThread  Outputthread;
	CEvent  OutputEvent;
	CEvent  CloseEvent;
	BOOL bClose;
	//======================================================//
	//			�������̽� ��ü ����
	//======================================================//
	IMyObject* obj;
//======================================================//
//			�Ϲ��Լ�
//======================================================//
	//��� ������ �Լ�
	void OutputThread();
	// ���� ����� ���� ����� �Լ�
	void CreateTraceFile(TCHAR* name, BOOL is_truc=FALSE);
	//������ �����Լ�
	void DebugCloseEvent();
//======================================================//
//			�������̽� �� ���� �Լ�
//======================================================//
public:
	virtual void initInterfaceObject(IMyObject* _obj);
	//��� ȣ�� �Լ�
	virtual void Write(TCHAR* format,...);
	//����
	virtual void Clear();
	//����� ���� ��� ����
	virtual void SetVisible(BOOL visible);
	//����� ���� ��� ���� ����
	virtual BOOL GetVisible();
	//������ ����Լ�
	virtual void OutputString();
	//�޸� ���� 
	virtual void Release();
//=======================================================//
//			��ü ����
//======================================================//
private:
	BOOL _visible;
	//���Ϸ� ���
	_tofstream _filestream;
//=======================================================//

};
#endif// __TRACEFILEDEBUG_74F736E25AB94b5dA789E4730F58F2A1_H__
