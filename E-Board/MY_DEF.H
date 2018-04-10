/* MY_DEF.H: Prototypes & Typedef for my define */

struct MOUSE_STATE {
   int    x_position, y_position,
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
