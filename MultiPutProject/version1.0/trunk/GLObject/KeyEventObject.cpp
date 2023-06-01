
#include "stdafx.h"

//--------------------------------------------------------------
//|
//|생성자
//|
//---------------------------------------------------------------
CKeyEventObject::CKeyEventObject()
{}
//--------------------------------------------------------------
//|
//|소멸자
//|
//---------------------------------------------------------------
CKeyEventObject::~CKeyEventObject()
{}
//--------------------------------------------------------------
//|
//|	메모리 해제 
//|
//---------------------------------------------------------------
void CKeyEventObject::Release()
{

}
//--------------------------------------------------------------
//|
//|OpenGl 객체 초기화
//|
//---------------------------------------------------------------
void CKeyEventObject::initOpenGLObject()
{

}
//--------------------------------------------------------------
//|
//|Idle 호출 함수(회전)
//|
//---------------------------------------------------------------
void CKeyEventObject::idleRotatecall(GLfloat& fRoll, GLfloat& fPithc, GLfloat& fYaw)
{

}
//--------------------------------------------------------------
//|
//|Idle 호출 함수(이동)
//|
//---------------------------------------------------------------
void CKeyEventObject::idleTranslatecall(GLfloat& transX, GLfloat& transY, GLfloat& transZ)
{

}
//--------------------------------------------------------------
//|
//|Dlg 그리기 함수
//|
//---------------------------------------------------------------
void CKeyEventObject::Draw()
{
	// Draw a cube
	static GLfloat r = .7f;
	GLCube(-r, -r, -r, r, r, r);
}
// GLCube()
// Renders a cube.
//
//--------------------------------------------------------------
//|
//|버튼 Create 함수 
//|
//---------------------------------------------------------------
void CKeyEventObject::GLCube(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2)
{
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glVertex3f(x2, y2, z2);
	glVertex3f(x1, y2, z2);
	glVertex3f(x1, y1, z2);
	glVertex3f(x2, y1, z2);
	glEnd();

	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, -1.0f);
	glVertex3f(x2, y2, z1);
	glVertex3f(x2, y1, z1);
	glVertex3f(x1, y1, z1);
	glVertex3f(x1, y2, z1);
	glEnd();

	glBegin(GL_QUADS);
	glNormal3f(-1.0f, 0.0f, 0.0f);
	glVertex3f(x1, y2, z2);
	glVertex3f(x1, y2, z1);
	glVertex3f(x1, y1, z1);
	glVertex3f(x1, y1, z2);
	glEnd();

	glBegin(GL_QUADS);
	glNormal3f(1.0f, 0.0f, 0.0f);
	glVertex3f(x2, y2, z2);
	glVertex3f(x2, y1, z2);
	glVertex3f(x2, y1, z1);
	glVertex3f(x2, y2, z1);
	glEnd();

	glBegin(GL_QUADS);
	glNormal3f(0.0f, 1.0f, 0.0f);
	glVertex3f(x1, y2, z1);
	glVertex3f(x1, y2, z2);
	glVertex3f(x2, y2, z2);
	glVertex3f(x2, y2, z1);
	glEnd();

	glBegin(GL_QUADS);
	glNormal3f(0.0f, -1.0f, 0.0f);
	glVertex3f(x1, y1, z1);
	glVertex3f(x2, y1, z1);
	glVertex3f(x2, y1, z2);
	glVertex3f(x1, y1, z2);
	glEnd();
}
