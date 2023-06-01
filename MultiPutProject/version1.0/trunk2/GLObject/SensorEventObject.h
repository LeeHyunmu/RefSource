
#ifndef __SENSOREVENTOBJECT_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __SENSOREVENTOBJECT_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//���� �ε带 �⺻���� �ϴ� ��Ʈ�� Ŭ����
class CSensorEventObject : public IOpenGLObject
{
public:
	//������
	CSensorEventObject();
	~CSensorEventObject();
	//===================================
	// �����Լ� ����
	//==================================
	//�޸� ���� 
	virtual void Release();
	//Dlg Ŭ���� �ʱ�ȭ
	virtual void initOpenGLObject();
	//Idle ȣ�� �Լ�(ȸ��)
	virtual void idleRotatecall(UINT KeyValue, GLfloat& fRoll, GLfloat& fPithc, GLfloat& fYaw);
	//Idle ȣ�� �Լ�(�̵�)
	virtual void idleTranslatecall(UINT KeyValue, GLfloat& transX, GLfloat& transY, GLfloat& transZ);
	//Dlg �׸��� �Լ�
	virtual void Draw();
};
#endif //__SENSOREVENTOBJECT_74F736E25AB94b5dA789E4730F58F2A1_H__