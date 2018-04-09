#include <dos.h>
#include "amouse.h"
#define MOUSE 0x33


static union REGS r;
static struct SREGS s;

char msreset()
{
   r.x.ax=0;
   int86(MOUSE, &r, &r);
   return r.x.ax;
}

char msgetbutton()
{
   r.x.ax=0;
   int86(MOUSE, &r, &r);
   return r.x.ax;
}

void msshow()
{
   r.x.ax=1;
   int86(MOUSE, &r, &r);
}

void mshide()
{
   r.x.ax=2;
   int86(MOUSE, &r, &r);
}

char msleftpressed()
{
   r.x.ax=3;
   int86(MOUSE, &r, &r);
   return(r.x.bx&1);
}

char msrightpressed()
{
   r.x.ax=3;
   int86(MOUSE, &r, &r);
   return(r.x.bx&2);
}

char msmidpressed()
{
   r.x.ax=3;
   int86(MOUSE, &r, &r);
   return(r.x.bx&4);
}

void msgetxy(int *x, int *y)
{
   r.x.ax=3;
   int86(MOUSE, &r, &r);
   *x=r.x.cx;
   *y=r.x.dx;
}

void mstgetxy(int *x, int *y)
{
   msgetxy(x, y);
   *x=*x/8+1;
   *y=*y/8+1;
}

void msgotoxy(int x, int y)
{
   r.x.ax=4;
   r.x.cx=x;
   r.x.dx=y;
   int86(MOUSE, &r, &r);
}

void mstgotoxy(int x, int y)
{
   msgotoxy((x-1)*8, (y-1)*8);
}

unsigned mspressedstatus(int button, int *x, int *y)
{
   r.x.ax=5;
   r.x.bx=button;
   int86(MOUSE, &r, &r);
   *x=r.x.cx;
   *y=r.x.dx;
   return r.x.bx;
}                             /* pressed status counter */

unsigned msreleasestatus(int button, int *x, int *y)
{
   r.x.ax=6;
   r.x.bx=button;
   int86(MOUSE, &r, &r);
   *x=r.x.cx;
   *y=r.x.dx;
   return r.x.bx;             /* release status counter */
}

void mssetxlim(int min, int max)
{
   r.x.ax=7;
   r.x.cx=min;
   r.x.dx=max;
   int86(MOUSE, &r, &r);
}

void mssetylim(int min, int max)
{
   r.x.ax=8;
   r.x.cx=min;
   r.x.dx=max;
   int86(MOUSE, &r, &r);
}

void mssetcursor(int x, int y, unsigned int far *pattern)
{
   r.x.ax=9;
   r.x.bx=x;
   r.x.cx=y;
   r.x.dx=FP_OFF(pattern);
   s.es=FP_SEG(pattern);
   int86x(MOUSE, &r, &r, &s);
}

void mstsetcursor(int type, int screen_mask, int cursor_mask)
{
   r.x.ax=10;
   r.x.bx=type;
   r.x.cx=screen_mask;
   r.x.dx=cursor_mask;
   int86(MOUSE, &r, &r);
}

int msmotion(int *x, int *y)
{
   r.x.ax=11;
   int86(MOUSE, &r, &r);
   *x=r.x.cx;
   *y=r.x.dx;
   return(*x||*y);
}

void mssetevent(unsigned int mask, void far *handler)
{
   r.x.ax=12;
   r.x.cx=mask;
   r.x.dx=FP_OFF(handler);
   s.es=FP_SEG(handler);
   int86x(MOUSE, &r, &r, &s);
}

void mslighton()
{
   r.x.ax=13;
   int86(MOUSE, &r, &r);
}

void mslightoff()
{
   r.x.ax=14;
   int86(MOUSE, &r, &r);
}

void mssetratio(int ratx, int raty)
{
   r.x.ax=15;
   r.x.cx=ratx;  /* ratio of *x */
   r.x.dx=raty;  /* ratio of *y */
   int86(MOUSE, &r, &r);
}

void msblockhind(int sx, int sy, int ex, int ey)
{
   r.x.ax=16;
   r.x.cx=sx;
   r.x.dx=sy;
   r.x.si=ex;
   r.x.di=ey;
   int86(MOUSE, &r, &r);
}

void msspeed(int speed)
{
   r.x.ax=19;
   r.x.dx=speed;
   int86(MOUSE, &r, &r);
}

void far *msswapevent(unsigned int *mask, void far *handler)
{
   void far *oldhandler;
   r.x.ax=20;
   r.x.cx=*mask;
   r.x.dx=FP_OFF(handler);
   s.es=FP_SEG(handler);
   int86(MOUSE, &r, &r);
   oldhandler=MK_FP(s.es, r.x.dx);
   *mask=r.x.cx;
   return(oldhandler);
}

int msgetstorage()
{
   r.x.ax=21;
   int86(MOUSE, &r, &r);
   return(r.x.bx);
}

void msgetstate(void *buf)
{
   r.x.ax=22;
   r.x.dx=FP_OFF(buf);
   segread(&s);
   s.es=s.ds;
   int86(MOUSE, &r, &r);
}

void msputstate(void *buf)
{
   r.x.ax=23;
   r.x.dx=FP_OFF(buf);
   segread(&s);
   s.es=s.ds;
   int86(MOUSE, &r, &r);
}

