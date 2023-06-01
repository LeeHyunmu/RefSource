
#include "stdAfx.h"

//--------------------------------------------------------------
//|
//|OpenGL ������Ʈ ���� �Ҹ���
//|
//---------------------------------------------------------------
IOpenGLObject::~IOpenGLObject(){}
//--------------------------------------------------------------
//|
//|������
//|
//---------------------------------------------------------------
CSuperOpenGLStatic::CSuperOpenGLStatic(void):
CWindowImpl< CSuperOpenGLStatic, CStatic >( ),
glObject(NULL)
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
	if(glObject)
	{
		glObject->Release();
		delete glObject; glObject=NULL;
	}
	return 0;
}
//--------------------------------------------------------------
//|
//|��ư Create �Լ� 
//|
//---------------------------------------------------------------
BOOL CSuperOpenGLStatic::OnIdle()
{
//	m_fRoll+= 1; // Add some rotation to the cube
	UpdateFPS();
	if(glObject)
	{
		glObject->idleRotatecall(m_fPithc, m_fRoll, m_fYaw);
		glObject->idleTranslatecall(transX, transY, transZ);
	}
	RedrawWindow();
	return FALSE;
}
//--------------------------------------------------------------
//|
//|��ư Create �Լ� 
//|
//---------------------------------------------------------------
void CSuperOpenGLStatic::OnInit(void) 
{
	m_fRoll = 0.0f;
	m_fPithc=-30.0f;
	m_fYaw=0.0f;

	transX = 0.0f;
	transY = 0.0f;
	transZ = -12.0f;

	glClearColor(0.000f, 0.000f, 1.000f, 1.0f); //Background color
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
	if(glObject)
	{
		glObject->initOpenGLObject();
	}
}
//--------------------------------------------------------------
//|
//|��ư Create �Լ� 
//|
//---------------------------------------------------------------
void CSuperOpenGLStatic::OnRender(void) 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear buffers
	//��ǥ�踦 �ʱ�ȭ �Ѵ�
	glLoadIdentity(); // Load identity matrix

	// Add a light source
	GLfloat glfLight[] = {-4.0f, 4.0f, 4.0f, 0.0f};
	glLightfv(GL_LIGHT0, GL_POSITION, glfLight);

	// Position and rotate the camera
	glTranslatef(transX, transY, transZ);
	//ȸ��(Pitch)
	glRotatef(m_fPithc, 1.0f, 0.0f, 0.0f);
	//ȸ��(Roll)
	glRotatef(m_fRoll, 0.0f, 1.0f, 0.0f);
	//ȸ��(Yaw)
	glRotatef(m_fYaw, 0.0f, 0.0f, 1.0f);
	if(glObject)
	{
		glObject->Draw();
	}
	glFlush();
}
//--------------------------------------------------------------
//|
//|��ư Create �Լ� 
//|
//---------------------------------------------------------------
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
//--------------------------------------------------------------
//|
//|��ư Create �Լ� 
//|
//---------------------------------------------------------------
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
//--------------------------------------------------------------
//|
//|�̵�
//|
//---------------------------------------------------------------
void CSuperOpenGLStatic::SetTranslateXYZ(float _transX, float _transY, float _transZ)
{
	//�̵� X
	transX = _transX;
	//�̵� Y
	transY = _transY;
	//�̵� Z
	transY = _transZ;
}
//--------------------------------------------------------------
//|
//|ȸ�� ���� ��ü(Pitch, Roll, Yaw)
//|
//---------------------------------------------------------------
void CSuperOpenGLStatic::SetPitchRollYaw(float Pitch, float Roll, float Yaw)
{
	//�����̼� �� ����(Roll)
	m_fRoll=Roll;
	//�����̼� �� ����(Pitch)
	m_fPithc=Pitch;
	//�����̼� �� ����(Yaw)
	m_fYaw=Yaw;
}
//--------------------------------------------------------------
//|
//|������Ʈ ��ü�� �μ� �޴� �Լ�
//|
//---------------------------------------------------------------
BOOL CSuperOpenGLStatic::initOpenGLStatic(IOpenGLObject* _openglobj)
{
	BOOL b=FALSE;
	if(_openglobj)
	{
		glObject=_openglobj; b = TRUE;
	}
	return b;
}