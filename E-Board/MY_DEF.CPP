#include <dos.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define VIDEO 0x10
#define MOUSE 0x33
#define CUT '\\'

char far *EGA=(char far*)0xB8000000;

union REGS regs;

struct CURSOR_POSITION {
   int  cursor_x, cursor_y;
};
struct MOUSE_STATE {
   int x_position, y_position,
       l_button, r_button, m_button,
       button_count;
};

typedef struct {
   int    left, top, right, bottom,
	  frground, bkground,
	  style, shadow;
} TEXTBOX;

int etexist(void);
int mouse_reset(void);
int mouse_button_num(void);
long font_posi(int, int);
void et_quit(void);
void et_ascii(void);
void et_chine(void);
void et_toggle(void);
void et_english_mode(void);
void et_phonetic_mode(void);
void clscreen(int *, int, int);
void cursor_on(void);
void cursor_off(void);
void cursor_posi(struct CURSOR_POSITION *);
void cursor_move(int, int);
void mouse_toggle(int);
void move_mouse(int, int);
void mouse_range(int, int, int, int);
void mouse_state(struct MOUSE_STATE *);
void mouse_release(struct MOUSE_STATE *);
void setvideomode(int);
void textbox(TEXTBOX *);

void clscreen(int *ch, int foreground, int background)
{
   int addr, length=4000;

   for (addr=0; addr<length; addr+=2) {
      *(EGA+addr+1)=(foreground+(background<<4));
      *(EGA+addr)=*ch;
   }
}

void cursor_on(void)
{
   regs.h.ah=0x01;
   regs.h.ch=0x0d;
   regs.h.cl=0x0e;
   int86(VIDEO, &regs, &regs);
}

void cursor_off(void)
{
   regs.h.ah=0x01;
   regs.h.ch=0x20;
   regs.h.cl=regs.h.cl&0x9f;
   int86(VIDEO, &regs, &regs);
}

void cursor_posi(struct CURSOR_POSITION *cr)
{
   regs.h.ah=0x03;
   int86(VIDEO, &regs, &regs);
   cr->cursor_x=regs.h.dl;
   cr->cursor_y=regs.h.dh;
}

void cursor_move(int cursor_x, int cursor_y)
{
   regs.h.ah=0x02;
   regs.h.dl=cursor_x;
   regs.h.dh=cursor_y;
   int86(VIDEO, &regs, &regs);
}

int etexist(void)
{
    clrscr();
    unsigned int ax;
    regs.h.ah=0x91;
    regs.h.al=0x00;
    int86(VIDEO, &regs, &regs);
    ax=regs.x.ax;
    if (ax==0x9100)
       return(-1);
    else
       return(0);
}

void et_ascii(void)
{
   regs.h.ah=0x80;
   regs.h.al='A';
   int86(VIDEO, &regs, &regs);
}

void et_chine(void)
{
   regs.h.ah=0x80;
   regs.h.al='c';
   int86(VIDEO, &regs, &regs);
}

void et_english_mode(void)
{
   regs.h.ah=0x80;
   regs.h.al='n';
   int86(VIDEO, &regs, &regs);
}

void et_phonetic_mode(void)
{
   regs.h.ah=0x80;
   regs.h.al='3';
   int86(VIDEO, &regs, &regs);
}

void et_toggle(void)
{
   regs.h.ah=0x80;
   regs.h.al='a';
   int86(VIDEO, &regs, &regs);
}

void et_quit(void)
{
   regs.h.ah=0x80;
   regs.h.al='q';
   int86(VIDEO, &regs, &regs);
}

long font_posi(int H_byte, int L_byte)
{
   int i;
   int L_byte_offset;
   long Seri_no;
   unsigned int Int_code;

   Int_code=H_byte*256+L_byte;

   L_byte_offset=L_byte-0x40+1;
   if (L_byte_offset>63) L_byte_offset-=34;

   Seri_no=(H_byte-0xA1)*157+L_byte_offset;

   if (Int_code>=0xA440)
      Seri_no=Seri_no-30;
   if (Int_code>=0xC940)
      Seri_no-=408;
   Seri_no-=441;

   return(Seri_no-1);
}

