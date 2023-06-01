
#ifndef __IOUTPUT_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __IOUTPUT_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//--------------------------------------------------------------
//|
//|IOutput �������̽�
//|
//---------------------------------------------------------------
interface IOutput
{
	//��� Ŭ�����Ҹ������� �Ҹ���
	virtual ~IOutput()=0;
	//��� ȣ�� �Լ�
	virtual void Write(TCHAR* format,...)=0;
	//����
	virtual void Clear()=0;
	//�����â ���̰� �Ⱥ��̰� ����
	virtual void SetVisible(BOOL visible)=0;
	//����� â�� ���̴°�?
	virtual BOOL GetVisible()=0;
	//������ ����Լ�
	virtual void OutputString()=0;
	//�޸� ���� 
	virtual void Release()=0;
};

#endif //__IOUTPUT_74F736E25AB94b5dA789E4730F58F2A1_H__