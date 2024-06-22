#include "../passman.h"

void menu_dbs_resize(menu_dbs_t* menu, int xmax, int ymax)
{
  int x = xmax / 2;
  int y = ymax / 2;
  int w = xmax - 12;
  int h = ymax - 10;

  menu_win_input_resize((menu_t*) menu, "search", x, 5, w);

  menu_win_list_resize((menu_t*) menu, "dbs", x, y + 2, w, h);

  menu_win_confirm_resize((menu_t*) menu, "delete", x, y, 30);

  menu_win_input_resize((menu_t*) menu, "open", x, y, 50);

  menu_win_input_resize((menu_t*) menu, "new", x, y, 50);

  menu_win_input_resize((menu_t*) menu, "rename", x, y, 50);
}

static int menu_dbs_event(menu_head_t* menu_head, int key)
{
  if(!menu_head || menu_head->type != MENU_DBS) return 0;

  screen_t* screen = menu_head->screen;

  if(!screen) return 0;

  switch(key)
  {
    case KEY_CTRLZ:
      screen_name_win_focus_set(screen, "exit");
      return 1;

    default:
      return 0;
  }
}

extern int menu_dbs_win_delete_event(win_head_t* win_head, int key);

extern int menu_dbs_win_dbs_create(menu_dbs_t* menu, int x, int y, int w, int h);

extern int menu_dbs_win_open_event(win_head_t* win_head, int key);

extern int menu_dbs_win_new_event(win_head_t* win_head, int key);

extern int menu_dbs_win_rename_event(win_head_t* win_head, int key);

extern int menu_dbs_win_search_event(win_head_t* win_head, int key);


menu_dbs_t* menu_dbs_create(char* name, int xmax, int ymax)
{
  menu_dbs_t* menu = malloc(sizeof(menu_dbs_t));

  menu->head = menu_head_create(MENU_DBS, name, menu_dbs_event);


  memset(menu->buffer_search, '\0', sizeof(menu->buffer_search));

  memset(menu->buffer_name, '\0', sizeof(menu->buffer_name));

  menu->dbase = NULL;


  int x = xmax / 2;
  int y = ymax / 2;
  int w = xmax - 12;
  int h = ymax - 10;

  menu_win_input_create((menu_t*) menu, "search", true, true, x, 5, w, menu->buffer_search, sizeof(menu->buffer_search), NULL, false, menu_dbs_win_search_event);
  
  menu_dbs_win_dbs_create(menu, x, y, w, h);

  menu_win_confirm_create((menu_t*) menu, "delete", false, false, x, y, 30, "Delete Database?", "Yes", "No", menu_dbs_win_delete_event);

  menu_win_input_create((menu_t*) menu, "open", false, false, x, y, 50, menu->password, sizeof(menu->password), "Password", true, menu_dbs_win_open_event);

  menu_win_input_create((menu_t*) menu, "new", false, false, x, y, 50, menu->buffer_name, sizeof(menu->buffer_name), "New", false, menu_dbs_win_new_event);

  menu_win_input_create((menu_t*) menu, "rename", false, false, x, y, 50, menu->buffer_name, sizeof(menu->buffer_name), "Rename", false, menu_dbs_win_rename_event);


  menu_name_win_focus_set((menu_t*) menu, "dbs");

  return menu;
}

void menu_dbs_free(menu_dbs_t* menu)
{
  if(!menu) return;

  menu_head_free(menu->head);

  free(menu);
}
