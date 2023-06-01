
#ifndef __CRC32_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __CRC32_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//======================================================//
//			전역 객체 선언
//======================================================//
#define POLYNOMIAL	0x04c11db7L
#define CRC_TABLENUM 256
//=======================================================//

class CCrc32 : public ICrc
{
public:
	//생성자
	CCrc32();
	//소멸자
	~CCrc32();

//======================================================//
//			일반함수 선언
//======================================================//
	// 테이블 생성
	void gen_crc_table();
	//생성 및 채크
	unsigned long update_crc(unsigned long crc_accum, char *data_blk_ptr,int data_blk_size);
//=================스래드 함수===========================//

//=======================================================//
//			객체 포인터 변수
//======================================================//
	IMyObject* obj;
	IOutput* debug;
//=================inner class===========================//
private:

//=======================================================//
//=======================================================//
//		 멤버	변수 선언
//======================================================//
	unsigned long crc_table[CRC_TABLENUM];

//======================================================//
//			인터페이스 선언 재 정의 함수
//======================================================//
	virtual void Release();
	virtual BOOL initCrc(IMyObject* _obj);
	//체크 값 생성
	virtual BOOL SetCheckResult(char *dataptr,int datasize);
	//값 체크하기
	virtual BOOL GetCheck(char *dataptr,int datasize);
//=======================================================//

};
#endif// __CRC32_74F736E25AB94b5dA789E4730F58F2A1_H__
