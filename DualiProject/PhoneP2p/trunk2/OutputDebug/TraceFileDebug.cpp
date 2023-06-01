
#include "stdAfx.h"
#include "TraceFileDebug.h"
#include <tchar.h>

#define  DEBUGFILE_MAXLEN 100
//--------------------------------------------------------------
//|
//|������
//|
//---------------------------------------------------------------
CTraceFileDebug::CTraceFileDebug(TCHAR* name)
{
	bClose=FALSE;
	//���� ��� ����
	CreateFile(name);
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
// 	_tsetlocale(LC_ALL, _T("Korean"));
// 	_filestream.imbue(std::locale("kor"));

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
	if(OutputEvent.IsOpen())
	{
		bClose=TRUE;
		OutputEvent.SetEvent();
		CloseEvent.Create();
	}
	CloseEvent.WaitForEvent();
	if(_filestream.is_open()) _filestream.close();
	Outputthread.WaitForThread();
	while(qData.empty()==false)
		qData.pop();
}
void CTraceFileDebug::DebugCloseEvent()
{
	if(OutputEvent.IsOpen())
	{
		bClose=TRUE;
		OutputEvent.SetEvent();
		Sleep(3000);
	}
}
//--------------------------------------------------------------
//|
//|����� ��ü�� �����Ѵ�
//|
//---------------------------------------------------------------
void CTraceFileDebug::initInterfaceObject(IMyObject* _obj)
{
	//========================================
	// ������Ʈ ���� ���� ���� Ŭ����
	//========================================
	obj=_obj;
	//========================================
	// ������ ����
	//========================================
	OutputThread();
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
	Outputthread.Create(OutputProcess, (LPVOID)obj);
}
//--------------------------------------------------------------
//|
//|ī�� ���� �� ó�� �Լ�
//|
//---------------------------------------------------------------
void CTraceFileDebug::OutputString()
{
	while(OutputEvent.WaitForEvent())
	{
		if(bClose)
		{
			break;
		}
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
	CloseEvent.SetEvent();
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
DWORD WINAPI CTraceFileDebug::OutputProcess(LPVOID lpArg)
{
	IOutput *debug = ((IMyObject *)lpArg)->GetOutputObject();
	if(debug)
		debug->OutputString();
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
		_vstprintf_s(buffer, sizeof(buffer)/sizeof(buffer[0]), format, args);
		va_end(args);
		WCHAR fullText[1024];
		//============================================
		// ��¥ �ð� ���
		//=============================================
		COleDateTime oletime;
		oletime = COleDateTime::GetCurrentTime();
		CString strtime;
		strtime.Format(_T("%04d%02d%02d_%02d:%02d:%02d"),oletime.GetYear(),oletime.GetMonth(), oletime.GetDay()
			,oletime.GetHour(), oletime.GetMinute(), oletime.GetSecond());
		_stprintf_s(fullText, sizeof(fullText)/sizeof(fullText[0]), _T("%s> %s"), strtime, buffer);
		//============================================
		// ���
		//=============================================
		DWORD nLen = _tcslen(fullText);
		//	int nLen = MultiByteToWideChar(CP_ACP, 0, fullText, textlen, NULL, NULL);
		//	WCHAR _unText[MAX_PATH]={0,};
		//	MultiByteToWideChar(CP_ACP, 0,fullText, textlen,_unText, nLen);
		DWORD textpoint = 0;
		if(_filestream.is_open())
		{
			while(nLen != textpoint)
			{
				WCHAR WriteData[DEBUGFILE_MAXLEN+1];
				memset(WriteData, 0, sizeof(WriteData)/sizeof(WriteData[0]));
				int offset = nLen-textpoint;
				if(offset > DEBUGFILE_MAXLEN)
				{
					memcpy(WriteData,fullText+textpoint,DEBUGFILE_MAXLEN*sizeof(WCHAR)); textpoint+=DEBUGFILE_MAXLEN;
					//StringCbCopyW(WriteData, DEBUGWINDOW_MAXLEN,fullText+textpoint);textpoint+=DEBUGWINDOW_MAXLEN;
				}
				else
				{
					memcpy(WriteData,fullText+textpoint,offset*sizeof(WCHAR)); textpoint+=offset;
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