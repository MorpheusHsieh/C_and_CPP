#include <io.h>
#include <dos.h>
#include <conio.h>
#include <ctype.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <complex.h>
#include <sys\stat.h>
#include <sys\types.h>
#include "my_def.h"

#define N         6
#define SPACE    32
#define ESC      27
#define ENTER    13
#define UP     1072
#define DOWN   1080
#define LEFT   1075
#define RIGHT  1077
#define PGUP   1073
#define PGDOWN 1081
#define TRUE      1
#define FALSE     0
#define Alt_f  1033
#define Alt_p  1025
#define Alt_d  1032
#define Alt_h  1035
#define PORTA 0x200
#define PORTB 0x201
#define PORTC 0x202
#define RESET 0x203

#define SPECIAL_COLOR RED
#define SELECT_BK_COLOR GREEN
#define SUBMENU_FR_COLOR BLACK
#define SUBMENU_BK_COLOR LIGHTGRAY
#define FUNCTION_BAR_FR_COLOR BLACK
#define FUNCTION_BAR_BK_COLOR LIGHTGRAY
#define DESCRIPTION_COLOR YELLOW+(LIGHTGRAY<<4);

int controller(void);
int keyScan(void);
void demo(void);
void description(char *);
void ET_PATH(void);
void initial(void);
void Functionbar(int, int, char *, int, int);
void menuCenter(TEXTBOX *, char *);
void popup(int, int, int, int);
long pow2(int);
void pulldown(int, int);
void reserboard(void);
void saveboard(void);
void Showboard(void);
void sub000(void);
void sub001(void);
void sub002(void);
void sub003(void);
void sub100(void);
void sub101(void);
void sub200(void);
void sub201(void);
void sub202(void);
void sub203(void);
void sub204(void);
void sub300(void);
void sub301(void);
void sub302(void);
void sub303(void);
void sub400(void);
void sub500(void);

int Handle1, Handle2;
int ms_x, ms_y;
int x1=8, x2=71, y1=5, y2=20;
int x1_tmp, y1_tmp, x2_tmp, y2_tmp;
int submenu_x1, submenu_y1, submenu_x2, submenu_y2;
char *screen_save;
char far *EGA=(char far*)0xB8000000;
char Code[4];
unsigned char ET_path[81];
unsigned char filename[81];
unsigned static int Font[128], Tnof[128];
unsigned static int LED[64][16];

union REGS regs;
struct text_info text;
struct MOUSE_STATE ms= { 0, 0, 0, 0, 0, 0 };
struct SUBMENU {
   int  number_of_item, level, name_start_addr,
	name_width, sub_width;
   char *name, *hotkey,
	**item, **description;
} submenu[N];

TEXTBOX menu  = { 20,  6, 60, 16, WHITE, CYAN, 4, 1 },
	board = {  8,  5, 71, 20, WHITE, CYAN, 0, 1 };

char function_bar[] = "/2^ð/2^File/2^Edit/2^Run/2^Project/2^Display/2^Help";
char *sub1[] = { " ^New        ",
		 " ^Open...    ",
		 " ^Save       ",
		 " S^ave as... ",
		 " ^Quit       "  },
     *sub2[] = { " C^lear  ",
		 " ^Font   "      },
     *sub3[] = { " ^Up move    ",
		 " ^Down move  ",
		 " ^Left move  ",
		 " ^Right move ",
		 " ^Slice      "  },
     *sub4[] = { " ^Open project...  ",
		 " ^Close project    ",
		 " ^Add item...      ",
		 " ^Delete item      "  },
     *sub5[] = { " ^Board    "  },
     *sub6[] = { " ^About... "  };
char *des1[] = { "New files",
		 "Open exists file",
		 "Save use default filename",
		 "Save as new filename",
		 "Exit E-Board"                  },
     *des2[] = { "Clear the board",
		 "Copy the selected",
		 "Call chinese font"             },
     *des3[] = { "It's not necessary to describe.",
		 "Go downward. Don't you know ?",
		 "Left is left, not right, up or down.",
		 "Are you crazy ?",
		 "Choose me, chose me, press ENTER to choose.....What are your waiting for ?"  },
     *des4[] = { "Load a project file and its desktop",
		 "Close the current project and reload the default project",
		 "Add an item to the project",
		 "Delete an item to the project" },
     *des5[] = { "Output Screen",
		 "Output to electric board"      },
     *des6[] = { "Show version and copyright information" };

void main(void)
{
   int choice;

   clrscr();
   if ( etexist()== -1) {
      printf("\nET chinese system is noexist.\n\7");
      exit(0);
   }
   ET_PATH();
   initial();
   mouse_reset();
   mouse_toggle(1);
   cursor_off();
   demo();
   window(1, 1, 80, 25);
   while (choice!=4) {
      ms.l_button=0, ms.m_button=0, ms.r_button=0 ;
      choice=controller();
      switch (choice)
      {
	 case   0: sub000(); break;
	 case   1: sub001(); break;
	 case   2: sub002(); break;
	 case   3: sub003(); break;
	 case 100: sub100(); break;
	 case 101: sub101(); break;
	 case 200: sub200(); break;
	 case 201: sub201(); break;
	 case 202: sub202(); break;
	 case 203: sub203(); break;
	 case 204: sub204(); break;
	 case 300: sub300(); break;
	 case 301: sub301(); break;
	 case 302: sub302(); break;
	 case 303: sub303(); break;
	 case 400: sub400(); break;
	 case 500: sub500(); break;
      }
   }
   window(1, 1, 80, 25);
   textattr(WHITE+(BLACK<<4));
   clrscr();
   mouse_toggle(0);
   cursor_on();
}

