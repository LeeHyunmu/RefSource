
#ifndef __KEYEVENTOBJECT_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __KEYEVENTOBJECT_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//파일 로드를 기본으로 하는 비트맵 클래스
class CKeyEventObject : public IOpenGLObject
{
public:
	//생성자
	CKeyEventObject();
	~CKeyEventObject();
	//===================================
	// 가상함수 정의
	//==================================
	//메모리 해제 
	virtual void Release();
	//Dlg 클래스 초기화
	virtual void initOpenGLObject();
	//Idle 호출 함수(회전)
	virtual void idleRotatecall(UINT KeyValue, GLfloat& fRoll, GLfloat& fPithc, GLfloat& fYaw);
	//Idle 호출 함수(이동)
	virtual void idleTranslatecall(UINT KeyValue, GLfloat& transX, GLfloat& transY, GLfloat& transZ);
	//Dlg 그리기 함수
	virtual void Draw();
	//===================================
	// 일반함수
	//==================================
	//일반함수
	void GLCube(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2);
};
#endif //__KEYEVENTOBJECT_74F736E25AB94b5dA789E4730F58F2A1_H__