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
//|������
//|
//---------------------------------------------------------------
CMainGdiDlg::CMainGdiDlg():
hWnd(NULL)
{}
//--------------------------------------------------------------
//|
//|�Ҹ���
//|
//---------------------------------------------------------------
CMainGdiDlg::~CMainGdiDlg()
{
}
//--------------------------------------------------------------
//|
//|������� ��ü�� �޸𸮿��� �����Ѵ�
//|
//---------------------------------------------------------------
void CMainGdiDlg::ControlDestroy()
{
}
//--------------------------------------------------------------
//|
//|dlg �ʱ�ȭ 
//|
//---------------------------------------------------------------

void CMainGdiDlg::initDlg(HWND _hWnd)
{
	//========================================
	// HWND ����
	//========================================
	hWnd = _hWnd;
	//========================================
	// Rgn���� ���� �׸���
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
	if(nPos==-1)                        // .�� ã�� �� ����..
		sResult = sPathName +find+sNewExt;
	else
		sResult = sPathName.Left(nPos)+find+sNewExt;
	return sResult;
}
//--------------------------------------------------------------
//|
//|Dlg �׸��� �Լ�
//|
//---------------------------------------------------------------
void CMainGdiDlg::DrawPaint()
{
	CPaintDC dc(hWnd);
}
//--------------------------------------------------------------
//|
//|������ �ٽ� �׸���
//|
//---------------------------------------------------------------
void CMainGdiDlg::MainGdiinvalidateRect(CRect* Rc, BOOL bErase)
{
	InvalidateRect(hWnd, Rc, bErase);
	//InvalidateRect�� ������ �ٷ� ���� �ϱ� ��������..
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