int controller(void)
{
   int level;
   int key=0, status=0, active=0, bar=0, i;
   char chr;

   while ( key+ms.l_button+ms.m_button+ms.r_button == 0 )
   {
      key=keyScan();
      if (key)
      {
	 switch (key)
	 {
	    case Alt_f: pulldown(0, 1);
			status=1;
			active=1;
			bar=0;
			description(submenu[bar].description[submenu[bar].level]);
			break;
	    case Alt_p: pulldown(1, 1);
			status=1;
			active=1;
			bar=1;
			description(submenu[bar].description[submenu[bar].level]);
			break;
	    case Alt_d: pulldown(2, 1);
			status=1;
			active=1;
			bar=2;
			description(submenu[bar].description[submenu[bar].level]);
			break;
	    case Alt_h: pulldown(3, 1);
			status=1;
			active=1;
			bar=3;
			description(submenu[bar].description[submenu[bar].level]);
			break;
	    case ESC  : if (active) {
			    puttext(submenu_x1-2, submenu_y1, submenu_x2+2, submenu_y2+1, screen_save);
			    status=0;
			    active=0;
			    description("");
			    break;
			}
			else
			   exit(0);
	    case LEFT : if (active) {
			   puttext(submenu_x1-2, submenu_y1, submenu_x2+2, submenu_y2+1, screen_save);
			   if (bar) --bar;
			   else bar=N-1;
			   pulldown(bar, status);
			   description(submenu[bar].description[submenu[bar].level]);
			}
			break;
	    case RIGHT: if (active) {
			   puttext(submenu_x1-2, submenu_y1, submenu_x2+2, submenu_y2+1, screen_save);
			   if (bar==N-1) bar=0;
			   else bar++;
			   pulldown(bar, status);
			   description(submenu[bar].description[submenu[bar].level]);
			}
			break;
	    case UP   : if (status) {
			   level=submenu[bar].level;
			   Functionbar(submenu[bar].name_start_addr,
				    level+3, submenu[bar].item[level],
				    BLACK, LIGHTGRAY);
			   if (submenu[bar].level)
			      --submenu[bar].level;
			   else
			      submenu[bar].level=submenu[bar].number_of_item-1;
			   level=submenu[bar].level;
			   Functionbar(submenu[bar].name_start_addr,
				    level+3, submenu[bar].item[level],
				    BLACK, GREEN);
			   description(submenu[bar].description[submenu[bar].level]);
			}
			break;
	    case DOWN : if (status) {
			   level=submenu[bar].level;
			   Functionbar(submenu[bar].name_start_addr,
				    level+3, submenu[bar].item[level],
				    BLACK, LIGHTGRAY);
			   if (submenu[bar].level==submenu[bar].number_of_item-1)
			      submenu[bar].level=0;
			   else
			      submenu[bar].level++;
			   level=submenu[bar].level;
			   Functionbar(submenu[bar].name_start_addr,
				    level+3, submenu[bar].item[level],
				    BLACK, GREEN);
			   description(submenu[bar].description[submenu[bar].level]);
			}
			break;
	    case ENTER: if (active) {
			   if (status)
			      return(bar*100+submenu[bar].level);
			   else {
			      puttext(submenu_x1-2, submenu_y1, submenu_x2+2, submenu_y2+1, screen_save);
			      pulldown(bar, 1);
			      status=1;
			   }
			}
			break;
	    default   : if (status) {
			   if (key<'a')
			     key+=32;
			   i=0;
			   while (submenu[bar].hotkey[i] != NULL) {
			      if (submenu[bar].hotkey[i]==key)
				 return(bar*100+i);
			      i++;
			   }
			}
	 }
      }
      mouse_state(&ms);
      if (ms.l_button) {
	 ms_x=(ms.x_position>>3)+1;
	 ms_y=(ms.y_position>>3)+1;
	 if (status) {
	    if (
	       ms_y<submenu[bar].number_of_item+3
	       && ms_y>=3
	       && ms_x>=submenu[bar].name_start_addr
	       && ms_x<submenu[bar].name_start_addr+submenu[bar].sub_width
	    )  {
	       level=submenu[bar].level;
	       Functionbar(submenu[bar].name_start_addr, level+3,
			   submenu[bar].item[level], BLACK, LIGHTGRAY);
	       submenu[bar].level=ms_y-3;
	       level=submenu[bar].level;
	       Functionbar(submenu[bar].name_start_addr, level+3,
			   submenu[bar].item[level], BLACK, GREEN);
	       description(submenu[bar].description[submenu[bar].level]);

	       puttext(submenu_x1-2, submenu_y1, submenu_x2+2, submenu_y2+1, screen_save);
	       return (bar*100+ms_y-3);
	    }
	    else {
	       puttext(submenu_x1-2, submenu_y1, submenu_x2+2, submenu_y2+1, screen_save);
	       active=0;
	       status=0;
	    }
	 }
	 else {
	    if (ms_y==1) {
	       for (i=0; i<N; i++) {
		  if (ms_x>=submenu[i].name_start_addr && ms_x<submenu[i].name_start_addr+submenu[i].name_width)
		  {
		     bar=i;
		     pulldown(i, 1);
		     description(submenu[bar].description[submenu[bar].level]);
		     status=1;
		     break;
		  }
	       }
	    }
	 }
	 if ( ms_x>=x1 && ms_x<=x2 && ms_y>=y1 && ms_y<=y2 ) {
	    mouse_toggle(0);
	    if ( (char)*(EGA+(ms_y-1)*160+(ms_x-1)*2+1)
		 == (char)(WHITE+(RED<<4)) )
	    {
	       *(EGA+(ms_y-1)*160+(ms_x-1)*2)=' ';
	       *(EGA+(ms_y-1)*160+(ms_x-1)*2+1)=(WHITE+(GREEN<<4));
	       LED[ms_x-8][ms_y-5]=1;
	    }
	    else {
	       *(EGA+(ms_y-1)*160+(ms_x-1)*2)=' ';
	       *(EGA+(ms_y-1)*160+(ms_x-1)*2+1)=(WHITE+(RED<<4));
	       LED[ms_x-8][ms_y-5]=0;
	    }
	    mouse_toggle(1);
	 }
      }
   key=0; ms.l_button=0; ms.r_button=0; ms.m_button=0;
   }
}