void textbox(TEXTBOX *menu)
{
   int x, y, temp;
   char left_top, right_top, left_bottom, right_bottom, vertical, horizontal;

   if (menu->left>menu->right)
      { temp=menu->left; menu->left=menu->right; menu->right=temp; }
   if (menu->top>menu->bottom)
      { temp=menu->top; menu->top=menu->bottom; menu->bottom=temp; }
   textattr(menu->frground+(menu->bkground<<4));
      /* textattr sets text attributes for text-window functions*/

   for (x=menu->top-1; x<menu->bottom; x++) {
      for (y=menu->left-1; y<menu->right; y++) {
	 *(EGA+x*160+y*2)=' ';
	 *(EGA+x*160+y*2+1)=(menu->frground+(menu->bkground<<4));
	 *(EGA+x*160+y*2)=' ';
	 *(EGA+x*160+y*2+1)=(menu->frground+(menu->bkground<<4));
      }
   }

   switch (menu->style)
   {
      case 1 :                                   /* 1 = signal */
	 left_top='Ú'; right_top ='¿'; left_bottom='À'; right_bottom='Ù';
	 vertical='³'; horizontal='Ä';
	 break;
      case 2 :      /* 2 = double Horizontal & signal Vertical */
	 left_top='Õ'; right_top ='¸'; left_bottom='Ô'; right_bottom='¾';
	 vertical='³'; horizontal='Í';
	 break;
      case 3 :      /* 3 = signal Horizontal & double Vertical */
	 left_top='Ö'; right_top ='·'; left_bottom='Ó'; right_bottom='½';
	 vertical='º'; horizontal='Ä';
	 break;
      case 4 :                                   /* 4 = double */
	 left_top='É'; right_top ='»'; left_bottom='È'; right_bottom='¼';
	 vertical='º'; horizontal='Í';
	 break;
      default:
	 left_top=' '; right_top =' '; left_bottom=' '; right_bottom=' ';
	 vertical=' '; horizontal=' ';
   }
   for (x=menu->left-1; x<menu->right; x++)                    /* Draw Horizontal Line   */
      {
      *(EGA+((menu->top-1)*160)+x*2)=horizontal;
      *(EGA+((menu->bottom-1)*160)+x*2)=horizontal;
      }
   for (y=menu->top-1; y<menu->bottom; y++)                       /* Draw Vertical Line     */
      {
      *(EGA+y*160+(menu->left-1)*2)=vertical;
      *(EGA+y*160+(menu->right-1)*2)=vertical;
      }
   *(EGA+(menu->top-1)*160+(menu->left-1)*2)=left_top ;        /* Draw Left_Top          */
   *(EGA+(menu->top-1)*160+(menu->right-1)*2)=right_top;        /* Draw Right_Top         */
   *(EGA+(menu->bottom-1)*160+(menu->left-1)*2)=left_bottom ;  /* Draw Left_Bottom           */
   *(EGA+(menu->bottom-1)*160+(menu->right-1)*2)=right_bottom;  /* Draw right_Bottom      */

   if (menu->shadow==1) {
      for (x=menu->top; x<menu->bottom+1; x++)
	 for (y=menu->right; y<menu->right+2; y++) {
	    *(EGA+x*160+y*2)=' ';
	    *(EGA+x*160+y*2+1)=(BLACK+(BLACK<<4));
	 }
      for (y=menu->left; y<menu->right+1; y++) {
	 *(EGA+menu->bottom*160+y*2)=' ';
	 *(EGA+menu->bottom*160+y*2+1)=(BLACK+(BLACK<<4));
      }
   }
   window(menu->left+1, menu->top+1, menu->right-1, menu->bottom-1);
}

int mouse_reset(void)
{
   regs.x.ax=0;
   int86(MOUSE, &regs, &regs);
   return(regs.x.ax);
}

int mouse_button_num(void)
{
   regs.x.ax=0;
   int86(MOUSE, &regs, &regs);
   return(regs.x.bx);
}

void mouse_toggle(int t)
{
   regs.x.ax=2-t;
   int86(MOUSE, &regs, &regs);
}

void mouse_state(struct MOUSE_STATE *ms)
{
   regs.x.ax=3;
   int86(MOUSE, &regs, &regs);
   if (regs.x.bx) {
      ms->l_button=regs.x.bx&1;
      ms->r_button=(regs.x.bx&2)>>1;
      ms->m_button=(regs.x.bx&4)>>2;
      ms->x_position=regs.x.cx;
      ms->y_position=regs.x.dx;
      regs.x.ax=3;
      while (regs.x.bx)
	 int86(MOUSE, &regs, &regs);
   }
   else {
      ms->l_button=0;
      ms->r_button=0;
      ms->m_button=0;
      ms->x_position=0;
      ms->y_position=0;
   }
}

void move_mouse(int x, int y)
{
   regs.x.ax=4;
   regs.x.cx=x;
   regs.x.dx=y;
   int86(MOUSE, &regs, &regs);
}

void mouse_release(struct MOUSE_STATE *ms)
{
   regs.x.ax=5;
   regs.x.bx=7;
   int86(MOUSE, &regs, &regs);
   ms->l_button=regs.x.ax&1;
   ms->r_button=(regs.x.ax&2)>>1;
   ms->m_button=(regs.x.ax&4)>>2;
   ms->button_count=regs.x.bx;
   ms->x_position=regs.x.cx;
   ms->y_position=regs.x.dx;
}

void mouse_range(int x1, int y1, int x2, int y2) {
   regs.x.ax=7;
   regs.x.cx=x1;
   regs.x.dx=x2;
   int86(MOUSE, &regs, &regs);
   regs.x.ax=8;
   regs.x.cx=y1;
   regs.x.dx=y2;
   int86(MOUSE, &regs, &regs);
}

void setvideomode(int mode)
{
   regs.h.ah=0x00;
   regs.h.al=mode;
   int86(VIDEO, &regs, &regs);
}
