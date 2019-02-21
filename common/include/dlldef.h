#ifndef _DLL_DEF_H_
#define _DLL_DEF_H_

#define BUILDING_DLL

#ifdef BUILDING_DLL
	#define DLLIMPORT __declspec(dllexport)
#else
	#define DLLIMPORT __declspec(dllimport)
#endif

#endif