void demo(void)
{
   int i, j, k, ch;
   char *message;

   ch=' ';
   clscreen(&ch, WHITE, BLACK);

   ch=' ';
   window(1, 1, 80, 25);
   clscreen(&ch, WHITE, LIGHTBLUE);

   gettextinfo(&text);
   x1_tmp=text.winleft;
   y1_tmp=text.wintop;
   x2_tmp=text.winright;
   y2_tmp=text.winbottom;
   for (i=0; i<80; i++) {
      *(EGA+(y1_tmp-1)*160+i*2+1)=(BLACK+(LIGHTGRAY<<4));
      *(EGA+(y1_tmp-1)*160+i*2)=' ';
      *(EGA+(y2_tmp-1)*160+i*2+1)=(BLACK+(LIGHTGRAY<<4));
      *(EGA+(y2_tmp-1)*160+i*2)=' ';
   }
   Functionbar(1, 1, function_bar, BLACK, LIGHTGRAY);

   sub500();

   x1_tmp=(x1+x2)/2-1;
   y1_tmp=(y1+y2)/2-1;
   x2_tmp=(x1+x2)/2;
   y2_tmp=(y1+y2)/2;
   for (i=0; i<4; i++) {
      for (j=y1_tmp; j<y2_tmp; j++) {
	 for (k=x1_tmp; k<x2_tmp; k++) {
	    *(EGA+j*160+(k-1)*2)=' ';
	    *(EGA+j*160+(k-1)*2+1)=(WHITE+(CYAN<<4));
	    *(EGA+j*160+(k-1)*2)=' ';
	    *(EGA+j*160+(k-1)*2+1)=(WHITE+(CYAN<<4));
	 }
      }
      x1_tmp=(x1_tmp+x1)/2;
      y1_tmp=(y1_tmp+y1)/2;
      x2_tmp=(x2_tmp+x2)/2;
      y2_tmp=(y2_tmp+y2)/2;
      delay(50);
   }
   textbox(&board);

   for (i=0; i<40; i++) {
      for (j=y1; j<=y2; j++) {
	 *(EGA+(j-1)*160+(x1+i-1)*2+1)=(WHITE+(YELLOW<<4));
	 *(EGA+(j-1)*160+(x2-i-1)*2+1)=(WHITE+(YELLOW<<4));
      }
      delay(8);
   }
   for (i=32; i>=0; i--) {
      for (j=y1; j<=y2; j++) {
	 *(EGA+(j-1)*160+(x1+i-1)*2+1)=(WHITE+(RED<<4));
	 *(EGA+(j-1)*160+(x2-i-1)*2+1)=(WHITE+(RED<<4));
      }
      delay(8);
   }
}

void description(char *message)
{
   int i=0;
   char far *video=(char far*)(0xB8000000+24*160+2);

   while (message[i]!=NULL) {
      *(video++)=message[i++];
      *(video++)=DESCRIPTION_COLOR;
   }
   for (; i<=80; i++) {
      *(video++)=' ';
      *(video++)=DESCRIPTION_COLOR;
   }
}

void ET_PATH(void)
{
   int len;

   if (getenv("ET")==NULL)
      strcpy(ET_path, "c:\\et5");
   else
      strcpy(ET_path, getenv("ET"));

   strcat(ET_path, "\\stdfont.15");
   if ( access(ET_path, 0) != 0) {
      printf("\nC:\\ET5\STDFONT.15 is noexist.");
      printf("\nInitial: SET ET=C:\\ET5\n");
      printf("Syntax: SET ET=[d:][path]\n\7");
      exit(0);
   }
}

