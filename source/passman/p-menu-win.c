#include "../passman.h"

win_t* menu_name_win_get(menu_t* menu, char* win_name)
{
  return wins_name_win_get(menu->wins, menu->win_count, win_name);
}

win_list_t* menu_name_win_list_get(menu_t* menu, char* win_name)
{
  return wins_name_win_list_get(menu->wins, menu->win_count, win_name);
}

win_confirm_t* menu_name_win_confirm_get(menu_t* menu, char* win_name)
{
  return wins_name_win_confirm_get(menu->wins, menu->win_count, win_name);
}

win_text_t* menu_name_win_text_get(menu_t* menu, char* win_name)
{
  return wins_name_win_text_get(menu->wins, menu->win_count, win_name);
}

win_input_t* menu_name_win_input_get(menu_t* menu, char* win_name)
{
  return wins_name_win_input_get(menu->wins, menu->win_count, win_name);
}

void menu_name_win_input_buffer_set(menu_t* menu, char* win_name, char* buffer, size_t size)
{
  win_input_t* win = menu_name_win_input_get(menu, win_name);

  win_input_buffer_set(win, buffer, size);
}

void menu_name_win_input_buffer_clear(menu_t* menu, char* win_name)
{
  win_input_t* win = menu_name_win_input_get(menu, win_name);

  win_input_buffer_clear(win);
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

void menu_name_win_focus_set(menu_t* menu, char* win_name)
{
  wins_name_win_focus_set(menu->wins, menu->win_count, win_name);
}

void menu_win_input_resize(menu_t* menu, char* win_name, int x, int y, int w)
{
  win_input_t* win = menu_name_win_input_get(menu, win_name);

  win_input_resize(win, x, y, w);
}

void menu_win_list_resize(menu_t* menu, char* win_name, int x, int y, int w, int h)
{
  win_list_t* win = menu_name_win_list_get(menu, win_name);

  win_list_resize(win, x, y, w, h);
}

void menu_win_confirm_resize(menu_t* menu, char* win_name, int x, int y, int w)
{
  win_confirm_t* win = menu_name_win_confirm_get(menu, win_name);

  win_confirm_resize(win, x, y, w);
}

void menu_win_input_create(menu_t* menu, char* name, bool active, bool tab_ability, int x, int y, int w, char* buffer, size_t size, char* title, bool secret, win_event_t* event)
{
  win_input_t* win = win_input_create(name, active, tab_ability, x, y, w, buffer, size, title, secret, event);

  menu_win_add(menu, (win_t*) win);
}

void menu_win_confirm_create(menu_t* menu, char* name, bool active, bool tab_ability, int x, int y, int w, char* prompt, char* text_yes, char* text_no, win_event_t* event)
{
  win_confirm_t* win = win_confirm_create(name, active, tab_ability, x, y, w, prompt, text_yes, text_no, event);

  menu_win_add(menu, (win_t*) win);
}

void menu_win_text_create(menu_t* menu, char* name, bool active, bool tab_ability, int x, int y, int w, int h, char* title, char* text, win_event_t* event)
{
  win_text_t* win = win_text_create(name, active, tab_ability, x, y, w, h, title, text, event);

  menu_win_add(menu, (win_t*) win);
}

void menu_win_list_create(menu_t* menu, char* name, bool active, bool tab_ability, int x, int y, int w, int h, int max_count, win_event_t* event)
{
  win_list_t* win = win_list_create(name, active, tab_ability, x, y, w, h, max_count, event);

  menu_win_add(menu, (win_t*) win);
}

int wins_tab_win_index(win_t** wins, int count, bool reverse)
{
  int active_num = 0;

  for(int index = 0; index < count; index++)
  {
    int win_index = reverse ? (count - 1 - index): index;

    win_t* win = wins[win_index];

    if(win->active && win->tab_ability) active_num++;

    if(active_num == 2) return win_index;
  }
  return -1;
}

void menu_win_tab(menu_t* menu, bool reverse)
{
  int index = wins_tab_win_index(menu->wins, menu->win_count, reverse);

  if(index >= 0 && index < menu->win_count)
  {
    wins_rotate(menu->wins, menu->win_count, index);
  }
}

char* menu_name_win_list_item_get(menu_t* menu, char* win_name)
{
  win_list_t* win = menu_name_win_list_get(menu, win_name);

  return win_list_item_get(win);
}

void menu_name_win_list_item_add(menu_t* menu, char* win_name, char* item)
{
  win_list_t* win = menu_name_win_list_get(menu, win_name);

  win_list_item_add(win, item);
}
