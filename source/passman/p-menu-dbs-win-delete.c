#include "../passman.h"

static void menu_dbs_win_delete_event_enter(menu_dbs_t* menu, win_confirm_t* win)
{
  win_list_t* win_dbs = menu_name_win_list_get((menu_t*) menu, "dbs");

  char* item = win_list_hover_item_string_get(win_dbs);

  refresh();

  printf("Deleting: %s\n", item ? item : "unknown");

  getch();

  win->head.active = false;
}

int menu_dbs_win_delete_event(win_head_t* win_head, int key)
{
  if(!win_head || win_head->type != WIN_CONFIRM) return 0;

  menu_head_t* menu_head = win_head->menu;

  if(!menu_head || menu_head->type != MENU_DBS) return 0;


  if(win_confirm_event(win_head, key)) return 1;


  menu_dbs_t* menu = (menu_dbs_t*) menu_head;

  win_confirm_t* win = (win_confirm_t*) win_head;

  switch(key)
  {
    case KEY_ENTR:
      menu_dbs_win_delete_event_enter(menu, win);
      return 2;

    default:
      return 0;
  }
}