void Functionbar(int x, int y, char *string, int frcolor, int bkcolor)
{
   int i;
   int offset=0, counter=0, move=0;
   char temp[2];

   while ( string[counter]!=NULL ) {
      if (string[counter]=='/') {
	 counter++;
	 temp[0]=string[counter];
	 temp[1]=NULL;
	 move=atoi(temp);
	 for (i=0; i<move; i++)
	    *(EGA+(y-1)*160+((x-1)+offset+i)*2)=' ';
	 offset+=move;
      }
      else if (string[counter]=='^') {
	 counter++;
	 *(EGA+(y-1)*160+((x-1)+offset)*2+1)=(SPECIAL_COLOR+(bkcolor<<4));
	 *(EGA+(y-1)*160+((x-1)+offset)*2)=string[counter];
	 offset++;
      }
      else {
	 *(EGA+(y-1)*160+((x-1)+offset)*2+1)=(frcolor+(bkcolor<<4));
	 *(EGA+(y-1)*160+((x-1)+offset)*2)=string[counter];
	 offset++;
      }
      counter++;
   }
}

void initial(void)
{
   submenu[0].number_of_item = 5;
   submenu[0].level=0;
   submenu[0].name_start_addr=6;
   submenu[0].name_width=4;
   submenu[0].sub_width=12;
   submenu[0].name=" ^File ";
   submenu[0].hotkey="nosal";
   submenu[0].item=sub1;
   submenu[0].description=des1;

   submenu[1].number_of_item=2;
   submenu[1].level=0;
   submenu[1].name_start_addr=12;
   submenu[1].name_width=4;
   submenu[1].sub_width=8;
   submenu[1].name=" ^Edit ";
   submenu[1].hotkey="lf";
   submenu[1].item=sub2;
   submenu[1].description=des2;

   submenu[2].number_of_item=5;
   submenu[2].level=0;
   submenu[2].name_start_addr=18;
   submenu[2].name_width=3;
   submenu[2].sub_width=12;
   submenu[2].name=" ^Run ";
   submenu[2].hotkey="udlrs";
   submenu[2].item=sub3;
   submenu[2].description=des3;

   submenu[3].number_of_item=4;
   submenu[3].level=0;
   submenu[3].name_start_addr=23;
   submenu[3].name_width=7;
   submenu[3].sub_width=18;
   submenu[3].name=" ^Project ";
   submenu[3].hotkey="ocad";
   submenu[3].item=sub4;
   submenu[3].description=des4;

   submenu[4].number_of_item=1;
   submenu[4].level=0;
   submenu[4].name_start_addr=32;
   submenu[4].name_width=7;
   submenu[4].sub_width=10;
   submenu[4].name=" ^Display ";
   submenu[4].hotkey="b";
   submenu[4].item=sub5;
   submenu[4].description=des5;

   submenu[5].number_of_item=1;
   submenu[5].level=0;
   submenu[5].name_start_addr=41;
   submenu[5].name_width=4;
   submenu[5].sub_width=10;
   submenu[5].name=" ^Help ";
   submenu[5].hotkey="a";
   submenu[5].item=sub6;
   submenu[5].description=des6;

}

/* Key_scan(): Scan code of Keyboard */
int keyScan(void)
{
   char far *keyb_head=(char far*)0x41a;
   char far *keyb_tail=(char far*)0x41c;
   if (*keyb_head==*keyb_tail)
      return(0);

   int key_asc, key_scan;
   regs.h.ah=0x00;
   int86(0x16, &regs, &regs);
   key_asc=regs.h.al;
   key_scan=regs.h.ah;
   if (key_asc!=0)
      return (key_asc);
   else
      return (key_scan+1000);
}

void menuCenter(TEXTBOX *menu, char *string)
{
   int i, tab;

   tab=(menu->right - menu->left - strlen(string))>>1;
   for (i=0; i<tab; i++)
      cputs(" ");
   cputs(string);
}

