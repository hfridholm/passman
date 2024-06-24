#include "../passman.h"

/*
 *
 */
void menu_db_resize(menu_db_t* menu, int xmax, int ymax)
{
  int x = xmax / 2;
  int y = ymax / 2;
  int w = xmax - 12;

  menu_win_input_resize((menu_t*) menu, "name", x, 5, w);

  menu_win_input_resize((menu_t*) menu, "email", x, 9, w);

  menu_win_input_resize((menu_t*) menu, "search", x, 13, w);

  menu_win_list_resize((menu_t*) menu, "acs", x, y + 7, w, ymax - 18); 


  menu_win_input_resize((menu_t*) menu, "save", x, y, 50);

  menu_win_input_resize((menu_t*) menu, "rename", x, y, 50);

  menu_win_input_resize((menu_t*) menu, "new", x, y, 50);

  menu_win_confirm_resize((menu_t*) menu, "delete", x, y, 40);
}

/*
 *
 */
int menu_db_event(menu_head_t* menu_head, int key)
{
  if(!menu_head || menu_head->type != MENU_DB) return 0;

  menu_db_t* menu = (menu_db_t*) menu_head;

  switch(key)
  {
    case KEY_CTRLS:
      menu_win_focus_set((menu_t*) menu, "save");
      return 1;

    case KEY_CTRLZ:
      screen_menu_focus_set(menu->head.screen, "dbs");
      return 2;

    default:
      return 0;
  }
}

extern int menu_db_win_new_event(win_head_t* win_head, int key);

extern int menu_db_win_rename_event(win_head_t* win_head, int key);

extern int menu_db_win_save_event(win_head_t* win_head, int key);

extern int menu_db_win_delete_event(win_head_t* win_head, int key);

extern int menu_db_win_acs_event(win_head_t* win_head, int key);

extern int menu_db_win_search_event(win_head_t* win_head, int key);


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

  menu_win_input_create((menu_t*) menu, "search", true, true, x, 13, w, menu->buffer_search, sizeof(menu->buffer_search), "Search", false, menu_db_win_search_event);
  
  menu_win_list_create((menu_t*) menu, "acs", true, true, x, y + 7, w, ymax - 18, 120, menu->buffer_search, menu_db_win_acs_event);


  menu_win_input_create((menu_t*) menu, "save", false, false, x, y, 50, menu->password, sizeof(menu->password), "Password", true, menu_db_win_save_event);

  menu_win_input_create((menu_t*) menu, "rename", false, false, x, y, 50, menu->buffer_name, sizeof(menu->buffer_name), "Rename", false, menu_db_win_rename_event);

  menu_win_input_create((menu_t*) menu, "new", false, false, x, y, 50, menu->buffer_name, sizeof(menu->buffer_name), "New", false, menu_db_win_new_event);

  menu_win_confirm_create((menu_t*) menu, "delete", false, false, x, y, 40, "Delete?", "Yes", "No", menu_db_win_delete_event);

  menu_win_focus_set((menu_t*) menu, "acs");

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

/*
 * RETURN (int status)
 * - 0 | Success!
 * - 1 | Bad input
 */
int menu_db_dbase_fill(menu_db_t* menu, dbase_t* dbase)
{
  if(!menu || !dbase) return 1;

  menu->dbase = dbase;

  menu_win_input_buffer_set((menu_t*) menu, "name", dbase->name, sizeof(dbase->name));

  menu_win_input_buffer_set((menu_t*) menu, "email", dbase->email, sizeof(dbase->email));

  win_list_t* win_acs = menu_win_list_get((menu_t*) menu, "acs");

  win_list_items_empty(win_acs);

  for(size_t index = 0; index < dbase->accnt_count; index++)
  {
    accnt_t accnt = dbase->accnts[index];

    win_list_item_add(win_acs, accnt.name, NULL);
  }
  return 0;
}
