
#include "stdAfx.h"
#include "Crc32.h"

//--------------------------------------------------------------
//|
//|������
//|
//---------------------------------------------------------------
CCrc32::CCrc32():
obj(NULL),
debug(NULL)
{
}
//--------------------------------------------------------------
//|
//|�Ҹ���
//|
//---------------------------------------------------------------
CCrc32::~CCrc32()
{
}
//--------------------------------------------------------------
//|
//|��ü �ʱ�ȭ �Լ�
//|
//---------------------------------------------------------------
BOOL CCrc32::initCrc(IMyObject* _obj)
{
	//=====================================================
	// �� ��ü ���� Ŭ���� ������Ʈ
	//======================================================
	obj=_obj;
	//=====================================================
	// ����� ��� Ŭ����
	//======================================================
	debug=obj->GetOutput();
	// ���̺� ����
	gen_crc_table();
	return TRUE;
}
//--------------------------------------------------------------
//|
//|������� ��ü�� �޸𸮿��� �����Ѵ�
//|
//---------------------------------------------------------------
void CCrc32::Release()
{
}
//--------------------------------------------------------------
//|
//|�׽��� ����
//|
//---------------------------------------------------------------
void CCrc32::gen_crc_table()
{
	register int i, j;
	register unsigned long crc_accum;
	for(i=0; i<CRC_TABLENUM; i++)
	{
		crc_accum = ((unsigned long) i << 24);
		for(j=0; j<8; j++)
		{
			if(crc_accum & 0x80000000L)
				crc_accum = (crc_accum << 1) ^ POLYNOMIAL;
			else
				crc_accum = (crc_accum << 1);
		}
		crc_table[i] = crc_accum;
		debug->Write("%03d=%lx", i, crc_accum);
	}
	return;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
unsigned long CCrc32::update_crc(unsigned long crc_accum, char *data_blk_ptr,int data_blk_size)
{
	register int i, j;

	for(j=0; j<data_blk_size; j++)
	{
		i = ((int)(crc_accum >> 24) ^ *data_blk_ptr++) & 0xff;
		crc_accum = (crc_accum << 8) ^ crc_table[i];
	}
	return crc_accum;
}
BOOL CCrc32::SetCheckResult(char *dataptr,int datasize)
{
	unsigned long crc=0;
	crc = update_crc(crc,dataptr, datasize);
	dataptr[datasize] =(char) ((crc & 0xFF000000L) >> 24);
	dataptr[datasize+1] = (char)((crc & 0x00FF0000L) >> 16);
	dataptr[datasize+2] =(char) ((crc & 0x0000FF00L) >> 8);
	dataptr[datasize+3] = (char)(crc & 0x000000FFL);
	return TRUE;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
BOOL CCrc32::GetCheck(char *dataptr,int datasize)
{
	unsigned long check=0;
	check = update_crc(check,dataptr, datasize);
	if(check==0)
	{
		debug->Write("������ ���� OK");
		return TRUE;
	}
	debug->Write("������ ���� NO");
	return FALSE;
}