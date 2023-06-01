
#ifndef __SUPEROPENGLSTATIC_B7A15A9D01DC403996FC45F7F92F52D3_H__
#define __SUPEROPENGLSTATIC_B7A15A9D01DC403996FC45F7F92F52D3_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//��ư �̹��� ����
//--------------------------------------------------------------
//|
//|IOutput �������̽�
//|
//---------------------------------------------------------------
interface IOpenGLObject
{
	//�������� �Ҹ���
	virtual ~IOpenGLObject()=0;
	//�޸� ���� 
	virtual void Release()=0;
	//Dlg Ŭ���� �ʱ�ȭ
	virtual void initOpenGLObject()=0;
	//Idle ȣ�� �Լ�(ȸ��)
	virtual void idleRotatecall(UINT KeyValue, GLfloat& fRoll, GLfloat& fPithc, GLfloat& fYaw)=0;
	//Idle ȣ�� �Լ�(�̵�)
	virtual void idleTranslatecall(UINT KeyValue, GLfloat& transX, GLfloat& transY, GLfloat& transZ)=0;
	//Dlg �׸��� �Լ�
	virtual void Draw()=0;
};

class CSuperOpenGLStatic : public CWindowImpl<CSuperOpenGLStatic, CStatic>, COpenGL<CSuperOpenGLStatic>, public CMessageFilter, public CIdleHandler
{
	//Ű���� ��
	UINT KeyValue;
	//�����̼� �� ����(Roll)
	GLfloat m_fRoll;
	//�����̼� �� ����(Pitch)
	GLfloat m_fPithc;
	//�����̼� �� ����(Yaw)
	GLfloat m_fYaw;
	//�̵� X 
	GLfloat transX;
	//�̵� Y 
	GLfloat transY;
	//�̵� Z 
	GLfloat transZ;
	//������Ʈ ��ü
	IOpenGLObject* glObject;
public:
	DECLARE_WND_SUPERCLASS(_T("SuperTextStatic"), _T("STATIC"))
	BEGIN_MSG_MAP_EX(CSuperTextStatic)
		MSG_WM_CREATE(OnCreate)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		CHAIN_MSG_MAP(COpenGL<CSuperOpenGLStatic>)
		DEFAULT_REFLECTION_HANDLER()
		END_MSG_MAP()
	//������
	CSuperOpenGLStatic(void);
	//�Ҹ���
	~CSuperOpenGLStatic(void);
	//
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//�޸𸮿��� ����
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	//����Ŭ���� �Լ�
	BOOL SubclassWindow(HWND hWnd);
	HWND UnsubclassWindow(BOOL bForce = FALSE);
	//��ư ����
	int OnCreate(LPCREATESTRUCT lpCreateStruct);

	//���̵� �Լ�
	virtual BOOL OnIdle();
	//3D �ʱ�ȭ �Լ�
	void OnInit(void);
	//������ �Լ�
	void OnRender(void);
	//������ ��ȯ �Լ�
	void OnResize(int cx, int cy);
	//FPS �Լ�
	void UpdateFPS();
	
	// GLCube()
	// Renders a cube.
	//�׸��� �Լ�
	//void GLCube(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2);
	//ȸ��
	void SetPitchRollYaw(float Roll, float Pitch, float Yaw);
	//�̵�
	void SetTranslateXYZ(float _transX, float _transY, float _transZ);
	//Ű���� ���� ����
	void SetVirtualKey(UINT _KeyValue);
	//������Ʈ ��ü�� �μ� �޴� �Լ�
	BOOL initOpenGLStatic(IOpenGLObject* _openglobj);
};
#endif// __SUPEROPENGLSTATIC_B7A15A9D01DC403996FC45F7F92F52D3_H__
