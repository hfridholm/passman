#include "../passman.h"

win_t* menu_name_pop_get(menu_t* menu, const char* pop_name)
{
  for(int index = 0; index < menu->pop_count; index++)
  {
    win_t* pop = menu->pops[index];

    if(strcmp(pop->name, pop_name) == 0) return pop;
  }
  return NULL;
}

void menu_pop_add(menu_t* menu, win_t* pop)
{
  menu->pops = realloc(menu->pops, sizeof(menu_t*) * (menu->pop_count + 1));

  pop->menu = menu;

  menu->pops[menu->pop_count++] = pop;
}

win_t* menu_pop_get(menu_t* menu)
{
  if(menu->pop_index >= 0 && menu->pop_index < menu->pop_count)
  {
    return menu->pops[menu->pop_index];
  }
  return NULL;
}

int menu_pop_input_resize(menu_t* menu, const char* pop_name, int x, int y, int w)
{
  win_t* pop = menu_name_pop_get(menu, pop_name);

  if(pop == NULL) return 1;

  if(pop->type != WIN_INPUT) return 2;

  win_input_resize((win_input_t*) pop, x, y, w);
  
  return 0;
}

int menu_pop_list_resize(menu_t* menu, const char* pop_name, int x, int y, int w, int h)
{
  win_t* pop = menu_name_pop_get(menu, pop_name);

  if(pop == NULL) return 1;

  if(pop->type != WIN_LIST) return 2;

  win_list_resize((win_list_t*) pop, x, y, w, h);
  
  return 0;
}

int menu_pop_confirm_resize(menu_t* menu, const char* pop_name, int x, int y, int w)
{
  win_t* pop = menu_name_pop_get(menu, pop_name);

  if(pop == NULL) return 1;

  if(pop->type != WIN_CONFIRM) return 2;

  win_confirm_resize((win_confirm_t*) pop, x, y, w);
  
  return 0;
}

void menu_pop_input_create(menu_t* menu, char* name, int x, int y, int w, char* buffer, size_t size, char* title, bool secret, bool active, key_handler_t* key_handler)
{
  win_input_t* pop = win_input_create(name, x, y, w, buffer, size, title, secret, active, key_handler);

  menu_pop_add(menu, (win_t*) pop);
}

void menu_pop_confirm_create(menu_t* menu, char* name, int x, int y, int w, char* prompt, char* text_yes, char* text_no, bool active, key_handler_t* key_handler)
{
  win_confirm_t* pop = win_confirm_create(name, x, y, w, prompt, text_yes, text_no, active, key_handler);

  menu_pop_add(menu, (win_t*) pop);
}