void popup(int x1, int y1, int x2, int y2)
{
   int x, y;

   mouse_toggle(1);
   for (x=x1; x<=x2; x++)
      if (x<=x1 || x>=x2) {
	 *(EGA+(y1-1)*160+(x-1)*2)=' ';
	 *(EGA+(y1-1)*160+(x-1)*2+1)=(BLACK+(LIGHTGRAY<<4));
	 *(EGA+(y2-1)*160+(x-1)*2)=' ';
	 *(EGA+(y2-1)*160+(x-1)*2+1)=(BLACK+(LIGHTGRAY<<4));
      }
      else {
	 *(EGA+(y1-1)*160+(x-1)*2)='Ä';
	 *(EGA+(y1-1)*160+(x-1)*2+1)=(BLACK+(LIGHTGRAY<<4));
	 *(EGA+(y2-1)*160+(x-1)*2)='Ä';
	 *(EGA+(y2-1)*160+(x-1)*2+1)=(BLACK+(LIGHTGRAY<<4));
      }
   for (y=y1+1; y<y2; y++) {
      for (x=x1; x<=x2; x++)
	 if (x==(x1+1) || x==(x2-1)) {
	    *(EGA+(y-1)*160+(x-1)*2)='³';
	    *(EGA+(y-1)*160+(x-1)*2+1)=(BLACK+(LIGHTGRAY<<4));
	 }
	 else {
	    *(EGA+(y-1)*160+(x-1)*2)=' ';
	    *(EGA+(y-1)*160+(x-1)*2+1)=(BLACK+(LIGHTGRAY<<4));
	 }
      *(EGA+(y-1)*160+(x-1)*2+1)=(BLACK+(BLACK<<4));
      *(EGA+(y-1)*160+(x-1)*2+3)=(BLACK+(BLACK<<4));
   }
   *(EGA+(y2-1)*160+x2*2+1)=(BLACK+(BLACK<<4));
   *(EGA+(y2-1)*160+x2*2+3)=(BLACK+(BLACK<<4));

   y=y2+1;
   for (x=x1+2; x<=x2+2; x++)
      *(EGA+(y-1)*160+(x-1)*2+1)=(BLACK+(BLACK<<4));
   *(EGA+(y1-1)*160+x1*2)='Ú';
   *(EGA+(y1-1)*160+(x2-2)*2)='¿';
   *(EGA+(y2-1)*160+x1*2)='À';
   *(EGA+(y2-1)*160+(x2-2)*2)='Ù';

   mouse_toggle(0);
}

long pow2(int order)
{
   int i, sum=1;
   for (i=0; i<order; i++)
      sum*=2;
   return(sum);
}

void pulldown(int number, int status)
{
   int i, level;
   int bufsize;

   mouse_toggle(0);
   submenu_x1=submenu[number].name_start_addr;
   submenu_y1=1;
   submenu_x2=submenu_x1+submenu[number].sub_width+1;
   submenu_y2=submenu_y1+submenu[number].number_of_item+2;

   bufsize=((submenu[number].number_of_item+2)*(submenu[number].sub_width+6)*2);
   screen_save=(char*)calloc(bufsize, sizeof(int));

   gettext(submenu_x1-2, submenu_y1, submenu_x2+2, submenu_y2+1, screen_save);
   Functionbar(submenu_x1-1, submenu_y1, submenu[number].name, BLACK, GREEN);

   if (status) {
      popup(submenu_x1-2, submenu_y1+1, submenu_x2, submenu_y2);
      for (i=0; i<submenu[number].number_of_item; i++)
	 Functionbar(submenu_x1, 3+i,
		     submenu[number].item[i], BLACK, LIGHTGRAY);
      level=submenu[number].level;
      Functionbar(submenu_x1, level+3, submenu[number].item[level], BLACK, GREEN);
   }
   mouse_toggle(1);
}

void resetboard(void)
{
   outportb(RESET, 0x80);
}

void Showboard(void)
{
   int i, j;

   for (i=0; i<64; i++) {
      for (j=0; j<16; j++) {
	 if (LED[i][j]==1) {
	    *(EGA+(5+j-1)*160+(8+i-1)*2)=' ';
	    *(EGA+(5+j-1)*160+(8+i-1)*2+1)=(WHITE+(GREEN<<4));
	 }
	 else {
	    *(EGA+(5+j-1)*160+(8+i-1)*2)=' ';
	    *(EGA+(5+j-1)*160+(8+i-1)*2+1)=(WHITE+(RED<<4));
	 }
      }
   }
}


void sub000(void)
{
   unsigned char file_tmp[81];

   gettext(1, 1, 80, 25, screen_save);
   gotoxy(1, 24);
   textattr(WHITE+(LIGHTBLUE<<4));
   cprintf("New file, Filename:");
   gets(file_tmp);
   puttext(1, 1, 80, 25, screen_save);
   strcpy(filename, file_tmp);

   if ( access(strcat(file_tmp, ".pat"), 0) == -1 ) {
      if ( (Handle1=creat(file_tmp, O_RDWR)) == -1 ) {
	 perror("Error");
	 exit(0);
      }
   }
   else {
      if (access(file_tmp, 2) != 0)
	 chmod(file_tmp, S_IWRITE);
      if ( (Handle1=open(file_tmp, O_RDWR, S_IWRITE)) == -1 ) {
	 perror("Error");
	 exit(0);
      }
   }

   strcpy(file_tmp, filename);
   if ( access(strcat(file_tmp, ".dat"), 0) == -1 )   {
      if ( (Handle2=creat(file_tmp, S_IWRITE)) == -1 ) {
	 perror("Error");
	 exit(0);
      }
   }
   else {
      if ( access(file_tmp, 2) != 0)
	 chmod(file_tmp, S_IWRITE);
      else
	 if ( (Handle2=open(file_tmp, O_RDWR, S_IWRITE)) == -1 ) {
	    perror("Error");
	    exit(0);
	 }
   }
   close(Handle1);
   close(Handle2);
}


