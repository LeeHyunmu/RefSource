
#include "stdAfx.h"
#include "TraceFileDebug.h"

#define  DEBUGFILE_MAXLEN 100
#define  DEBUGTHREAD_LOOPTIME 200

IOutput* CTraceFileDebug::debug=NULL;
//--------------------------------------------------------------
//|
//|싱글톤 디버그
//|
//---------------------------------------------------------------
IOutput* CTraceFileDebug::GetDebug()
{
	return debug;
}
//--------------------------------------------------------------
//|
//|생성자
//|
//---------------------------------------------------------------
CTraceFileDebug::CTraceFileDebug(TCHAR* name)
{
	debug=this;
	bClose=FALSE;
	//파일 출력 생성
	CreateFile(name);
	// 스래드 생성
	OutputThread();
	bDebugRunning=TRUE;
}
//--------------------------------------------------------------
//|
//|파일출력 생성
//|
//---------------------------------------------------------------
void CTraceFileDebug::CreateFile(TCHAR* name, BOOL is_truc)
{
	//텍스트파일로 만들기 위해 이름뒤에 ".txt"를 붙인다.
	static TCHAR filename[MAX_PATH];
	if(name) _stprintf_s(filename, _countof(filename), _T("%s.txt"), name);

	//한글 출력이 가능하게 한다.
	_tsetlocale(LC_ALL, _T("Korean"));
	_filestream.imbue(std::locale("kor"));

	//삭제모드(기존 파일지우기)로 텍스트 파일을 연다.
	if(is_truc) _filestream.open(filename, std::ios_base::trunc);
	// 추가 모드(기존파일에 연결)로 파일을 연다.
	else _filestream.open(filename, std::ios_base::app);
	if(!_filestream.is_open())
		MessageBox(NULL, _T("traceMessage Class can not open file."), _T("Alert"), MB_OK);
}
//--------------------------------------------------------------
//|
//|소멸자
//|
//---------------------------------------------------------------
CTraceFileDebug::~CTraceFileDebug()
{
}
//--------------------------------------------------------------
//|
//|메모리 해제 
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
//|스래드 생성
//|
//---------------------------------------------------------------
void CTraceFileDebug::OutputThread()
{
	////////////////////////////////////////////////////////////////
	//종료 스레드 생성
	OutputEvent.Create();
	Outputthread.Create(OutputProcess, (LPVOID)this);
}
//--------------------------------------------------------------
//|
//|카드 감지 및 처리 함수
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
//|텍스트 출력
//|
//---------------------------------------------------------------
void CTraceFileDebug::Write(TCHAR* format,...)
{
	if(GetVisible())
	{
		CStaticDataInitCriticalSectionLock  m_cs;
		m_cs.Lock();
		//============================================
		// 가변인자로부터 문자열 얻기
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
		// 날짜 시간 얻기
		//=============================================
		COleDateTime oletime;
		oletime = COleDateTime::GetCurrentTime();
		CString strtime;
		strtime.Format(_T("%04d%02d%02d_%02d:%02d:%02d"),oletime.GetYear(),oletime.GetMonth(), oletime.GetDay()
			,oletime.GetHour(), oletime.GetMinute(), oletime.GetSecond());
		_stprintf_s(fullText, _T("%s> %s"), strtime, buffer);
		//============================================
		// 출력
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
				//저장
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
//| 내용 삭제
//|
//---------------------------------------------------------------
void CTraceFileDebug::Clear()
{
	if(_filestream.is_open())
	{
		//파일내용 삭제
		//쉽게 기존 파일을 지우고 다시 생성함
		_filestream.close();
		CreateFile(NULL,TRUE);
	}
}
//--------------------------------------------------------------
//|
//|SHOW, HIDE 설정
//|
//---------------------------------------------------------------
void CTraceFileDebug::SetVisible(BOOL visible)
{
	_visible = visible;
}
//--------------------------------------------------------------
//|
//|SHOW HIDE 상태
//|
//---------------------------------------------------------------
BOOL CTraceFileDebug::GetVisible()
{
	return _visible;
}