
#ifndef __MYSQLDBTABLE_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __MYSQLDBTABLE_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//======================================================//
//			���� ��ü ����
//======================================================//

//=======================================================//

class CMysqlDbtable : public IMysqlDbtable
{
public:
	//������
	CMysqlDbtable();
	//�Ҹ���
	~CMysqlDbtable();
//======================================================//
//			�Ϲ��Լ� ����
//======================================================//
	void GetErrorErrStr(char* ErrStr);
	int ISEOF();
	HRESULT MoveNext();
	HRESULT MovePrevious();
	HRESULT MoveFirst();
	HRESULT MoveLast();
	int AddNew();
	int Update();
	int AddFieldValue(char* FieldName, char* FieldValue);
	int AddFieldValue(char* FieldName,int FieldValue);
	int AddFieldValue(char* FieldName,float FieldValue);
	int AddFieldValue(char* FieldName,double FieldValue);
	int AddFieldValue(char* FieldName,long FieldValue);
	BOOL GetFieldValue(char* FieldName, char* FieldValue);
	BOOL GetFieldValue(char* FieldName,int& FieldValue);
	BOOL GetFieldValue(char* FieldName,float& FieldValue);
	BOOL GetFieldValue(char* FieldName,double& FieldValue);
	BOOL GetFieldValue(char* FieldName,double& FieldValue,int Scale);
	BOOL GetFieldValue(char* FieldName,long& FieldValue);
//=================������ �Լ�===========================//

//=======================================================//
//			��ü ������ ����
//======================================================//
	IMyObject* obj;
	IOutput* debug;
//=======================================================//
//			���� ����
//======================================================//
public:
	RecPtr m_Rec;
private:

//======================================================//
//			�������̽� ���� �� ���� �Լ�
//======================================================//
	virtual void Release();
	//���ڵ� ������ ����
	virtual void SetRecordPtr(RecPtr _rec);
	virtual BOOL initMysqlDbtable(IMyObject* _obj);
//=======================================================//

};
#endif// __MYSQLDBTABLE_74F736E25AB94b5dA789E4730F58F2A1_H__
