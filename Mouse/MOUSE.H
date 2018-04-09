struct MOUSE_STATE {
   int x_position, y_position,
      l_button, r_button, m_button,
      button_count;
};

int mouse_button_num(void);
void mouse_toggle(int);
void move_mouse(int, int);
void mouse_range(int, int, int, int);
void mouse_state(struct MOUSE_STATE *);
void mouse_release(struct MOUSE_STATE *);
