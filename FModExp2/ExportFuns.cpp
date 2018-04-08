// ExportFuns.cpp
// Define the export functions;

//#include <vcl\vcl.h>
#include "ModExp.h"
#include "KeyGen.h"
#include "ExportFuns.h"

//***********************************************************************************************
//The ModExp.cpp export functions
//

void WINAPI lmodexp(unsigned long *ex,unsigned long *ev,unsigned long *ep,unsigned long *ew)
{
	modexp(ex,ev,ep,ew);
}

void WINAPI lmod(unsigned long *ma,unsigned long *mb)
{
	mod(ma,mb);
}

void WINAPI lmodmul(unsigned long *mx,unsigned long *my,unsigned long *mp,unsigned long *mz)
{
	modmul(mx,my,mp,mz);
}

int WINAPI lcompare(unsigned long *p1,unsigned long *p2)
{
	return compare(p1,p2);
}

void WINAPI lshiftleft(unsigned long *sl)
{
	shiftleft(sl);
}

void WINAPI lshiftright(unsigned long *rl)
{
	shiftright(rl);
}

void WINAPI lsub(unsigned long *p1,unsigned long *p2)
{
	sub(p1,p2);
}

void WINAPI ladd(unsigned long *p1,unsigned long *p2)
{
	add(p1,p2);
}

void WINAPI genkeys(int keylen, char* pkey,char *mkey)
// pkey public key buffer  , the length must > 612  ( 630 ok)
// mkey private key buffer , the length must > 1224 (1260 ok)
{
	_genkeys(keylen, pkey, mkey);
}

