#include "../passman.h"

menu_head_t menu_head_create(menu_type_t type, char* name)
{
  menu_head_t menu;

  menu.type = type;
  menu.name = name;

  menu.win_count = 0;
  menu.win_index = -1;

  menu.pop_count = 0;
  menu.pop_index = -1;

  return menu;
}

void menu_refresh(menu_t* menu)
{
  for(int index = 0; index < menu->win_count; index++)
  {
    if(index == menu->win_index) continue;

    win_refresh(menu->wins[index]);
  }

  win_refresh(menu->wins[menu->win_index]);
}

void menu_resize(menu_t* menu, int xmax, int ymax)
{
  switch(menu->type)
  {
    case MENU_DB:
      menu_db_resize((menu_db_t*) menu, xmax, ymax);
      break;

    case MENU_DBS:
      menu_dbs_resize((menu_dbs_t*) menu, xmax, ymax);
      break;

    case MENU_PSW:
      menu_psw_resize((menu_psw_t*) menu, xmax, ymax);
      break;
  }
}

void menus_resize(menu_t* menus, int count, int xmax, int ymax)
{
  for(int index = 0; index < count; index++)
  {
    menu_resize(menus[index], xmax, ymax);
  }
}

void menu_free(menu_t* menu)
{
  switch(menu->type)
  {
    case MENU_DB:
      menu_db_free((menu_db_t*) menu);
      break;

    case MENU_DBS:
      menu_dbs_free((menu_dbs_t*) menu);
      break;

    case MENU_PSW:
      menu_psw_free((menu_psw_t*) menu);
      break;
  }
}

void menus_free(menu_t** menus, int count)
{
  for(int index = 0; index < count; index++)
  {
    menu_free(menus[index], count);
  }
}

void menu_head_free(menu_head_t* menu)
{
  if(menu == NULL) return;

  wins_free(menu->wins, menu->win_count);

  free(menu->wins);

  wins_free(menu->pops, menu->pop_count);

  free(menu->pops);
}

win_t* menu_win_get(menu_t* menu, const char* win_name)
{
  for(int index = 0; index < menu->win_count; index++)
  {
    win_t* win = menu->wins[index];

    if(strcmp(win->name, win_name) == 0) return win;
  }
  return NULL;
}

int menu_win_input_resize(menu_t* menu, const char* win_name, int x, int y, int w)
{
  win_t* win = menu_win_get(menu, win_name);

  if(win == NULL) return 1;

  if(win->type != WIN_INPUT) return 2;

  win_input_resize(win, x, y, w);
  
  return 0;
}

void menu_win_add(menu_t* menu, win_t* win)
{
  menu->wins = realloc(sizeof(menu_t*) * (menu->win_count + 1));

  menu->wins[menu->win_count++] = win;
}

int menu_win_input_create(menu_t* menu, char* name, int x, int y, int w, char* buffer, size_t size, char* title, bool secret, bool active)
{
  win_input_t* win = win_input_create(name, x, y, w, buffer, size, title, secret, active);

  menu_win_add(menu, win);
}

void menu_win_tab(menu_t* menu, bool reverse)
{
  for(int index = 0; index < menu->win_count; index++)
  {
    int add_index = (reverse)
      ? (menu->win_index + index * (menu->win_count - 1))
      : (menu->win_index + index + 1);

    int win_index = add_index % menu->win_count;

    if(menu->wins[win_index]->active)
    {
      menu->win_index = win_index;
    }
  }
}

void menu_head_key_handler(menu_head_t* menu, int key)
{
  switch(key)
  {
    case KEY_TAB:
      menu_win_tab(menu, false);
      break;
    
    case KEY_BTAB:
      menu_win_tab(menu, true);
      break;
  }
}

void menu_key_handler(menu_t* menu, screen_t* screen, int key)
{
  win_t* pop = menu_pop_get(menu);
  win_t* win = menu_win_get(menu);

  if(pop != NULL && pop->active && pop->key_handler)
  {
    pop->key_handler(screen, pop, key);
  }
  else if(win != NULL && win->active && win->key_handler)
  {
    menu_head_key_handler(menu, key);

    win->key_handler(screen, win, key);
  }
}
