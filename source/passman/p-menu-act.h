#ifndef P_MENU_ACT_H
#define P_MENU_ACT_H

typedef struct
{
  menu_head_t head;
  accnt_t*    accnt;
} menu_act_t;

extern menu_act_t* menu_act_create(char* name, int xmax, int ymax);

extern void        menu_act_free(menu_act_t* menu);

extern void        menu_act_resize(menu_act_t* menu, int xmax, int ymax);


extern int menu_act_accnt_fill(menu_act_t* menu, accnt_t* accnt);

#endif // P_MENU_ACT_H
