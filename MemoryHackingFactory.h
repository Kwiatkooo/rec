#ifndef _MEMORYHACKINGFACTORY_H
#define _MEMORYHACKINGFACTORY_H

class IMProt;
class IMAlloc;
class IPatch;
class IHook;

class MemoryHackingFactory
{
public:
	static IMProt* GetMProt();
	static IMAlloc* GetMAlloc();
	static IPatch* GetPatch();
	static IHook* GetHook();

	static void DestroyMProt( IMProt *p );
	static void DestroyMAlloc( IMAlloc *p );
	static void DestroyPatch( IPatch *p );
	static void DestroyHook( IHook *p );
};

#endif