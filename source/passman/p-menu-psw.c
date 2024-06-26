#include "../passman.h"

void menu_psw_resize(menu_psw_t* menu, int xmax, int ymax)
{
  int x = xmax / 2;
  int y = ymax / 2;

  menu_win_input_resize((menu_t*) menu, "psw", x, y, xmax - 10);
}

menu_psw_t* menu_psw_create(char* name, int xmax, int ymax)
{
  menu_psw_t* menu = malloc(sizeof(menu_psw_t));

  menu->head = menu_head_create(MENU_PSW, name, NULL);

  int x = xmax / 2;
  int y = ymax / 2;

  menu_win_input_create((menu_t*) menu, "psw", true, true,
    x, y, xmax - 10, NULL, 0, NULL, false, win_input_event);

  return menu;
}

void menu_psw_free(menu_psw_t* menu)
{
  if(menu == NULL) return;

  menu_head_free(menu->head);

  free(menu);
}
