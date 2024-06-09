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
    free(menu->dbs_names[index]);
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
  if(win_head == NULL || win_head->type != WIN_LIST) return;

  // win_list_t* win = (win_list_t*) win_head;

  // char* item = win->items[win->item_index];

  switch(key)
  {
    case KEY_ENTR:
      break;

    case 'd':
      break;

    case 'n':
      break;

    case 'r':
      break;

    case KEY_TAB:
      break;

    default:
      break;
  }
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

  menu_win_input_create((menu_t*) menu, "search", x, 5, w, menu->buffer_search, sizeof(menu->buffer_search), NULL, false, true, win_input_key_handler);
  
  menu_win_list_create((menu_t*) menu, "dbs", x, y + 2, w, h, menu->dbs_names, menu->dbs_count, true, menu_dbs_win_dbs_key_handler);

  menu_win_confirm_create((menu_t*) menu, "delete", x, y, 30, "Delete Database?", "Yes", "No", false, win_confirm_key_handler);

  menu_win_input_create((menu_t*) menu, "open", x, y, 50, password, sizeof(password), "Password", true, false, win_input_key_handler);

  menu_win_input_create((menu_t*) menu, "new", x, y, 50, menu->buffer_new, sizeof(menu->buffer_new), "New", false, false, win_input_key_handler);

  menu_win_input_create((menu_t*) menu, "rename", x, y, 50, NULL, 0, "Rename", false, false, win_input_key_handler);

  return menu;
}

void menu_dbs_free(menu_dbs_t* menu)
{
  if(menu == NULL) return;

  menu_dbs_names_free(menu);

  menu_head_free(menu->head);

  free(menu);
}
