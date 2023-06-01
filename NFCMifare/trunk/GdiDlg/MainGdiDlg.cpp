#include "stdAfx.h"
#include "MainGdiDlg.h"
#include "../MifareAccessCondition/MifareAccessCondition.h"
#include "../OutputDebug/OutputDebug.h"
#include <stdlib.h>
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------

//--------------------------------------------------------------
//|
//|생성자
//|
//---------------------------------------------------------------
CMainGdiDlg::CMainGdiDlg():
hWnd(NULL)
{}
//--------------------------------------------------------------
//|
//|소멸자
//|
//---------------------------------------------------------------
CMainGdiDlg::~CMainGdiDlg()
{
}
//--------------------------------------------------------------
//|
//|사용중인 객체를 메모리에서 제거한다
//|
//---------------------------------------------------------------
void CMainGdiDlg::ControlDestroy()
{
}
//--------------------------------------------------------------
//|
//|dlg 초기화 
//|
//---------------------------------------------------------------

void CMainGdiDlg::initDlg(HWND _hWnd)
{
	//========================================
	// HWND 저장
	//========================================
	hWnd = _hWnd;
	//========================================
	// Rgn으로 원을 그린다
	//========================================
	CRect parentRect;
	GetWindowRect(_hWnd, &parentRect);

	byte key[4];
	key[0]=0;
	key[1]=0;
	key[2]=136;
	key[3]=0;
	CMifareKeyMake mifare;
	byte sector = mifare.GetAccessCondition(&key[0], CMifareKeyMake::Mifare_datablock0);
	char buf[10];
	itoa(sector,&buf[0],10);
	DebugEditCall(buf);
	key[0]=0;
	key[1]=0;
	key[2]=0;
	key[3]=0;
	mifare.SetAccessCondition(&key[0], CMifareKeyMake::Mifare_datablock0, 7);
	itoa(key[1],&buf[0],10);
	DebugEditCall(buf);
}
CString   CMainGdiDlg::ChangeExt(CString sPathName, CString sNewExt, char find)
{
	int            nPos;
	CString         sResult;
	nPos= sPathName.ReverseFind(find);
	if(nPos==-1)                        // .을 찾을 수 없다..
		sResult = sPathName +find+sNewExt;
	else
		sResult = sPathName.Left(nPos)+find+sNewExt;
	return sResult;
}
//--------------------------------------------------------------
//|
//|Dlg 그리기 함수
//|
//---------------------------------------------------------------
void CMainGdiDlg::DrawPaint()
{
	CPaintDC dc(hWnd);
}
//--------------------------------------------------------------
//|
//|강제로 다시 그리기
//|
//---------------------------------------------------------------
void CMainGdiDlg::MainGdiinvalidateRect(CRect* Rc, BOOL bErase)
{
	InvalidateRect(hWnd, Rc, bErase);
	//InvalidateRect를 강제로 바로 실행 하기 위해존제..
	UpdateWindow(hWnd);
}
//--------------------------------------------------------------
//|
//| //
//|
//---------------------------------------------------------------
void CMainGdiDlg::DebugEditCall(CString str)
{
#if DEBUGVIEWER
	UINT len = str.GetLength();
	byte* data = new byte[len+5];
	ZeroMemory(data, len+5);
	memcpy(&data[0], &len, 4);
	memcpy(&data[4], str.GetBuffer(), len);
	PostMessage(hWnd, WM_COMMANDDEBUGSTRING,0,(LPARAM)data);
#endif//DEBUGVIEWER
}