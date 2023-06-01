
#include "stdafx.h"

//--------------------------------------------------------------
//|
//|������
//|
//---------------------------------------------------------------
CKeyEventObject::CKeyEventObject()
{}
//--------------------------------------------------------------
//|
//|�Ҹ���
//|
//---------------------------------------------------------------
CKeyEventObject::~CKeyEventObject()
{}
//--------------------------------------------------------------
//|
//|	�޸� ���� 
//|
//---------------------------------------------------------------
void CKeyEventObject::Release()
{

}
//--------------------------------------------------------------
//|
//|OpenGl ��ü �ʱ�ȭ
//|
//---------------------------------------------------------------
void CKeyEventObject::initOpenGLObject()
{

}
//--------------------------------------------------------------
//|
//|Idle ȣ�� �Լ�(ȸ��)
//|
//---------------------------------------------------------------
void CKeyEventObject::idleRotatecall(UINT KeyValue, GLfloat& fRoll, GLfloat& fPithc, GLfloat& fYaw)
{
	switch(KeyValue)
	{
	case GLKEYDOWN_RIGHTARROW:
		{
			fYaw+=0.5f;
			if(fYaw>360.0f)
				fYaw=0.1f;
		}
		break;
	case GLKEYDOWN_LEFTARROW:
		{
			fYaw-=0.5f;
			if(fYaw < -360.0f)
				fYaw = -0.1f;
		}
		break;
	}
}
//--------------------------------------------------------------
//|
//|Idle ȣ�� �Լ�(�̵�)
//|
//---------------------------------------------------------------
void CKeyEventObject::idleTranslatecall(UINT KeyValue, GLfloat& transX, GLfloat& transY, GLfloat& transZ)
{
	switch(KeyValue)
	{
	case GLKEYDOWN_DOWNARROW:
		{
			transZ+=0.05f;
			if(transZ > -5.0f)
				transZ = -5.0f;
		}
		break;
	case GLKEYDOWN_UPARROW:
		{
			transZ-=0.05f;
			if(transZ < -40.0f)
				transZ = -40.0f;
		}
		break;
	}
}
//--------------------------------------------------------------
//|
//|Dlg �׸��� �Լ�
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
//|��ư Create �Լ� 
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
