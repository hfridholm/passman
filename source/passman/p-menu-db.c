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

  dbase_write(menu->dbase, menu->dbase->name, menu->password);

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

    case KEY_CTRLZ:
      screen_name_menu_focus_set(screen, "dbs");
      break;
  }
}

void menu_db_win_acs_event(win_head_t* win_head, int key)
{
  if(!win_head || win_head->type != WIN_LIST) return;

  win_list_event(win_head, key);

  win_list_t* win = (win_list_t*) win_head;


  menu_t* menu_head = win_head->menu;

  if(!menu_head || menu_head->type != MENU_DB) return;

  menu_db_t* menu = (menu_db_t*) menu_head;


  screen_t* screen = win_head->screen;

  switch(key)
  {
    case 'o': case KEY_ENTR:
      screen_name_menu_focus_set(screen, "act");
      break;

    case 'd':
      menu_name_win_confirm_answer_set((menu_t*) menu, "delete", false);

      menu_name_win_focus_set((menu_t*) menu, "delete");
      break;

    case 'n':
      menu_name_win_input_buffer_clear((menu_t*) menu, "new");

      menu_name_win_focus_set((menu_t*) menu, "new");
      break;

    case 'r':
      char* item = win_list_item_get(win);

      if(!item) break;

      strncpy(menu->buffer_name, item, sizeof(menu->buffer_name));

      menu_name_win_input_buffer_update((menu_t*) menu, "rename");

      menu_name_win_focus_set((menu_t*) menu, "rename");
      break;

    default:
      break;
  }
}

void menu_db_win_rename_enter(menu_db_t* menu, win_input_t* win)
{
  win_list_t* win_list = menu_name_win_list_get((menu_t*) menu, "dbs");

  char* new_name = win->buffer;

  win_list_item_rename(win_list, new_name);

  win_input_buffer_clear(win);

  win->head.active = false;
}

void menu_db_win_rename_event(win_head_t* win_head, int key)
{
  if(!win_head || win_head->type != WIN_INPUT) return;

  win_input_event(win_head, key);

  win_input_t* win = (win_input_t*) win_head;


  menu_head_t* menu_head = win_head->menu;

  if(!menu_head || menu_head->type != MENU_DB) return;

  menu_db_t* menu = (menu_db_t*) menu_head;

  switch(key)
  {
    case KEY_ESC: case KEY_CTRLZ:
      win_input_buffer_clear(win);

      win_head->active = false;
      break;

    case KEY_ENTR:
      menu_db_win_rename_enter(menu, win);
      break;

    default:
      break;
  }
}

void menu_db_win_new_enter(menu_db_t* menu, win_input_t* win)
{
  if(!menu || !win) return;

  dbase_t* dbase = menu->dbase;

  if(!dbase) return;

  if(dbase->accnt_count >= 120) return;

  accnt_t* accnt = &dbase->accnts[dbase->accnt_count];

  strncpy(accnt->name, win->buffer, sizeof(accnt->name));

  dbase->accnt_count++;

  menu_name_win_list_item_add((menu_t*) menu, "acs", accnt->name);

  win_input_buffer_clear(win);

  win->head.active = false;
}

void menu_db_win_new_event(win_head_t* win_head, int key)
{
  if(!win_head || win_head->type != WIN_INPUT) return;

  win_input_event(win_head, key);

  win_input_t* win = (win_input_t*) win_head;


  menu_head_t* menu_head = win_head->menu;

  if(!menu_head || menu_head->type != MENU_DB) return;

  menu_db_t* menu = (menu_db_t*) menu_head;

  switch(key)
  {
    case KEY_ESC: case KEY_CTRLZ:
      win_input_buffer_clear(win);

      win_head->active = false;
      break;

    case KEY_ENTR:
      menu_db_win_new_enter(menu, win);
      break;

    default:
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

  menu->dbase = NULL;

  int x = xmax / 2;
  int y = ymax / 2;
  int w = xmax - 12;

  menu_win_input_create((menu_t*) menu, "name", true, true, x, 5, w, NULL, 0, "Name", false, win_input_event);
  
  menu_win_input_create((menu_t*) menu, "email", true, true, x, 9, w, NULL, 0, "Email", false, win_input_event);

  menu_win_input_create((menu_t*) menu, "save", false, false, x, y, 50, menu->password, sizeof(menu->password), "Password", true, menu_db_win_save_event);

  int h = ymax - 14;

  menu_win_list_create((menu_t*) menu, "acs", true, true, x, (h / 2) + 12, w, h, 120, menu_db_win_acs_event);

  menu_win_input_create((menu_t*) menu, "rename", false, false, x, y, 50, menu->buffer_name, sizeof(menu->buffer_name), "Rename", false, menu_db_win_rename_event);

  menu_win_input_create((menu_t*) menu, "new", false, false, x, y, 50, menu->buffer_name, sizeof(menu->buffer_name), "New", false, menu_db_win_new_event);

  // menu_win_confirm_create((menu_t*) menu, "delete", false, false, x, y, 50, "Delete", menu_db_win_delete_event);

  return menu;
}

/*
 *
 */
void menu_db_free(menu_db_t* menu)
{
  if(!menu) return;

  menu_head_free(menu->head);

  free(menu);
}

void menu_db_dbase_set(menu_db_t* menu, dbase_t* dbase)
{
  menu_name_win_input_buffer_set((menu_t*) menu, "name", dbase->name, sizeof(dbase->name));

  menu_name_win_input_buffer_set((menu_t*) menu, "email", dbase->email, sizeof(dbase->email));

  for(size_t index = 0; index < dbase->accnt_count; index++)
  {
    menu_name_win_list_item_add((menu_t*) menu, "acs", dbase->accnts[index].name);
  }
}
