#include <stdio.h>
#include <dos.h>
#include <graphics.h>
#include <process.h>

#define ON   1
#define OFF  0

#define selcolr   14
#define unselcolr  8

struct MOUSE_STATUE { int X,Y   ;
                      int L,R,C ; //Left buttom,Right buttom,Center buttom
                    };
typedef struct MOUSE_STATUE Mouse_Stat;


union REGS regs;
int GraphDriver;
int GraphMode;
Mouse_Stat press ;
int dot_ary[16][16];
int exitX0,exitX1,exitY0,exitY1;

void gotoxy(int,int);
void disp_cursor(void);
void Initialize(void);
void drawdot(void);
void tog_dot(int,int,int);
void out_ary(int[16][16] );
void ary2file( int[16][16] );

int initmouse(void);
void mouse_cursor(int sw);
Mouse_Stat cur_stat(void);
Mouse_Stat wait_press(void);
void set_mouse_proc(int,int,int);


main(int argc,char *argv[])
{
  int x,y;
  int Maxcolors;
  Initialize();
  if (initmouse()==0)
  { printf("\nMouse not found\n");
    exit(1);
  }
  drawdot();
  mouse_cursor(ON);
  //set_mouse_proc(1,FP_SEG(disp_cursor),FP_OFF(disp_cursor));
  do
  {
    disp_cursor();
    press=cur_stat();
    x=press.X;
    y=press.Y;
    if(press.L==1)
    {                                       
      //press=wait_press();
      mouse_cursor(OFF);
      tog_dot(x,y,ON);
      mouse_cursor(ON);
    }
    else if (press.R==1)
    {
      //press=wait_press();
      mouse_cursor(OFF);
      tog_dot(x,y,OFF);
      mouse_cursor(ON);
    };

  } while(!((press.R|press.L)==1 && press.X>exitX0 &&press.X<exitX1 &&press.Y>exitY0 &&press.Y<exitY1));
  initmouse();
  out_ary(dot_ary);
  ary2file(dot_ary);
  closegraph();
  return 0;
};

void tog_dot(int xx,int yy,int mode)
{
  int colr;
  colr=getpixel(xx,yy);
  if (colr==unselcolr || colr==selcolr)
  {
     switch(mode)
     { case 1 : if (colr==selcolr) return;
                else
                {  setfillstyle(1,selcolr);
                   floodfill(xx,yy,15);
                };
                break;
       case 0 : if (colr==unselcolr) return;
                else
                {  setfillstyle(1,unselcolr);
                   floodfill(xx,yy,15);
                };
                break;
     }; // end switch
  };// end if
};

void gotoxy(int x,int y)
{
  regs.h.dl=x;
  regs.h.dh=y;
  regs.h.bh=0;
  regs.h.ah=2;
  int86(0x10,&regs,&regs);
}

void Initialize(void)
{
  int ErrorCode;
  int maxcolor;
  GraphDriver = DETECT;
  initgraph(&GraphDriver,&GraphMode,"");
  ErrorCode=graphresult();
  if (ErrorCode != 0)
  {
    printf("\nGraph mode error !");
    exit(1);
  };
  setcolor(15);
};

void drawdot(void)
{
 int i,j;
 int x0=100,y0=50;
 int x,y;
 int maxx,maxy;
 x=x0;y=y0;
 setfillstyle(1,unselcolr);
 for (i = 1;i <= 16;i++)
 {
   y=y0;
   for(j=1 ;j<=16;j++)
   {
     circle(x,y,7);
     floodfill(x,y,15);
     y=y+17;
   };
   x=x+17;
 };

 setcolor(GREEN);
 rectangle(x0-17,y0-17,x,y);

 setcolor(YELLOW);
 maxx=getmaxx(); maxy=getmaxy();
 exitX0=170; exitY0=350;
 exitX1=exitX0+80; exitY1=exitY0+40;
 rectangle(exitX0,exitY0,exitX1,exitY1);
 setfillstyle(1,BROWN);
 floodfill(exitX0+1,exitY0+1,YELLOW);

 setusercharsize(2,1,3,2);
 settextstyle(TRIPLEX_FONT, HORIZ_DIR, 2);
 setcolor(WHITE);
 outtextxy(exitX0+20,exitY0+8,"Exit");

};
// reset mouse
// return  0 , if mouse driver not found .
// return the buttoms of mouse ,if mouse driver found .
int initmouse(void)
{
  union REGS reg;
  reg.x.ax=0;
  int86(0x33,&reg,&reg);
  if (reg.x.ax == 0 ) return 0;
  else return reg.x.bx ;
};

// mouse_cursor( sw )
// if sw==0 , hidden mouse cursor ; cursor off .
// if sw==1 , display mouse cursor ; cursor on .
// Note : this function will reset the mouse_cursor disable area .
void mouse_cursor(int sw)
{
  union REGS reg;
  if (sw==0)
     reg.x.ax=2;
  else
     reg.x.ax=1;
  int86(0x33,&reg,&reg);
  return;
};

Mouse_Stat cur_stat(void)
{
  union REGS reg;
  Mouse_Stat st;
  reg.x.ax=3;
  int86(0x33,&reg,&reg);
  st.L=0; st.R=0; st.C=0;
  if (reg.x.bx & 0x1) st.L=1;
  if (reg.x.bx & 0x2) st.R=1;
  if (reg.x.bx & 0x4) st.C=1;
  st.X=reg.x.cx;  st.Y=reg.x.dx;
  return st;
};

Mouse_Stat  wait_press(void)
{ Mouse_Stat prs,rel;
  prs=cur_stat();
  while (prs.L==0 && prs.R==0 && prs.C==0)  prs=cur_stat();
  rel=prs;
  while (rel.L==1 || rel.R==1 || rel.C==1)  rel=cur_stat();
  return prs;
};


void out_ary(int ary[16][16])
{
 int i,j;
 int x=100,y=50;
 int colr;

 for (i = 1;i <= 16;i++)
 {
   for(j=1 ;j<=16;j++)
   {
     colr=getpixel(x,y);
     if (colr==selcolr) ary[j-1][i-1]=1;
     else ary[j-1][i-1]=0;
     y=y+17;
   };
   y=50;
   x=x+17;
 };
};

void ary2file( int fntary[16][16] )
{   int bt;
    int i,j,k,m;
    char fname[80];
    FILE *fp ;
    gotoxy(5,26);
    printf("Input the output data file name : >");
    gets(fname);
    fp = fopen(fname, "w+");

    fprintf(fp,"\n  ; ");
    for (m=0;m<16;m++)
    {
      if ((m+1)%4==1) fprintf(fp,"\n  db ");
      else fprintf(fp,",");
      bt=0;
      for(k=7;k>=0;k--)
        { bt=bt<<1;
          bt=bt|fntary[k][m];
        };
      fprintf(fp," %02Xh,",bt);
      bt=0;
      for(k=15;k>=8 ;k--)
        { bt=bt<<1;
          bt=bt|fntary[k][m];
        };
      fprintf(fp," %02Xh",bt);
    };
    fclose(fp);
};

void disp_cursor(void)
{ Mouse_Stat ms;
  ms=cur_stat();
  gotoxy(0,0);
  printf("x=%3d y=%3d",ms.X,ms.Y);
  return;
};

void set_mouse_proc(int mask,int seg,int ofst)
{
  union REGS reg;
  struct SREGS sreg;
  reg.x.ax=0xC;
  reg.x.cx=mask;
  sreg.es=seg;
  reg.x.dx=ofst;
  int86x(0x33,&reg,&reg,&sreg);
};
