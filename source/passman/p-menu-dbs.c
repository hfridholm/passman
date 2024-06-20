#include "../passman.h"

char* names[] = {"Secret", "Home", "School", "This", "That", "Sweden", "Hampus", "Nogger"};
int name_count = 8;

static void menu_dbs_names_create(menu_dbs_t* menu)
{
  menu->dbs_names = malloc(sizeof(char*) * name_count);

  for(int index = 0; index < name_count; index++)
  {
    menu->dbs_names[index] = malloc(sizeof(char) * 64);

    strcpy(menu->dbs_names[index], names[index]);
  }

  menu->dbs_count = name_count;
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

void menu_dbs_win_dbs_key_handler(win_head_t* win_head, int key)
{
  if(!win_head || win_head->type != WIN_LIST) return;

  win_list_key_handler(win_head, key);


  if(!win_list_item_get((win_list_t*) win_head)) return;


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

      memset(menu->buffer_new, '\0', sizeof(menu->buffer_new));

      if(win_new) win_input_buffer_update(win_new);

      menu_name_win_focus_set((menu_t*) menu, "new");
      break;

    case 'r':
      win_input_t* win_rename = menu_name_win_input_get((menu_t*) menu, "rename");

      char* item = win_list_item_get((win_list_t*) win_head);

      if(win_rename) win_input_buffer_set(win_rename, item, strlen(item) + 1);

      menu_name_win_focus_set((menu_t*) menu, "rename");
      break;

    default:
      break;
  }
}

void menu_dbs_win_delete_key_handler(win_head_t* win_head, int key)
{
  if(!win_head || win_head->type != WIN_CONFIRM) return;

  win_confirm_key_handler(win_head, key);


  if(key != KEY_ENTR) return;


  menu_head_t* menu_head = win_head->menu;

  if(!menu_head || menu_head->type != MENU_DBS) return;


  win_list_t* win_list = menu_name_win_list_get(menu_head, "dbs");

  char* item = win_list_item_get(win_list);

  printf("Deleting: %s\n", item ? item : "unknown");

  win_head->active = false;
}

void menu_dbs_win_open_key_handler(win_head_t* win_head, int key)
{
  if(!win_head || win_head->type != WIN_INPUT) return;

  win_input_key_handler(win_head, key);


  if(key != KEY_ENTR) return;


  menu_head_t* menu_head = win_head->menu;

  if(!menu_head || menu_head->type != MENU_DBS) return;


  win_list_t* win_list = menu_name_win_list_get(menu_head, "dbs");

  char* item = win_list_item_get(win_list);

  printf("Opening: %s\n", item ? item : "unknown");

  win_head->active = false;
}

void menu_dbs_win_new_key_handler(win_head_t* win_head, int key)
{
  if(!win_head || win_head->type != WIN_INPUT) return;

  win_input_key_handler(win_head, key);

  win_input_t* win = (win_input_t*) win_head;


  if(key != KEY_ENTR) return;


  menu_head_t* menu_head = win_head->menu;

  if(!menu_head || menu_head->type != MENU_DBS) return;



  win_list_t* win_list = menu_name_win_list_get(menu_head, "dbs");

  win_list_item_add(win_list, win->buffer);


  win_head->active = false;
}

void menu_dbs_win_dbs_create(menu_dbs_t* menu, int x, int y, int w, int h)
{
  win_list_t* win = win_list_create("dbs", true, true,
    x, y + 2, w, h, menu_dbs_win_dbs_key_handler);

  for(int index = 0; index < menu->dbs_count; index++)
  {
    win_list_item_add(win, menu->dbs_names[index]);
  }

  menu_win_add((menu_t*) menu, (win_t*) win);
}

menu_dbs_t* menu_dbs_create(char* name, int xmax, int ymax)
{
  menu_dbs_t* menu = malloc(sizeof(menu_dbs_t));

  menu->head = menu_head_create(MENU_DBS, name);

  menu_dbs_names_create(menu);

  int x = xmax / 2;
  int y = ymax / 2;
  int w = xmax - 12;
  int h = ymax - 10;

  memset(menu->buffer_search, '\0', sizeof(menu->buffer_search));

  memset(menu->buffer_new, '\0', sizeof(menu->buffer_new));


  menu_win_input_create((menu_t*) menu, "search", true, true,
    x, 5, w, menu->buffer_search, sizeof(menu->buffer_search), NULL, false, win_input_key_handler);
  
  menu_dbs_win_dbs_create(menu, x, y, w, h);

  menu_win_confirm_create((menu_t*) menu, "delete", false, false,
    x, y, 30, "Delete Database?", "Yes", "No", menu_dbs_win_delete_key_handler);

  menu_win_input_create((menu_t*) menu, "open", false, false,
    x, y, 50, password, sizeof(password), "Password", true, menu_dbs_win_open_key_handler);

  menu_win_input_create((menu_t*) menu, "new", false, false,
    x, y, 50, menu->buffer_new, sizeof(menu->buffer_new), "New", false, menu_dbs_win_new_key_handler);

  menu_win_input_create((menu_t*) menu, "rename", false, false,
    x, y, 50, NULL, 0, "Rename", false, pop_input_key_handler);

  return menu;
}

void menu_dbs_free(menu_dbs_t* menu)
{
  if(menu == NULL) return;

  menu_dbs_names_free(menu);

  menu_head_free(menu->head);

  free(menu);
}