void sub001(void)
{
   int i, j, k, num;
   char ch[2];
   unsigned char file_tmp[81];

   ch[1]=NULL;
   gettext(1, 1, 80, 25, screen_save);
   gotoxy(1, 24);
   textattr(WHITE+(LIGHTBLUE<<4));
   cprintf("Filename: ");
   gets(filename);
   puttext(1, 1, 80, 25, screen_save);


   strcpy(file_tmp, filename);
   if ( access(strcat(file_tmp, ".pat"), 0) == -1 ) {
      gettext(1, 1, 80, 25, screen_save);
      gotoxy(1, 24);
      cprintf("%s noexist.\7", file_tmp);
      delay(200);
      puttext(1, 1, 80, 25, screen_save);
      return;
   }
   else {
      if ( (access(file_tmp, 2)) != 0)
	 chmod(file_tmp, S_IWRITE);
   }
   if ( (Handle1=open(file_tmp, O_RDWR)) == -1 ) {
	 perror("Error");
	 exit(0);
   }
   else {
      for (i=0; i<16; i++) {
	 for (j=0; j<64; j++) {
	    read(Handle1, ch, 1);
	    if ( ch[0]=='1' )
	       LED[j][i]=1;
	    else {
	       if ( ch[0]=='0' )
		  LED[j][i]=0;
	       else
		  read(Handle1, ch, 1);
	    }
	 }
      }
      for (i=0; i<64; i++) {
	 for (j=0; j<2; j++) {
	    num=0;
	    for (k=0; k<8; k++)
	       num+=(LED[i][j*8+k])*pow(2.0, k);
	    Font[i*2+j]=num;
	 }
      }
   }
   close(Handle1);
   Showboard();
}

void sub002(void)
{
   if (strlen(filename)==0) {
      gettext(1, 1, 80, 25, screen_save);
      gotoxy(1, 24);
      textattr(WHITE+(LIGHTBLUE<<4));
      cprintf("Filename: ");
      gets(filename);
      puttext(1, 1, 80, 25, screen_save);
   }
   saveboard();
}

void sub003(void)
{
   gettext(1, 1, 80, 25, screen_save);
   gotoxy(1, 24);
   textattr(WHITE+(LIGHTBLUE<<4));
   cprintf("Filename: ");
   gets(filename);
   puttext(1, 1, 80, 25, screen_save);
   saveboard();
}

void sub100(void)
{
   int i, j;
   for (i=0; i<64; i++) {
      for (j=0; j<16; j++) {
	 *(EGA+(5+j-1)*160+(8+i-1)*2)=' ';
	 *(EGA+(5+j-1)*160+(8+i-1)*2+1)=(WHITE+(RED<<4));
	 LED[i][j]='0';
      }
   }
}

void sub101(void)
{
    FILE *fptr;
    int i, j, k, len, num;
    long Begin_posi;
    unsigned char One_char;
    unsigned char H_byte, L_byte;
    unsigned char Cstring[9];
    unsigned int Eightbits[8]={ 128, 64, 32, 16, 8, 4, 2, 1 };

    mouse_toggle(0);
    et_chine();
    et_phonetic_mode();

    gettext(1, 1, 80, 25, screen_save);
    do {
       textattr(WHITE+(BLUE<<4));
       gotoxy(1, 24);
       cprintf("Please Key-in words: ");
       gets(Cstring);
       len=strlen(Cstring);
       Cstring[len]=NULL;
    } while(len>8);
    et_chine();
    et_english_mode();
    et_ascii();
    mouse_toggle(1);
    puttext(1, 1, 80, 25, screen_save);

    if ((fptr=fopen(ET_path, "rb"))==NULL) {
       printf("Open %s failed! ", ET_path);
       return;
    }
    for (i=0; i<len/2; i++) {
       H_byte=Cstring[i*2];
       L_byte=Cstring[i*2+1];
       Begin_posi=(font_posi(H_byte, L_byte)*30);
       fseek(fptr, Begin_posi, SEEK_SET);
       for (j=0; j<30; j++) {
	  One_char=getc(fptr);
	  Font[j]=One_char;
       }
       Font[30]=NULL;
       for (j=0; j<15; j++) {
	  for (k=0; k<8; k++) {
	     LED[k+i*16][j+1]=((Font[j*2]&Eightbits[k])>>(7-k));
	     LED[k+8+i*16][j+1]=((Font[j*2+1]&Eightbits[k])>>(7-k));
	  }
       }
   }
   fclose(fptr);
   Showboard();

   for (i=0; i<64; i++) {
      for (j=0; j<2; j++) {
	 num=0;
	 for (k=0; k<8; k++)
	    num+=(LED[i][j*8+k])*pow(2.0, k);
	 Font[i*2+j]=num;
      }
   }
}

