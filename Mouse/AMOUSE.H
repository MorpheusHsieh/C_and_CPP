/* Amouse.h :define date structure & function prototype */
#ifndef __MOUSE_DEF
#define __MOUSE_DEF

#ifdef __cplusplus

extern "C" {

#endif

char cdecl      msreset(void);
char cdecl      msbutton(void);
void cdecl      msshow(void);
void cdecl      mshhide(void);
char cdecl      msleftpressed(void);
char cdecl      msrightpressed(void);
char cdecl      msmidpressed(void);
void cdecl      msgetxy(int*, int*);
void cdecl      mstgetxy(int*, int*);
void cdecl      msgotoxy(int, int);
void cdecl      mstgotoxy(int, int);
unsigned cdecl  mspressedstatus(int, int*, int*);
unsigned cdecl  msreleasestatus(int, int*, int*);
void cdecl      mssetxlim(int, int);
void cdecl      mssetylim(int, int);
void cdecl      mssetcursor(int, int, unsigned int far*);
void cdecl      mstsetcursor(int, int, int);
int  cdecl      msmotion(int*, int*);
void cdecl      mssetevent(unsigned int, void far*);
void cdecl      mslighton(void);
void cdecl      mslightoff(void);
void cdecl      mssetratio(int, int);
void cdecl      msblockhide(int, int, int, int);
void cdecl      mssetspeed(int);
void far* cdecl msswapevent(unsigned int*, void far*);
int  cdecl      msgetstorage(void);
void cdecl      msgetstate(void*);
void cdecl      msputstate(void*);
void cdecl      mssetallevent(unsigned int, void far*);
void far* cdecl msgetallevent(unsigned int*);
char cdecl      msinit(void);

extern unsigned mscursor[3][32];

#endif



