
#ifndef __GATYPECHANGE_B7A15A9D01DC403996FC45F7F92F52D3_H__
#define __GATYPECHANGE_B7A15A9D01DC403996FC45F7F92F52D3_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define  _GATYPECH  GATypeChange
class GATypeChange
{
public:
	//������
	GATypeChange(void);
	//�Ҹ���
	~GATypeChange(void);
	//�ٻ縦 ����Ʈ�� ��ȯ(�����͸� �ѱ�� �Ҵ�� �޸��� ���� ���� �����Ѵ�)
	static int GAChangeHexCordInt(CString data, DWORD dwLen);
	static BOOL ChangStringbyhexString(CString pass, CString& repass);
};
#endif// __GATYPECHANGE_B7A15A9D01DC403996FC45F7F92F52D3_H__