void mssetallevent(unsigned int mask, void far *handler)
{
   r.x.ax=24;
   r.x.cx=mask;
   r.x.dx=FP_OFF(handler);
   s.es=FP_SEG(handler);
   int86(MOUSE, &r, &r);
}

void far *msgetallevent(unsigned *mask)
{
   void far *handler;
   r.x.ax=25;
   r.x.cx=*mask;
   int86(MOUSE, &r, &r);
   handler=MK_FP(r.x.bx, r.x.dx);
   *mask=r.x.cx;
   return(handler);
}

char msinit()
{
   r.x.ax=33;
   int86(MOUSE, &r, &r);
   if (r.x.ax==33)
      return(0);
   else if (r.x.ax==65535 && r.x.bx==2)
      return(1);
}

unsigned mscursor[3][32]={{
/* mscursor[0] ---->  Hand Screen Mask */
0xE1FF, /* 1110000111111111 */
0xE1FF, /* 1110000111111111 */
0xE1FF, /* 1110000111111111 */
0xE1FF, /* 1110000111111111 */
0xE1FF, /* 1110000111111111 */
0XE000, /* 1100000000000000 */
0XE000, /* 1100000000000000 */
0XE000, /* 1100000000000000 */
0x0000, /* 1110000000000000 */
0x0000, /* 1110000000000000 */
0x0000, /* 1110000000000000 */
0x0000, /* 1110000000000000 */
0x0000, /* 1110000000000000 */
0x0000, /* 1110000000000000 */
0x0000, /* 1110000000000000 */
0x0000, /* 1110000000000000 */

/* Mscursor Mask */
0xE100, /* 0001111000000000 */
0X1200, /* 0001001000000000 */
0X1200, /* 0001001000000000 */
0X1200, /* 0001001000000000 */
0X1200, /* 0001001000000000 */
0X13FF, /* 0001001111111111 */
0X1249, /* 0001001001001001 */
0X1249, /* 0001001001001001 */
0XF249, /* 1111001001001001 */
0X9001, /* 1001000000000001 */
0X9001, /* 1001000000000001 */
0X9001, /* 1001000000000001 */
0X8001, /* 1000000000000001 */
0X8001, /* 1000000000000001 */
0X8001, /* 1000000000000001 */
0XFFFF  /* 1111111111111111 */ },

{ /* mscursor[1] ----> Arrow Screen Mask */
0X3FFF, /* 0011111111111111 */
0X1FFF, /* 0001111111111111 */
0X0FFF, /* 0000111111111111 */
0X07FF, /* 0000011111111111 */
0X03FF, /* 0000001111111111 */
0X01FF, /* 0000000111111111 */
0X00FF, /* 0000000011111111 */
0X007F, /* 0000000001111111 */
0X003F, /* 0000000000111111 */
0X001F, /* 0000000000011111 */
0X01FF, /* 0000000111111111 */
0X10FF, /* 0001000011111111 */
0X30FF, /* 0001100001111111 */
0XF87F, /* 1111100001111111 */
0XF87F, /* 1111100001111111 */
0XFC3F, /* 1111110000111111 */

/* mscursor Mask */
0x0000, /* 0000000000000000 */
0x4000, /* 0100000000000000 */
0x6000, /* 0110000000000000 */
0x7000, /* 0111000000000000 */
0x7800, /* 0111100000000000 */
0x7c00, /* 0111110000000000 */
0x7e00, /* 0111111000000000 */
0x7f00, /* 0111111100000000 */
0x7f80, /* 0111111110000000 */
0x7fc0, /* 0111111111000000 */
0x6c00, /* 0110110000000000 */
0x4600, /* 0100011000000000 */
0x0600, /* 0000011000000000 */
0x0300, /* 0000001100000000 */
0x0300, /* 0000001100000000 */
0x0180  /* 0000000110000000 */ },

{ /*mscursor[2] ----> Inverse Arrow Screen Mask */

0xffff, /* 1111111111111111 */
0xffff, /* 1111111111111111 */
0xffff, /* 1111111111111111 */
0xffff, /* 1111111111111111 */
0xffff, /* 1111111111111111 */
0xffff, /* 1111111111111111 */
0xffff, /* 1111111111111111 */
0xffff, /* 1111111111111111 */
0xffff, /* 1111111111111111 */
0xffff, /* 1111111111111111 */
0xffff, /* 1111111111111111 */
0xffff, /* 1111111111111111 */
0xffff, /* 1111111111111111 */
0xffff, /* 1111111111111111 */
0xffff, /* 1111111111111111 */
0xffff, /* 1111111111111111 */

/*  mscursor Maks */
0x0000, /* 0000000000000000 */
0x4000, /* 0100000000000000 */
0x6000, /* 0110000000000000 */
0x7000, /* 0111000000000000 */
0x7800, /* 0111100000000000 */
0x7c00, /* 0111110000000000 */
0x7e00, /* 0111111000000000 */
0x7f00, /* 0111111100000000 */
0x7f80, /* 0111111110000000 */
0x7fc0, /* 0111111111000000 */
0x6c00, /* 0110110000000000 */
0x4600, /* 0100011000000000 */
0x0600, /* 0000011000000000 */
0x0300, /* 0000001100000000 */
0x0300, /* 0000001100000000 */
0x0180  /* 0000000110000000 */ }};
