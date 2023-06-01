
#ifndef __MYSQLDBTABLE_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __MYSQLDBTABLE_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//======================================================//
//			전역 객체 선언
//======================================================//

//=======================================================//

class CMysqlDbtable : public IMysqlDbtable
{
public:
	//생성자
	CMysqlDbtable();
	//소멸자
	~CMysqlDbtable();
//======================================================//
//			일반함수 선언
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
//=================스래드 함수===========================//

//=======================================================//
//			객체 포인터 변수
//======================================================//
	IMyObject* obj;
	IOutput* debug;
//=======================================================//
//			변수 선언
//======================================================//
public:
	RecPtr m_Rec;
private:

//======================================================//
//			인터페이스 선언 재 정의 함수
//======================================================//
	virtual void Release();
	//레코드 포인터 저장
	virtual void SetRecordPtr(RecPtr _rec);
	virtual BOOL initMysqlDbtable(IMyObject* _obj);
//=======================================================//

};
#endif// __MYSQLDBTABLE_74F736E25AB94b5dA789E4730F58F2A1_H__
