#ifndef P_MENU_PSW_H
#define P_MENU_PSW_H

typedef struct
{
  menu_head_t head;
} menu_psw_t;

extern menu_psw_t* menu_psw_create(int xmax, int ymax);

extern void        menu_psw_resize(men_psw_t* menu, int xmax, int ymax);

extern void        menu_psw_free(men_psw_t* menu);

#endif // P_MENU_PSW_H
