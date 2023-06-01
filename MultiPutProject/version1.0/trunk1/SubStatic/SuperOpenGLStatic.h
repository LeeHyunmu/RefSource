
#ifndef __SUPEROPENGLSTATIC_B7A15A9D01DC403996FC45F7F92F52D3_H__
#define __SUPEROPENGLSTATIC_B7A15A9D01DC403996FC45F7F92F52D3_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//버튼 이미지 개수


class CSuperOpenGLStatic : public CWindowImpl<CSuperOpenGLStatic, CStatic>, COpenGL<CSuperOpenGLStatic>, public CMessageFilter, public CIdleHandler
{
	//로테이션 각 설정(Roll)
	GLfloat m_fRoll;
	//로테이션 각 설정(Pitch)
	GLfloat m_fPithc;
	//로테이션 각 설정(Yaw)
	GLfloat m_fYaw;
public:
	DECLARE_WND_SUPERCLASS(_T("SuperTextStatic"), _T("STATIC"))
	BEGIN_MSG_MAP_EX(CSuperTextStatic)
		MSG_WM_CREATE(OnCreate)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		CHAIN_MSG_MAP(COpenGL<CSuperOpenGLStatic>)
		DEFAULT_REFLECTION_HANDLER()
		END_MSG_MAP()
	//생성자
	CSuperOpenGLStatic(void);
	//소멸자
	~CSuperOpenGLStatic(void);
	//
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//메모리에서 제거
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	//서브클래싱 함수
	BOOL SubclassWindow(HWND hWnd);
	HWND UnsubclassWindow(BOOL bForce = FALSE);
	//버튼 생성
	int OnCreate(LPCREATESTRUCT lpCreateStruct);

	//아이들 함수
	virtual BOOL OnIdle();
	//3D 초기화 함수
	void OnInit(void);
	//렌더링 함수
	void OnRender(void);
	//사이즈 변환 함수
	void OnResize(int cx, int cy);
	//FPS 함수
	void UpdateFPS();
	
	// GLCube()
	// Renders a cube.
	//그리는 함수
	void GLCube(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2);
	//회전
	void SetPitchRollYaw(float Roll, float Pitch, float Yaw);
};
#endif// __SUPEROPENGLSTATIC_B7A15A9D01DC403996FC45F7F92F52D3_H__
