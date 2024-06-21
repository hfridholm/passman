#include "../passman.h"

void matrix_free(void** matrix, size_t count)
{
  if(!matrix) return;

  for(size_t index = 0; index < count; index++)
  {
    if(matrix[index]) free(matrix[index]);
  }

  free(matrix);
}

static void menu_dbs_names_create(menu_dbs_t* menu)
{
  char** names = NULL;
  size_t count = 0;

  dir_file_names(&names, &count, DBASE_DIR);

  for(size_t index = 0; index < count; index++)
  {
    size_t size = dbase_file_size(names[index]);

    if(size != DBASE_ENCRYPT_SIZE) continue;

    menu->dbs_names = realloc(menu->dbs_names, sizeof(char*) * (menu->dbs_count + 1));

    menu->dbs_names[menu->dbs_count] = strdup(names[index]);

    menu->dbs_count++;
  }

  matrix_free((void**) names, count);
}

static void menu_dbs_names_free(menu_dbs_t* menu)
{
  for(int index = 0; index < menu->dbs_count; index++)
  {
    char* name = menu->dbs_names[index];

    if(name) free(name);
  }

  free(menu->dbs_names);
}

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

void menu_dbs_win_dbs_event(win_head_t* win_head, int key)
{
  if(!win_head || win_head->type != WIN_LIST) return;

  win_list_event(win_head, key);


  menu_t* menu_head = win_head->menu;

  if(!menu_head || menu_head->type != MENU_DBS) return;

  menu_dbs_t* menu = (menu_dbs_t*) menu_head;


  switch(key)
  {
    case 'o': case KEY_ENTR:
      menu_name_win_focus_set((menu_t*) menu, "open");
      break;

    case 'd':
      win_confirm_t* win_delete = menu_name_win_confirm_get((menu_t*) menu, "delete");

      if(win_delete) win_delete->answer = false;

      menu_name_win_focus_set((menu_t*) menu, "delete");
      break;

    case 'n':
      win_input_t* win_new = menu_name_win_input_get((menu_t*) menu, "new");

      memset(menu->buffer_name, '\0', sizeof(menu->buffer_name));

      if(win_new) win_input_buffer_update(win_new);

      menu_name_win_focus_set((menu_t*) menu, "new");
      break;

    case 'r':
      win_input_t* win_rename = menu_name_win_input_get((menu_t*) menu, "rename");

      char* item = win_list_item_get((win_list_t*) win_head);

      if(!item) break;

      strncpy(menu->buffer_name, item, sizeof(menu->buffer_name));

      if(win_rename) win_input_buffer_update(win_rename);

      menu_name_win_focus_set((menu_t*) menu, "rename");
      break;

    default:
      break;
  }
}

void menu_dbs_win_delete_event(win_head_t* win_head, int key)
{
  if(!win_head || win_head->type != WIN_CONFIRM) return;

  win_confirm_event(win_head, key);


  if(key != KEY_ENTR) return;


  menu_head_t* menu_head = win_head->menu;

  if(!menu_head || menu_head->type != MENU_DBS) return;


  win_list_t* win_list = menu_name_win_list_get(menu_head, "dbs");

  char* item = win_list_item_get(win_list);

  printf("Deleting: %s\n", item ? item : "unknown");

  win_head->active = false;
}

void menu_dbs_win_open_enter(menu_dbs_t* menu, win_input_t* win)
{
  win_list_t* win_list = menu_name_win_list_get((menu_t*) menu, "dbs");

  char* item = win_list_item_get(win_list);


  screen_t* screen = menu->head.screen;

  if(!screen) return;

  int status = dbase_read(&screen->dbase, item, menu->password);

  if(status == 0)
  {
    screen_name_menu_db_dbase_set(screen, "db", &screen->dbase);

    screen_name_menu_focus_set(screen, "db");
  }
  else screen_text_popup(screen, "Error", "Could not open dbase");


  menu_name_win_input_buffer_clear((menu_t*) menu, "open");

  win->head.active = false;
}

void menu_dbs_win_open_event(win_head_t* win_head, int key)
{
  if(!win_head || win_head->type != WIN_INPUT) return;

  win_input_event(win_head, key);

  win_input_t* win = (win_input_t*) win_head;


  menu_head_t* menu_head = win_head->menu;

  if(!menu_head || menu_head->type != MENU_DBS) return;

  menu_dbs_t* menu = (menu_dbs_t*) menu_head;


  switch(key)
  {
    case KEY_ESC: case KEY_CTRLZ:
      menu_name_win_input_buffer_clear((menu_t*) menu, "open");

      win_head->active = false;
      break;

    case KEY_ENTR:
      menu_dbs_win_open_enter(menu, win);
      break;

    default:
      break;
  }
}

