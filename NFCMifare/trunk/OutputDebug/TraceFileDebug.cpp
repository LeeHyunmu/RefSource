
#include "stdAfx.h"
#include "TraceFileDebug.h"

#define  DEBUGFILE_MAXLEN 100
#define  DEBUGTHREAD_LOOPTIME 200

IOutput* CTraceFileDebug::debug=NULL;
//--------------------------------------------------------------
//|
//|�̱��� �����
//|
//---------------------------------------------------------------
IOutput* CTraceFileDebug::GetDebug()
{
	return debug;
}
//--------------------------------------------------------------
//|
//|������
//|
//---------------------------------------------------------------
CTraceFileDebug::CTraceFileDebug(TCHAR* name)
{
	debug=this;
	//���� ��� ����
	SetFileName(name);
	// ������ ����
	OutputThread();
}
//--------------------------------------------------------------
//|
//|������� ����
//|
//---------------------------------------------------------------
void CTraceFileDebug::SetFileName(TCHAR* name)
{
	memset(&filename,NULL, sizeof(filename));
	//============================================
	// ��¥ �ð� ���
	//=============================================
	COleDateTime oletime;
	oletime = COleDateTime::GetCurrentTime();
	CString strtime;
	strtime.Format(_T("%04d%02d%02d"),oletime.GetYear(),oletime.GetMonth(), oletime.GetDay());
	_stprintf_s(filename, _T("%s_%s.txt"), name, strtime);
}
//--------------------------------------------------------------
//|
//|�Ҹ���
//|
//---------------------------------------------------------------
CTraceFileDebug::~CTraceFileDebug()
{
}
//--------------------------------------------------------------
//|
//|�޸� ���� 
//|
//---------------------------------------------------------------
void CTraceFileDebug::Release()
{
	if(Outputthread.IsRunning())
	{
		Outputthread.Release();
	}
	while(qData.empty()==false)
		qData.pop();
	if(OutputEvent.IsOpen())
	{
		OutputEvent.SetEvent();
	}
	CloseWinDebugEvent.WaitForEvent(200);
}
//--------------------------------------------------------------
//|
//|������ ����
//|
//---------------------------------------------------------------
void CTraceFileDebug::OutputThread()
{
	////////////////////////////////////////////////////////////////
	//���� ������ ����
	OutputEvent.Create();
	CloseWinDebugEvent.Create();
	Outputthread.Create(OutputProcess, (LPVOID)this);
}
//--------------------------------------------------------------
//|
//|ī�� ���� �� ó�� �Լ�
//|
//---------------------------------------------------------------
CString CTraceFileDebug::crlf = _T("\r\n");
void CTraceFileDebug::OutputString()
{
	if(OutputEvent.WaitForEvent())
	{
		hFile = CreateFile(filename,GENERIC_WRITE,FILE_SHARE_READ,NULL,OPEN_ALWAYS,
			FILE_ATTRIBUTE_NORMAL, NULL);
		if(hFile!=INVALID_HANDLE_VALUE)
		{
			DWORD dwWritten;
			SetFilePointer(hFile, 0,NULL, FILE_END);
			while (!qData.empty())
			{
				CString pdt = qData.front();
				WriteFile(hFile, pdt.GetBuffer(0), pdt.GetLength(),&dwWritten,NULL);
				WriteFile(hFile, crlf.GetBuffer(0), crlf.GetLength(),&dwWritten,NULL);
				qData.pop();
			}
			CloseHandle(hFile);
		}
	}
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
DWORD WINAPI CTraceFileDebug::OutputProcess(LPVOID lpArg)
{
	CTraceFileDebug *debug = (CTraceFileDebug *)lpArg;
	while (debug->Outputthread.IsRunning() && debug->Outputthread.WaitForThread(DEBUGTHREAD_LOOPTIME)==FALSE)
	{
		if(debug)
		{
			debug->OutputString();
		}
	}
	debug->CloseWinDebugEvent.SetEvent();
	return 0;
}
//--------------------------------------------------------------
//|
//|�ؽ�Ʈ ���
//|
//---------------------------------------------------------------
void CTraceFileDebug::Write(TCHAR* format,...)
{
	CStaticDataInitCriticalSectionLock  m_cs;
	m_cs.Lock();
	//============================================
	// �������ڷκ��� ���ڿ� ���
	//=============================================
	va_list args;
	TCHAR buffer[BUFFSIZE_2048]={0,};
	va_start(args, format);
	//int len = vswprintf(NULL, format, args) + 1; 
	//buffer = new TCHAR[len];
	_vstprintf_s(buffer, BUFFSIZE_2048, format, args);
	va_end(args);
	TCHAR fullText[BUFFSIZE_2048];
	_stprintf_s(fullText, _T(">> %s"),buffer);
	//============================================
	// ���
	//=============================================
	DWORD textlen = _tcslen(fullText);
	DWORD nLen = MultiByteToWideChar(CP_ACP, 0, fullText, textlen, NULL, NULL);
	WCHAR _unText[BUFFSIZE_2048]={0,};
	MultiByteToWideChar(CP_ACP, 0, fullText, textlen, _unText, nLen);
	DWORD textpoint = 0;

	while(nLen > textpoint)
	{
		WCHAR WriteData[DEBUGFILE_MAXLEN+1];
		memset(WriteData, 0, sizeof(WriteData));
		int offset = nLen-textpoint;
		if(offset > DEBUGFILE_MAXLEN)
		{
			memcpy(WriteData,_unText+textpoint,DEBUGFILE_MAXLEN*sizeof(WCHAR)); textpoint+=DEBUGFILE_MAXLEN;
			//StringCbCopyW(WriteData, DEBUGWINDOW_MAXLEN,fullText+textpoint);textpoint+=DEBUGWINDOW_MAXLEN;
			qData.push(WriteData);
		}
		else
		{
			memcpy(WriteData,_unText+textpoint,offset*sizeof(WCHAR)); textpoint+=offset;
			//StringCbCopyW(WriteData, offset,fullText+textpoint); textpoint+=offset;
			qData.push(WriteData);
			break;
		}
	}
	m_cs.Unlock();
	if(qData.size())
	{
		OutputEvent.SetEvent();
	}
}
//--------------------------------------------------------------
//|
//|SHOW, HIDE ����
//|
//---------------------------------------------------------------
void CTraceFileDebug::SetVisible(BOOL visible)
{
}
//--------------------------------------------------------------
//|
//|SHOW HIDE ����
//|
//---------------------------------------------------------------
BOOL CTraceFileDebug::GetVisible()
{
	return FALSE;
}
//--------------------------------------------------------------
//|
//| ���� ����
//|
//---------------------------------------------------------------
void CTraceFileDebug::Clear()
{
}