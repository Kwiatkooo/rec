#include "CMProt.h"
#include "CMAlloc.h"
#include "CPatch.h"
#include "CHook.h"
#include "MemoryHackingFactory.h"

IMProt* MemoryHackingFactory::GetMProt()
{
	return new CMProt;
}

IMAlloc* MemoryHackingFactory::GetMAlloc()
{
	return new CMAlloc;
}

IPatch* MemoryHackingFactory::GetPatch()
{
	return new CPatch;
}

IHook* MemoryHackingFactory::GetHook()
{
	return new CHook;
}

void MemoryHackingFactory::DestroyMProt( IMProt *p)
{
	delete p;
}

void MemoryHackingFactory::DestroyMAlloc( IMAlloc *p )
{
	delete p;
}

void MemoryHackingFactory::DestroyPatch( IPatch *p)
{
	delete p;
}

void MemoryHackingFactory::DestroyHook( IHook *p )
{
	delete p;
}