void sub200(void)
{
   register int i, j, l, k;
   int H_byte;

   resetboard();

   for (i=0; i<128; i++) Tnof[i]=Font[i];

   for (k=0; k<16; k++) {
     delay(20);
     for (l=0; l<150; l++) {
	 for (i=0; i<4; i++) {
	    H_byte=0x0f-pow(2.0, i);
	    for (j=0; j<16; j++) {
	       outp(PORTC, (H_byte<<4)+j);
	       outp(PORTA, Tnof[i*32+j*2]);
	       outp(PORTB, Tnof[i*32+j*2+1]);
	       outp(PORTA, 0);
	       outp(PORTB, 0);
	    }
	 }
     }
	 for (i=0; i<4; i++) {
	    for (j=0; j<16; j++) {
		  Tnof[i*32+j*2]>>=1;
		  Tnof[i*32+j*2]+=(Tnof[i*32+j*2+1]&1)<<7;
		  Tnof[i*32+j*2+1]>>=1;
	    }
	 }

   }
   resetboard();
}

void sub201(void)
{
   register int i, j, l, k;
   int H_byte;

   for (i=0; i<128; i++) Tnof[i]=Font[i];

   resetboard();
   for (k=0; k<16; k++) {
     delay(20);
     for (l=0; l<150; l++) {
	 for (i=0; i<4; i++) {
	    H_byte=0x0f-pow(2.0, i);
	    for (j=0; j<16; j++) {
	       outp(PORTC, (H_byte<<4)+j);
	       outp(PORTA, Tnof[i*32+j*2]);
	       outp(PORTB, Tnof[i*32+j*2+1]);
	       outp(PORTA, 0);
	       outp(PORTB, 0);
	    }
	 }
     }
	 for (i=0; i<4; i++) {
	    for (j=0; j<16; j++) {
		  Tnof[i*32+j*2+1]<<=1;
		  Tnof[i*32+j*2+1]+=(Tnof[i*32+j*2]&128)>>7;
		  Tnof[i*32+j*2]<<=1;
	    }
	 }

   }
   resetboard();
}

void sub202(void)
{
   register int i, j, k, l;
   int H_byte;

   resetboard();
   for (k=0; k<128; k+=2) {
      for (l=0; l<80; l++) {
	 for (i=0; i<4; i++) {
	    H_byte=0x0f-pow(2.0, i);
	    for (j=0; j<16; j++) {
	       outp(PORTC, (H_byte<<4)+j);
	       outp(PORTA, Font[i*32+j*2+k]);
	       outp(PORTB, Font[i*32+j*2+1+k]);
	       outp(PORTA, 0);
	       outp(PORTB, 0);
	    }
	 }
      }
   }
   resetboard();
}

void sub203(void)
{
   register int i, j, k, l;
   int H_byte;

   resetboard();
   for (k=0; k<128; k+=2) {
      for (l=0; l<80; l++) {
	 for (i=0; i<4; i++) {
	    H_byte=0x0f-pow(2.0, i);
	    for (j=0; j<16; j++) {
	       if ((i*32+j*2-k)>=0) {
		  outp(PORTC, (H_byte<<4)+j);
		  outp(PORTA, Font[i*32+j*2-k]);
		  outp(PORTB, Font[i*32+j*2+1-k]);
		  outp(PORTA, 0);
		  outp(PORTB, 0);
	       }
	       else {
		  outp(PORTC, (H_byte<<4)+j);
		  outp(PORTA, 0);
		  outp(PORTB, 0);
	       }
	    }
	 }
      }
   }
   resetboard();
}

void sub204(void)
{
   register int i, j, k, l;
   int H_byte;

   resetboard();
   for (k=0; k<8; k++) {
      for (l=0; l<500; l++) {
	 for (i=0; i<4; i++) {
	    H_byte=0x0f-pow(2.0, i);
	    for (j=0; j<16; j++) {
	       outp(PORTC, (H_byte<<4)+j);
	       outp(PORTA, Font[i*32+j*2]>>k);
	       outp(PORTB, Font[i*32+j*2+1]<<k);
	       outp(PORTA, 0);
	       outp(PORTB, 0);
	    }
	 }
      }
   }
   resetboard();
}

void sub300(void)
{
   unsigned char file_tmp[81];

   gettext(1, 1, 80, 25, screen_save);
   gotoxy(1, 24);
   textattr(WHITE+(LIGHTBLUE<<4));
   cprintf("Make Project, Filename:");
   gets(file_tmp);
   puttext(1, 1, 80, 25, screen_save);
   strcpy(filename, file_tmp);

   if ( access(strcat(file_tmp, ".prj"), 0) == -1 ) {
      if ( (Handle1=creat(file_tmp, O_RDWR)) == -1 ) {
	 perror("Error");
	 return;
      }
   }
   else {
      if (access(file_tmp, 2) != 0)
	 chmod(file_tmp, S_IWRITE);
      if ( (Handle1=open(file_tmp, O_RDWR, S_IWRITE)) == -1 ) {
	 perror("Error");
	 return;
      }
   }

   close(Handle1);
}

void sub301(void){}

void sub302(void){}

void sub303(void){}

void sub400(void)
{
   register int i, j;
   int H_byte;

   ms.l_button=0;
   ms.m_button=0;
   ms.r_button=0;

   resetboard();
   while (ms.l_button+ms.m_button+ms.r_button==0) {
      for (i=0; i<4; i++) {
	 H_byte=0x0f-pow(2.0, i);
	 for (j=0; j<16; j++) {
	    outp(PORTC, (H_byte<<4)+j);
	    outp(PORTA, Font[i*32+j*2]);
	    outp(PORTB, Font[i*32+j*2+1]);
	    outp(PORTA, 0);
	    outp(PORTB, 0);
	 }
	 mouse_state(&ms);
      }
   }
   resetboard();
}

