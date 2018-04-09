#include <mouse.h>
#define MOUSE 0x33

int mouse_button_num(void);
void mouse_toggle(int);
void move_mouse(int, int);
void mouse_range(int, int, int, int);
void mouse_state(struct MOUSE_STATE *);
void mouse_release(struct MOUSE_STATE *);

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
   ms->l_button=regs.x.bx&1;
   ms->r_button=(regs.x.bx&2)>>1;
   ms->m_button=(regs.x.bx&4)>>2;
   ms->x_position=regs.x.cx;
   ms->y_position=regs.x.dx;
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














