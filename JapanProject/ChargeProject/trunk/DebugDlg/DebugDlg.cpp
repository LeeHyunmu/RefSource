//  : interface of the CAboutDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DebugDlg.h"

#define CONTENTSMALLTHREAD_LOOPTIME 200
#define  DEBUGFILE_MAXLEN 100
CDebugDlg* CDebugDlg::debug=NULL;
//--------------------------------------------------------------
//|
//|싱글톤 디버그
//|
//---------------------------------------------------------------
CDebugDlg* CDebugDlg::GetDebug()
{
	if(debug==NULL)
	{
		debug=new CDebugDlg();
		debug->Create(NULL);
		debug->ShowWindow(SW_SHOW);
	}
	return debug;
}
//--------------------------------------------------------------
//|
//|싱글톤 디버그
//|
//---------------------------------------------------------------
void CDebugDlg::ReleaseDebug()
{
	if(debug!=NULL)
	{
		debug->DestroyWindow();
		delete debug; debug=NULL;
	}
}
CDebugDlg::CDebugDlg()
{

}
LRESULT CDebugDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);

	//애니메이션 스래드
	Outputthread.Create(ThreadProcess, (LPVOID)this);
	OutputEvent.Create();
	
	DebugMessage.Attach(GetDlgItem(IDC_LIST_DEBUGMESSAGE));
	return TRUE;
}
//--------------------------------------------------------------
//|
//|객체 소멸 함수
//|
//---------------------------------------------------------------
LRESULT CDebugDlg::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// unregister message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	if(Outputthread.IsRunning())
	{
		Outputthread.Release();
	}
	DebugMessage.Detach();
	return 0;
}
LRESULT CDebugDlg::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	DestroyWindow();
	return 0;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
BOOL CDebugDlg::PreTranslateMessage(MSG* pMsg)
{
	return CWindow::IsDialogMessage(pMsg);
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
DWORD WINAPI CDebugDlg::ThreadProcess(LPVOID lpArg)
{
	CDebugDlg *Dlg = (CDebugDlg *)lpArg;
	while (Dlg->Outputthread.IsRunning()  && Dlg->Outputthread.WaitForThread(CONTENTSMALLTHREAD_LOOPTIME)==FALSE)
	{
		Dlg->OutputString();
	}
	Dlg->Outputthread.SetCloseEvent();
	return 0;
}
//--------------------------------------------------------------
//|
//|텍스트 출력
//|
//---------------------------------------------------------------
void CDebugDlg::Write(TCHAR* format,...)
{
	CStaticDataInitCriticalSectionLock  m_cs;
	m_cs.Lock();
	//============================================
	// 가변인자로부터 문자열 얻기
	//=============================================
	va_list args;
	TCHAR buffer[BUFFSIZE_2048]={0,};
	va_start(args, format);
	_vstprintf_s(buffer, BUFFSIZE_2048, format, args);
	va_end(args);
	TCHAR fullText[BUFFSIZE_2048];
	_stprintf_s(fullText, BUFFSIZE_2048, _T(">> %s"),buffer);
	//============================================
	// 출력
	//=============================================
	DWORD textlen = _tcslen(fullText);
	DWORD textpoint = 0;

	while(textlen > textpoint)
	{
		TCHAR WriteData[DEBUGFILE_MAXLEN+1];
		memset(WriteData, 0, sizeof(WriteData));
		int offset = textlen-textpoint;
		if(offset > DEBUGFILE_MAXLEN)
		{
			memcpy(WriteData,fullText+textpoint,DEBUGFILE_MAXLEN*sizeof(TCHAR)); textpoint+=DEBUGFILE_MAXLEN;
			qData.push(WriteData);
		}
		else
		{
			memcpy(WriteData,fullText+textpoint,offset*sizeof(TCHAR)); textpoint+=offset;
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
//|카드 감지 및 처리 함수
//|
//---------------------------------------------------------------
void CDebugDlg::OutputString()
{
	if(OutputEvent.WaitForEvent())
	{
		while (!qData.empty())
		{
			DWORD dwresult;
			TCHAR* pdt = qData.front().GetBuffer(0);
			// 리스트 박스에 문자열을 추가한다.
			int pdtlen = DebugMessage.AddString(pdt);
			//if(SUCCEEDED(SendMessageTimeout( _listbox, LB_ADDSTRING, 0, (LPARAM) pdt,SMTO_ABORTIFHUNG,500,&dwresult)))
			//{
			// 리스트 박스의 컨텐츠 개수만큼 커서를 아래로 이동시킨다.
			dwresult = DebugMessage.GetCount();
			DebugMessage.SetCurSel(dwresult);
			//추가 후 하나 제거
			qData.pop();
		}
	}
}