void sub500(void)
{
   int i, j, k;
   int bufsize;

   textattr(WHITE+(BLUE<<4));
   bufsize=((menu.right-menu.left+3)*(menu.bottom-menu.top+2)*2);
   screen_save=(char*)calloc(bufsize, sizeof(int));
   gettext(menu.left, menu.top, menu.right+2, menu.bottom+1, screen_save);

   x1_tmp=(menu.left+menu.right)/2-1;
   y1_tmp=(menu.top+menu.bottom)/2-1;
   x2_tmp=(menu.right+menu.left)/2;
   y2_tmp=(menu.bottom+menu.top)/2;
   for (i=0; i<4; i++) {
      for (j=y1_tmp; j<y2_tmp; j++) {
	 for (k=x1_tmp; k<x2_tmp; k++) {
	    *(EGA+(j-1)*160+(k-1)*2)=' ';
	    *(EGA+(j-1)*160+(k-1)*2+1)=(menu.frground+(menu.bkground<<4));
	    *(EGA+(j-1)*160+(k-1)*2)=' ';
	    *(EGA+(j-1)*160+(k-1)*2+1)=(menu.frground+(menu.bkground<<4));
	 }
      }
      x1_tmp=(x1_tmp+menu.left)/2;
      y1_tmp=(y1_tmp+menu.top)/2;
      x2_tmp=(x2_tmp+menu.right)/2;
      y2_tmp=(y2_tmp+menu.bottom)/2;
      delay(50);
   }

   textbox(&menu);

   menuCenter(&menu, "\r\n");
   menuCenter(&menu, "E-Board Utility");
   menuCenter(&menu, "\r");
   menuCenter(&menu, "Version 1.0");
   menuCenter(&menu, "\r\n");
   menuCenter(&menu, "A Product of Avatar Corporation");
   menuCenter(&menu, "\r\n");
   textattr(YELLOW+(CYAN<<4));
   menuCenter(&menu, "Avatar");
   textattr(WHITE+(BLACK<<4));
   delay(1000);
   puttext(menu.left, menu.top, menu.right+2, menu.bottom+1, screen_save);
   window(1, 1, 80, 25);
}

void saveboard(void)
{
   int i, j, k, num=0;
   char ch, bytes;
   unsigned char file_tmp[81];

   strcpy(file_tmp, filename);
   if ( access(strcat(file_tmp, ".pat"), 0) == -1 ) {
      if ( (Handle1=creat(file_tmp, O_RDWR)) == -1 ) {
	 perror("Error");
	 exit(0);
      }
   }
   else {
      if (access(file_tmp, 2) != 0)
	 chmod(file_tmp, S_IWRITE);
      if ( (Handle1=open(file_tmp, O_RDWR, S_IWRITE)) == -1 ) {
	 perror("Error");
	 exit(0);
      }
   }

   strcpy(file_tmp, filename);
   if ( access(strcat(file_tmp, ".dat"), 0) == -1 )   {
      if ( (Handle2=creat(file_tmp, S_IWRITE)) == -1 ) {
	 perror("Error");
	 exit(0);
      }
   }
   else {
      if ( access(file_tmp, 2) != 0)
	 chmod(file_tmp, S_IWRITE);
      else
	 if ( (Handle2=open(file_tmp, O_RDWR, S_IWRITE)) == -1 ) {
	    perror("Error");
	    exit(0);
	 }
   }
   for (i=0; i<16; i++) {
      for (j=0; j<64; j++)
	 if (*(EGA+(5+i-1)*160+(8+j-1)*2+1)==(WHITE+(GREEN<<4)))
	    LED[j][i]==1;
	 else
	    if (*(EGA+(5+i-1)*160+(8+j-1)*2+1)==(WHITE+(RED<<4)))
	       LED[j][i]==0;
   }
   for (i=0; i<16; i++) {
      for (j=0; j<64; j++ ) {
	 if (LED[j][i]==1)
	    write(Handle1, "1", 1);
	 else
	    write(Handle1, "0", 1);
      }
      write(Handle1, "\n", 1);
   }

   for (i=0; i<64; i++)  {
      for (j=0; j<2;j++)   {
	 num=0;
	 for (k=0; k<8; k++) {
	    num+=LED[i][j*8+k]*pow2(k);
	 }
	 itoa(num, Code, 16);
	 while (strlen(Code)<3)	 {
	    if (strlen(Code)>1)
	       strrev(Code);
	    if (strlen(Code)<3);
	       strcat(Code, "0");
	    strrev(Code);
	 }
	 Code[4]=NULL;
	 strupr(Code);
	 write(Handle2, Code, 3);
	 if (i%4==3 && j==1)
	    write(Handle2, "H\n", 2);
	 else
	    write(Handle2, "H,", 2);
      }
   }
   close(Handle1);
   close(Handle2);
}