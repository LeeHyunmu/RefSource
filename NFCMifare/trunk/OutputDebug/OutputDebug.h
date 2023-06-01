
#ifndef __OUTPUTDEBUG_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __OUTPUTDEBUG_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//======================================================//
//			Server Çì´õ Include
//======================================================//
#ifdef _DEBUG
	#define DEBUGVIEWER 1
#if DEBUGVIEWER
	#include "TraceWinDebug.h"
#endif //DEBUGVIEWER
#else
	#define DEBUGVIEWER 0
#if DEBUGVIEWER
	#include "TraceFileDebug.h"
#endif //DEBUGVIEWER
#endif
//=======================================================//

#endif //__OUTPUTDEBUG_74F736E25AB94b5dA789E4730F58F2A1_H__