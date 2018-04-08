//modexp.h

#ifndef _ModExp
#define _ModExp

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#include<conio.h>
#define FULL  0xffffffff
#define LMASK 0x80000000   /* Mask the  leftest bit of a long integer.  */
#define RMASK 0x00000001   /* Mask the rightest bit of a long integer.  */
#define NNINE 0x3b9ac9ff   /* NNINE is 999999999(10^9-1).               */
//#define LENGTH 5           /* The # of long integers in a large integer.*/

#define LENGTH 33           /* The # of long integers in a large integer.*/
typedef unsigned long LINT[LENGTH+1];

extern int  _fastcall compare(unsigned long *,unsigned long *);
extern void _fastcall shiftleft(unsigned long *);
extern void _fastcall shiftright(unsigned long *);
extern void _fastcall add(unsigned long *,unsigned long *);
extern void _fastcall sub(unsigned long *,unsigned long *);
extern void _fastcall mod(unsigned long *,unsigned long *);
extern void _fastcall modmul(unsigned long *,unsigned long *,unsigned long *,unsigned long *);
extern void _fastcall modexp(unsigned long *,unsigned long *,unsigned long *,unsigned long *);
#endif