
#ifndef __INTERFACE_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __INTERFACE_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//======================================================//
//			인터페이스 헤더 Include
//======================================================//
//전체 클래스 생성 포인터를 넘겨주는 인터페이스(선선언)
interface IMyObject;
//======================================================//
//			인터페이스 헤더 Include
//======================================================//
//디버그 출력을 인터페이스
#include "IOutput.h"
#include "IServer.h"
#include "IServerMg.h"
#include "IMysqlDb.h"
#include "IMysqlDbtable.h"
#include "IADOCommand.h"
#include "IADODatabase.h"
#include "IADOException.h"
#include "IADOParameter.h"
#include "IADORecordset.h"
#include "IMobileDbMg.h"
#include "IThreadPool.h"
#include "ICrc.h"
//===========모든 인터페이스 밑에 있어야 한다=============//
#include "IMyObject.h"
//=======================================================//

#endif //__INTERFACE_74F736E25AB94b5dA789E4730F58F2A1_H__