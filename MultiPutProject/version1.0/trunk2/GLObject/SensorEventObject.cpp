
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
//|생성자
//|
//---------------------------------------------------------------
CSensorEventObject::CSensorEventObject()
{}
//--------------------------------------------------------------
//|
//|소멸자
//|
//---------------------------------------------------------------
CSensorEventObject::~CSensorEventObject()
{}
//--------------------------------------------------------------
//|
//|	메모리 해제 
//|
//---------------------------------------------------------------
void CSensorEventObject::Release()
{

}
//--------------------------------------------------------------
//|
//|OpenGl 객체 초기화
//|
//---------------------------------------------------------------
void CSensorEventObject::initOpenGLObject()
{

}
//--------------------------------------------------------------
//|
//|Idle 호출 함수(회전)
//|
//---------------------------------------------------------------
void CSensorEventObject::idleRotatecall(UINT KeyValue, GLfloat& fRoll, GLfloat& fPithc, GLfloat& fYaw)
{

}
//--------------------------------------------------------------
//|
//|Idle 호출 함수(이동)
//|
//---------------------------------------------------------------
void CSensorEventObject::idleTranslatecall(UINT KeyValue, GLfloat& transX, GLfloat& transY, GLfloat& transZ)
{

}
//--------------------------------------------------------------
//|
//|그리기 함수
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
	* 테두리 선을 그린다.
	*/
	glDrawElements(GL_LINE_LOOP, sizeof(indices), GL_UNSIGNED_BYTE, indices);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);	
}