void menu_dbs_win_new_event(win_head_t* win_head, int key)
{
  if(!win_head || win_head->type != WIN_INPUT) return;

  win_input_event(win_head, key);

  win_input_t* win = (win_input_t*) win_head;


  if(key != KEY_ENTR) return;


  menu_head_t* menu_head = win_head->menu;

  if(!menu_head || menu_head->type != MENU_DBS) return;


  screen_t* screen = win_head->screen;

  if(!screen) return;


  screen->dbase = (dbase_t) {0};

  strcpy(screen->dbase.name, win->buffer);


  win_list_t* win_list = menu_name_win_list_get(menu_head, "dbs");

  win_list_item_add(win_list, win->buffer);


  screen_name_menu_db_dbase_set(screen, "db", &screen->dbase);

  screen_name_menu_focus_set(screen, "db");


  win_head->active = false;
}

void menu_dbs_win_dbs_create(menu_dbs_t* menu, int x, int y, int w, int h)
{
  win_list_t* win = win_list_create("dbs", true, true,
    x, y + 2, w, h, -1, menu_dbs_win_dbs_event);

  for(size_t index = 0; index < menu->dbs_count; index++)
  {
    win_list_item_add(win, menu->dbs_names[index]);
  }

  menu_win_add((menu_t*) menu, (win_t*) win);
}

void menu_dbs_win_rename_event(win_head_t* win_head, int key)
{
  if(!win_head || win_head->type != WIN_INPUT) return;

  win_input_event(win_head, key);

  win_input_t* win = (win_input_t*) win_head;


  if(key != KEY_ENTR) return;


  menu_head_t* menu_head = win_head->menu;

  if(!menu_head || menu_head->type != MENU_DBS) return;

  menu_dbs_t* menu = (menu_dbs_t*) menu_head;


  win_list_t* win_list = menu_name_win_list_get((menu_t*) menu, "dbs");

  char* old_name = win_list_item_get(win_list);

  char* new_name = win->buffer;

  dbase_file_rename(old_name, new_name);

  win_list_item_rename(win_list, new_name);


  win_head->active = false;
}

void menu_dbs_event(menu_head_t* menu_head, int key)
{
  if(!menu_head || menu_head->type != MENU_DBS) return;

  menu_dbs_t* menu = (menu_dbs_t*) menu_head;

  screen_t* screen = menu_head->screen;

  if(!screen) return;

  switch(key)
  {
    case KEY_CTRLZ:
      screen_name_win_focus_set(screen, "exit");
      break;
  }
}

menu_dbs_t* menu_dbs_create(char* name, int xmax, int ymax)
{
  menu_dbs_t* menu = malloc(sizeof(menu_dbs_t));

  menu->head = menu_head_create(MENU_DBS, name, menu_dbs_event);

  menu_dbs_names_create(menu);

  int x = xmax / 2;
  int y = ymax / 2;
  int w = xmax - 12;
  int h = ymax - 10;

  memset(menu->buffer_search, '\0', sizeof(menu->buffer_search));

  memset(menu->buffer_name, '\0', sizeof(menu->buffer_name));


  menu_win_input_create((menu_t*) menu, "search", true, true,
    x, 5, w, menu->buffer_search, sizeof(menu->buffer_search), NULL, false, win_input_event);
  
  menu_dbs_win_dbs_create(menu, x, y, w, h);

  menu_win_confirm_create((menu_t*) menu, "delete", false, false,
    x, y, 30, "Delete Database?", "Yes", "No", menu_dbs_win_delete_event);

  menu_win_input_create((menu_t*) menu, "open", false, false,
    x, y, 50, menu->password, sizeof(menu->password), "Password", true, menu_dbs_win_open_event);

  menu_win_input_create((menu_t*) menu, "new", false, false,
    x, y, 50, menu->buffer_name, sizeof(menu->buffer_name), "New", false, menu_dbs_win_new_event);

  menu_win_input_create((menu_t*) menu, "rename", false, false,
    x, y, 50, menu->buffer_name, sizeof(menu->buffer_name), "Rename", false, menu_dbs_win_rename_event);

  return menu;
}

void menu_dbs_free(menu_dbs_t* menu)
{
  if(!menu) return;

  menu_dbs_names_free(menu);

  menu_head_free(menu->head);

  free(menu);
}
