
#ifndef __INTERFACE_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __INTERFACE_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//======================================================//
//			�������̽� ��� Include
//======================================================//
//��ü Ŭ���� ���� �����͸� �Ѱ��ִ� �������̽�(������)
interface IMyObject;
//======================================================//
//			�������̽� ��� Include
//======================================================//
//����� ����� �������̽�
#include "ICrc.h"   
#include "IOutput.h" 
#include "IMysqlDb.h" 
#include "IMysqlDbtable.h"
#include "IADOCommand.h" 
#include "IADODatabase.h"
#include "IADOException.h"
#include "IADOParameter.h"
#include "IADORecordset.h"
#include "IDatabaseMg.h"
#include "IServer.h"
#include "IServerMg.h"

//===========������ Ǯ �������̽�=============//
//������ Ǯ�� �����  ���� �������̽��� ���� �����Ѵ�
#include "IThreadPool.h"
//===========��� �������̽� �ؿ� �־�� �Ѵ�=============//
#include "IMyObject.h"
//=======================================================//

#endif //__INTERFACE_74F736E25AB94b5dA789E4730F58F2A1_H__