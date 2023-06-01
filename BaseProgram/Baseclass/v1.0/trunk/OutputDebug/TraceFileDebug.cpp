
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
	bClose=FALSE;
	//���� ��� ����
	CreateFile(name);
	// ������ ����
	OutputThread();
	bDebugRunning=TRUE;
}
//--------------------------------------------------------------
//|
//|������� ����
//|
//---------------------------------------------------------------
void CTraceFileDebug::CreateFile(TCHAR* name, BOOL is_truc)
{
	//�ؽ�Ʈ���Ϸ� ����� ���� �̸��ڿ� ".txt"�� ���δ�.
	static TCHAR filename[MAX_PATH];
	if(name) _stprintf_s(filename, _countof(filename), _T("%s.txt"), name);

	//�ѱ� ����� �����ϰ� �Ѵ�.
	_tsetlocale(LC_ALL, _T("Korean"));
	_filestream.imbue(std::locale("kor"));

	//�������(���� ���������)�� �ؽ�Ʈ ������ ����.
	if(is_truc) _filestream.open(filename, std::ios_base::trunc);
	// �߰� ���(�������Ͽ� ����)�� ������ ����.
	else _filestream.open(filename, std::ios_base::app);
	if(!_filestream.is_open())
		MessageBox(NULL, _T("traceMessage Class can not open file."), _T("Alert"), MB_OK);
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
	bDebugRunning=FALSE;
	if(OutputEvent.IsOpen())
	{
		bClose=TRUE;
		CloseFileDebugEvent.Create();
		OutputEvent.SetEvent();
		CloseFileDebugEvent.WaitForEvent();
	}
	if(_filestream.is_open()) _filestream.close();
	Outputthread.WaitForThread();
	while(qData.empty()==false)
		qData.pop();
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
	Outputthread.Create(OutputProcess, (LPVOID)this);
}
//--------------------------------------------------------------
//|
//|ī�� ���� �� ó�� �Լ�
//|
//---------------------------------------------------------------
void CTraceFileDebug::OutputString()
{
	if(OutputEvent.WaitForEvent())
	{
		if(_filestream.is_open())
		{
			while (!qData.empty())
			{
				TCHAR* pdt = qData.front().GetBuffer(0);
				_filestream << pdt << std::endl;
				qData.pop();
			}
		}
	}
}
// 
BOOL CTraceFileDebug::GetbRunning()
{
	return bDebugRunning;
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
		if(debug->bClose)
		{
			break;
		}
		if(debug)
		{
			debug->OutputString();
		}
	}
	debug->CloseFileDebugEvent.SetEvent();
	return 0;
}
//--------------------------------------------------------------
//|
//|�ؽ�Ʈ ���
//|
//---------------------------------------------------------------
void CTraceFileDebug::Write(TCHAR* format,...)
{
	if(GetVisible())
	{
		CStaticDataInitCriticalSectionLock  m_cs;
		m_cs.Lock();
		//============================================
		// �������ڷκ��� ���ڿ� ���
		//=============================================
		va_list args;
		TCHAR buffer[256]={0,};
		va_start(args, format);
		//int len = vswprintf(NULL, format, args) + 1; 
		//buffer = new TCHAR[len];
		_vstprintf_s(buffer, 255, format, args);
		va_end(args);
		TCHAR fullText[1024];
		//============================================
		// ��¥ �ð� ���
		//=============================================
		COleDateTime oletime;
		oletime = COleDateTime::GetCurrentTime();
		CString strtime;
		strtime.Format(_T("%04d%02d%02d_%02d:%02d:%02d"),oletime.GetYear(),oletime.GetMonth(), oletime.GetDay()
			,oletime.GetHour(), oletime.GetMinute(), oletime.GetSecond());
		_stprintf_s(fullText, _T("%s> %s"), strtime, buffer);
		//============================================
		// ���
		//=============================================
		DWORD textlen = _tcslen(fullText);
		int nLen = MultiByteToWideChar(CP_ACP, 0, fullText, textlen, NULL, NULL);
		WCHAR _unText[MAX_PATH]={0,};
		MultiByteToWideChar(CP_ACP, 0, fullText, textlen, _unText, nLen);
		DWORD textpoint = 0;
		if(_filestream.is_open())
		{
			while(nLen != textpoint)
			{
				WCHAR WriteData[DEBUGFILE_MAXLEN+1];
				memset(WriteData, 0, sizeof(WriteData));
				int offset = nLen-textpoint;
				if(offset > DEBUGFILE_MAXLEN)
				{
					memcpy(WriteData,_unText+textpoint,DEBUGFILE_MAXLEN*sizeof(WCHAR)); textpoint+=DEBUGFILE_MAXLEN;
					//StringCbCopyW(WriteData, DEBUGWINDOW_MAXLEN,fullText+textpoint);textpoint+=DEBUGWINDOW_MAXLEN;
				}
				else
				{
					memcpy(WriteData,_unText+textpoint,offset*sizeof(WCHAR)); textpoint+=offset;
					//StringCbCopyW(WriteData, offset,fullText+textpoint); textpoint+=offset;
				}
				//����
				qData.push(WriteData);
			}
		}
		m_cs.Unlock();
		if(qData.size())
			OutputEvent.SetEvent();
	}
}
//--------------------------------------------------------------
//|
//| ���� ����
//|
//---------------------------------------------------------------
void CTraceFileDebug::Clear()
{
	if(_filestream.is_open())
	{
		//���ϳ��� ����
		//���� ���� ������ ����� �ٽ� ������
		_filestream.close();
		CreateFile(NULL,TRUE);
	}
}
//--------------------------------------------------------------
//|
//|SHOW, HIDE ����
//|
//---------------------------------------------------------------
void CTraceFileDebug::SetVisible(BOOL visible)
{
	_visible = visible;
}
//--------------------------------------------------------------
//|
//|SHOW HIDE ����
//|
//---------------------------------------------------------------
BOOL CTraceFileDebug::GetVisible()
{
	return _visible;
}