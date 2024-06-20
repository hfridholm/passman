#include "../passman.h"

/*
 *
 */
void menu_db_resize(menu_db_t* menu, int xmax, int ymax)
{
  int x = xmax / 2;
  int w = xmax - 12;

  menu_win_input_resize((menu_t*) menu, "name", x, 5, w);

  menu_win_input_resize((menu_t*) menu, "email", x, 9, w);
}

/*
 *
 */
void menu_db_win_save_event(win_head_t* win_head, int key)
{
  if(!win_head || win_head->type != WIN_INPUT) return;

  win_input_event(win_head, key);

  win_input_t* win = (win_input_t*) win_head;


  menu_t* menu_head = win_head->menu;

  if(!menu_head || menu_head->type != MENU_DB) return;

  menu_db_t* menu = (menu_db_t*) menu_head;


  if(key != KEY_ENTR) return;


  screen_t* screen = win_head->screen;

  if(!screen) return;


  dbase_write(&screen->dbase, screen->dbase.name, menu->password);

  screen_text_popup(screen, "Save", "Saved dbase");


  win_head->active = false;
}

void menu_db_event(menu_head_t* menu_head, int key)
{
  if(!menu_head || menu_head->type != MENU_DB) return;

  menu_db_t* menu = (menu_db_t*) menu_head;

  screen_t* screen = menu_head->screen;

  if(!screen) return;

  switch(key)
  {
    case KEY_CTRLS:
      menu_name_win_focus_set((menu_t*) menu, "save");
      break;
  }
}

/*
 *
 */
menu_db_t* menu_db_create(char* name, int xmax, int ymax)
{
  menu_db_t* menu = malloc(sizeof(menu_db_t));

  menu->head = menu_head_create(MENU_DB, name, menu_db_event);

  int x = xmax / 2;
  int y = ymax / 2;
  int w = xmax - 12;

  menu_win_input_create((menu_t*) menu, "name", true, true,
    x, 5, w, NULL, 0, "Name", false, win_input_event);
  
  menu_win_input_create((menu_t*) menu, "email", true, true,
    x, 9, w, NULL, 0, "Email", false, win_input_event);

  menu_win_input_create((menu_t*) menu, "save", false, false,
    x, y, w, menu->password, sizeof(menu->password), "Password", true, menu_db_win_save_event);

  return menu;
}

/*
 *
 */
void menu_db_free(menu_db_t* menu)
{
  if(menu == NULL) return;

  menu_head_free(menu->head);

  free(menu);
}

void menu_db_dbase_set(menu_db_t* menu, dbase_t* dbase)
{
  menu_name_win_input_buffer_set((menu_t*) menu, "name", dbase->name, sizeof(dbase->name));

  menu_name_win_input_buffer_set((menu_t*) menu, "email", dbase->email, sizeof(dbase->email));
}
