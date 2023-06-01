
#include "stdafx.h"

//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
float 	colors[] = 
{
	0.0f, 1.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f, 1.0f,
	1.0f, 0.0f, 0.0f, 1.0f,

};
float	vertices[] =
{				
	0.0f, 2.0f, 0.0f,
	1.0f, -0.7f, 0.0f,
	0.0f, -0.2f, 0.0f,
	-1.0f, -0.7f, 0.0f,
	0.0f, -0.2f, 1.5f,
};

float	texture[] = {
	0.0f,	0.0f,
	1.0f,	0.0f,
	1.0f,	1.0f,
	0.0f,	1.0f,				
};

BYTE	indices[] = 
{
	0,1,2,
	0,2,3,
	0,1,4,
	0,4,3,
	4,1,2,
	4,2,3
};
//--------------------------------------------------------------
//|
//|������
//|
//---------------------------------------------------------------
CSensorEventObject::CSensorEventObject()
{}
//--------------------------------------------------------------
//|
//|�Ҹ���
//|
//---------------------------------------------------------------
CSensorEventObject::~CSensorEventObject()
{}
//--------------------------------------------------------------
//|
//|	�޸� ���� 
//|
//---------------------------------------------------------------
void CSensorEventObject::Release()
{

}
//--------------------------------------------------------------
//|
//|OpenGl ��ü �ʱ�ȭ
//|
//---------------------------------------------------------------
void CSensorEventObject::initOpenGLObject()
{

}
//--------------------------------------------------------------
//|
//|Idle ȣ�� �Լ�(ȸ��)
//|
//---------------------------------------------------------------
void CSensorEventObject::idleRotatecall(UINT KeyValue, GLfloat& fRoll, GLfloat& fPithc, GLfloat& fYaw)
{

}
//--------------------------------------------------------------
//|
//|Idle ȣ�� �Լ�(�̵�)
//|
//---------------------------------------------------------------
void CSensorEventObject::idleTranslatecall(UINT KeyValue, GLfloat& transX, GLfloat& transY, GLfloat& transZ)
{

}
//--------------------------------------------------------------
//|
//|�׸��� �Լ�
//|
//---------------------------------------------------------------
void CSensorEventObject::Draw()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glFrontFace(GL_CW);
	glVertexPointer(3, GL_FLOAT, 0 ,vertices);
	glTexCoordPointer(2, GL_FLOAT, 0 ,texture);
	/*
	* �׵θ� ���� �׸���.
	*/
	glDrawElements(GL_LINE_LOOP, sizeof(indices), GL_UNSIGNED_BYTE, indices);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);	
}