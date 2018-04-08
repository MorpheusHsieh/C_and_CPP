

#ifndef _ExportFuns
#define _ExportFuns

#include <vcl\Classes.hpp>
//extern "C" void WINAPI genkey128(char *,char *);
extern "C" void WINAPI genkeys(int keylen, char* pkey,char *mkey);
extern "C" void WINAPI lmodexp(unsigned long * ,unsigned long * ,unsigned long * ,unsigned long * );
extern "C" void WINAPI lmod(unsigned long * ,unsigned long *);
extern "C" void WINAPI lmodmul(unsigned long *,unsigned long *,unsigned long *,unsigned long *);
extern "C" int  WINAPI lcompare(unsigned long *,unsigned long *);
extern "C" void WINAPI lshiftleft(unsigned long *);
extern "C" void WINAPI lshiftright(unsigned long *);
extern "C" void WINAPI lsub(unsigned long *,unsigned long *);
extern "C" void WINAPI ladd(unsigned long *,unsigned long *);
#endif

