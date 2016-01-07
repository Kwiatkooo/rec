#ifndef _IPATCH_H
#define _IPATCH_H

class IPatch
{
public:
	virtual bool Register( void *lpAddress, void *lpPatch, unsigned int uiSize ) = 0;
	virtual bool Install() = 0;
	virtual bool Uninstall() = 0;
	virtual bool Installed() = 0;
};

#endif