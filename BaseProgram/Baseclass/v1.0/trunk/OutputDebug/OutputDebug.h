
#ifndef __OUTPUTDEBUG_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __OUTPUTDEBUG_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//======================================================//
//			Server ��� Include
//======================================================//
#ifdef _DEBUG
#include "TraceWinDebug.h"
#define MYDEBUG CTraceWinDebug::GetDebug()
#else
#include "TraceFileDebug.h"
#define MYDEBUG CTraceFileDebug::GetDebug()
#endif

//=======================================================//

#endif //__OUTPUTDEBUG_74F736E25AB94b5dA789E4730F58F2A1_H__