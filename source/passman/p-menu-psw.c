#include "../passman.h"

void menu_psw_resize(menu_psw_t* menu, int xmax, int ymax)
{
  int x = xmax / 2;
  int y = ymax / 2;

  win_input_resize(menu->wins[0], x, y, xmax - 10);
}

menu_psw_t* menu_psw_create(int xmax, int ymax)
{
  menu_psw_t* menu = malloc(sizeof(menu_psw_t));

  menu->type = MENU_PSW;

  menu->win_count = 1;

  int x = xmax / 2;
  int y = ymax / 2;

  menu->wins[0] = win_input_create(x, y, xmax - 10, password, sizeof(password), NULL, false, true);
}

void menu_psw_free(menu_psw_t* menu)
{
  if(menu == NULL) return;

  menu_head_free(menu);

  free(menu);
}
