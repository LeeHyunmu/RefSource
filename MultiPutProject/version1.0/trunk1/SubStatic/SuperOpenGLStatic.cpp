
#include "stdAfx.h"

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
void draw()
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

//--------------------------------------------------------------
//|
//|������
//|
//---------------------------------------------------------------
CSuperOpenGLStatic::CSuperOpenGLStatic(void):
CWindowImpl< CSuperOpenGLStatic, CStatic >( )
{
}
//--------------------------------------------------------------
//|
//|�Ҹ���
//|
//---------------------------------------------------------------
CSuperOpenGLStatic::~CSuperOpenGLStatic(void)
{
}
//--------------------------------------------------------------
//|
//|���� Ŭ���� �Լ�
//|
//---------------------------------------------------------------
BOOL CSuperOpenGLStatic::SubclassWindow(HWND hWnd)
{
	ATLASSERT(::IsWindow(hWnd));
	BOOL bRet = CWindowImpl<CSuperOpenGLStatic, CStatic>::SubclassWindow(hWnd);
	return bRet;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
BOOL CSuperOpenGLStatic::PreTranslateMessage(MSG* pMsg)
{
	return CWindow::IsDialogMessage(pMsg);
}
//--------------------------------------------------------------
//|
//|���� Ŭ���� ����
//|
//---------------------------------------------------------------
HWND CSuperOpenGLStatic::UnsubclassWindow(BOOL bForce /*= FALSE*/)
{
	return CWindowImpl<CSuperOpenGLStatic, CStatic>::UnsubclassWindow(bForce);
}
//--------------------------------------------------------------
//|
//|��ư Create �Լ� 
//|
//---------------------------------------------------------------
int CSuperOpenGLStatic::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	//����Ʈ���� �޽���
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);
	SetMsgHandled(FALSE);
	return 0;
}
//--------------------------------------------------------------
//|
//|Create ������ Desroy �ƴϸ� �̻� �߻�(�����)
//|
//---------------------------------------------------------------
LRESULT CSuperOpenGLStatic::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	pLoop->RemoveIdleHandler(this);
	SetMsgHandled(FALSE);
	return 0;
}
BOOL CSuperOpenGLStatic::OnIdle()
{
//	m_fRoll+= 1; // Add some rotation to the cube
	UpdateFPS();
	RedrawWindow();
	return FALSE;
}

void CSuperOpenGLStatic::OnInit(void) 
{
	m_fRoll = 0.0f;
	m_fPithc=0.0f;
	m_fYaw=0.0f;

	glClearColor(0.000f, 0.000f, 1.000f, 1.0f); //Background color
	glFrontFace(GL_CCW);
	// Activate lighting and a light source
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);

	// Define material parameters
	static GLfloat glfMatAmbient[] = {0.000f, 0.450f, 1.000f, 1.0f};
	static GLfloat glfMatDiffuse[] = {0.000f, 0.000f, 0.580f, 1.0f};
	static GLfloat glfMatSpecular[]= {1.000f, 1.000f, 1.000f, 1.0f};
	static GLfloat glfMatEmission[]= {0.000f, 0.000f, 0.000f, 1.0f};
	static GLfloat fShininess = 128.000f;

	// Set material parameters
	glMaterialfv(GL_FRONT, GL_AMBIENT,  glfMatAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE,  glfMatDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, glfMatSpecular);
	glMaterialfv(GL_FRONT, GL_EMISSION, glfMatEmission);
	glMaterialf(GL_FRONT, GL_SHININESS, fShininess);
}

void CSuperOpenGLStatic::OnRender(void) 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear buffers
	//��ǥ�踦 �ʱ�ȭ �Ѵ�
	glLoadIdentity(); // Load identity matrix

	// Add a light source
	GLfloat glfLight[] = {-4.0f, 4.0f, 4.0f, 0.0f};
	glLightfv(GL_LIGHT0, GL_POSITION, glfLight);

	// Position and rotate the camera
	glTranslatef(0.0f, 0.0f, -12.0f);
	//ȸ��(Pitch)
	glRotatef(m_fPithc, 1.0f, 0.0f, 0.0f);
	//ȸ��(Roll)
	glRotatef(m_fRoll, 0.0f, 1.0f, 0.0f);
	//ȸ��(Yaw)
	glRotatef(m_fYaw, 0.0f, 0.0f, 1.0f);

	// Draw a cube
	//static GLfloat r = .7f;
	//GLCube(-r, -r, -r, r, r, r);
	draw();

	glFlush();
}

void CSuperOpenGLStatic::OnResize(int cx, int cy)
{
	GLfloat fFovy  = 30.0f; // Field-of-view
	GLfloat fZNear = 1.0f;  // Near clipping plane
	GLfloat fZFar = 30.0f;  // Far clipping plane

	// Calculate viewport aspect
	RECT rc;
	GetClientRect(&rc);

	GLfloat fAspect = (GLfloat)(rc.right-rc.left) / (GLfloat)(rc.bottom-rc.top);

	// Define viewport
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fFovy, fAspect, fZNear, fZFar);
	glViewport(rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top);
	glMatrixMode(GL_MODELVIEW);
}

void CSuperOpenGLStatic::UpdateFPS()
{
	const int FRAMEINTERVAL = 1000;            // Show FPS every 1000th milliseconds
	static DWORD nFrames = 0;                  // Number of frames since last update
	static DWORD nLastTick = GetTickCount();   // Time of last update
	DWORD nTick = GetTickCount();              // Current time
	if(nTick-nLastTick>=FRAMEINTERVAL)
	{	
		float fFPS = 1000.0f*(GLfloat)nFrames/(GLfloat)(nTick-nLastTick);
		nLastTick = nTick;
		nFrames = 0;
		//char szFPS[256];
		//sprintf_s(szFPS,256, "%.1f FPS", fFPS);
		//SetWindowText(szFPS);
	}
	nFrames++;
}
// GLCube()
// Renders a cube.
//
void CSuperOpenGLStatic::GLCube(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2)
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
//
void CSuperOpenGLStatic::SetPitchRollYaw(float Pitch, float Roll, float Yaw)
{
	//�����̼� �� ����(Roll)
	m_fRoll=Roll;
	//�����̼� �� ����(Pitch)
	m_fPithc=Pitch;
	//�����̼� �� ����(Yaw)
	m_fYaw=Yaw;
}