#include "../passman.h"

win_t* menu_name_win_get(menu_t* menu, const char* win_name)
{
  for(int index = 0; index < menu->win_count; index++)
  {
    win_t* win = menu->wins[index];

    if(strcmp(win->name, win_name) == 0) return win;
  }
  return NULL;
}

void menu_win_add(menu_t* menu, win_t* win)
{
  menu->wins = realloc(menu->wins, sizeof(menu_t*) * (menu->win_count + 1));

  win->menu = menu;

  menu->wins[menu->win_count++] = win;
}

win_t* menu_active_win_get(menu_t* menu)
{
  return wins_active_win_get(menu->wins, menu->win_count);
}

int menu_win_input_resize(menu_t* menu, const char* win_name, int x, int y, int w)
{
  win_t* win = menu_name_win_get(menu, win_name);

  if(win == NULL) return 1;

  if(win->type != WIN_INPUT) return 2;

  win_input_resize((win_input_t*) win, x, y, w);
  
  return 0;
}

int menu_win_list_resize(menu_t* menu, const char* win_name, int x, int y, int w, int h)
{
  win_t* win = menu_name_win_get(menu, win_name);

  if(win == NULL) return 1;

  if(win->type != WIN_LIST) return 2;

  win_list_resize((win_list_t*) win, x, y, w, h);
  
  return 0;
}

int menu_win_confirm_resize(menu_t* menu, const char* win_name, int x, int y, int w)
{
  win_t* win = menu_name_win_get(menu, win_name);

  if(win == NULL) return 1;

  if(win->type != WIN_CONFIRM) return 2;

  win_confirm_resize((win_confirm_t*) win, x, y, w);
  
  return 0;
}

void menu_win_input_create(menu_t* menu, char* name, int x, int y, int w, char* buffer, size_t size, char* title, bool secret, bool active, key_handler_t* key_handler)
{
  win_input_t* win = win_input_create(name, x, y, w, buffer, size, title, secret, active, key_handler);

  menu_win_add(menu, (win_t*) win);
}

void menu_win_confirm_create(menu_t* menu, char* name, int x, int y, int w, char* prompt, char* text_yes, char* text_no, bool active, key_handler_t* key_handler)
{
  win_confirm_t* win = win_confirm_create(name, x, y, w, prompt, text_yes, text_no, active, key_handler);

  menu_win_add(menu, (win_t*) win);
}

void menu_win_list_create(menu_t* menu, char* name, int x, int y, int w, int h, char** items, int count, bool active, key_handler_t* key_handler)
{
  win_list_t* win = win_list_create(name, x, y, w, h, items, count, active, key_handler);

  menu_win_add(menu, (win_t*) win);
}

void menu_win_tab(menu_t* menu, bool reverse)
{
  int index = wins_next_active_win_index(menu->wins, menu->win_count);

  if(index >= 0 && index < menu->win_count)
  {
    wins_rotate(menu->wins, menu->win_count, index);
  }
}
