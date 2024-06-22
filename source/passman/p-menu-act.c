#include "../passman.h"

static int menu_act_event(menu_head_t* menu_head, int key)
{
  if(!menu_head || menu_head->type != MENU_ACT) return 0;

  menu_act_t* menu = (menu_act_t*) menu_head;

  switch(key)
  {
    case KEY_CTRLS:
      // menu_win_focus_set((menu_t*) menu, "save");
      return 1;

    case KEY_CTRLZ:
      screen_menu_focus_set(menu->head.screen, "db");
      return 2;

    default:
      return 0;
  }
}

menu_act_t* menu_act_create(char* name, int xmax, int ymax)
{
  menu_act_t* menu = malloc(sizeof(menu_act_t));

  menu->head = menu_head_create(MENU_ACT, name, menu_act_event);

  menu->accnt = NULL;

  int x = xmax / 2;
  int w = xmax - 12;

  menu_win_input_create((menu_t*) menu, "account", true, true, x, 5, w, NULL, 0, "Account", false, win_input_event);
  
  menu_win_input_create((menu_t*) menu, "website", true, true, x, 9, w, NULL, 0, "Website", false, win_input_event);
  
  menu_win_input_create((menu_t*) menu, "name", true, true, x, 13, w, NULL, 0, "Name", false, win_input_event);
  
  menu_win_input_create((menu_t*) menu, "email", true, true, x, 17, w, NULL, 0, "Email", false, win_input_event);
  
  menu_win_input_create((menu_t*) menu, "password", true, true, x, 21, w, NULL, 0, "Password", true, win_input_event);
  
  return menu;
}

void menu_act_resize(menu_act_t* menu, int xmax, int ymax)
{
  int x = xmax / 2;
  int w = xmax - 12;

  menu_win_input_resize((menu_t*) menu, "account",  x, 5,  w);

  menu_win_input_resize((menu_t*) menu, "website",  x, 9,  w);

  menu_win_input_resize((menu_t*) menu, "name",     x, 13, w);

  menu_win_input_resize((menu_t*) menu, "email",    x, 17, w);

  menu_win_input_resize((menu_t*) menu, "password", x, 21, w);
}

void menu_act_accnt_fill(menu_act_t* menu)
{
  if(!menu || !menu->accnt) return;

  accnt_t* accnt = menu->accnt;

  menu_win_input_buffer_set((menu_t*) menu, "account",  accnt->account,  sizeof(accnt->account));

  menu_win_input_buffer_set((menu_t*) menu, "website",  accnt->website,  sizeof(accnt->website));

  menu_win_input_buffer_set((menu_t*) menu, "name",     accnt->name,     sizeof(accnt->name));

  menu_win_input_buffer_set((menu_t*) menu, "email",    accnt->email,    sizeof(accnt->email));

  menu_win_input_buffer_set((menu_t*) menu, "password", accnt->password, sizeof(accnt->password));
}

void menu_act_free(menu_act_t* menu)
{
  if(!menu) return;

  menu_head_free(menu->head);

  free(menu);
}
