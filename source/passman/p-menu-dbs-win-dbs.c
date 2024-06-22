#include "../passman.h"

void menu_dbs_win_dbs_event_delete(menu_dbs_t* menu, win_list_t* win)
{
  if(!win_list_hover_item_exists(win)) return;

  menu_name_win_confirm_answer_set((menu_t*) menu, "delete", false);

  menu_name_win_focus_set((menu_t*) menu, "delete");
}

void menu_dbs_win_dbs_event_new(menu_dbs_t* menu, win_list_t* win)
{
  menu_name_win_input_buffer_clear((menu_t*) menu, "new");

  menu_name_win_focus_set((menu_t*) menu, "new");
}

void menu_dbs_win_dbs_event_open(menu_dbs_t* menu, win_list_t* win)
{
  if(!win_list_hover_item_exists(win)) return;

  menu_name_win_focus_set((menu_t*) menu, "open");
}

void menu_dbs_win_dbs_event_rename(menu_dbs_t* menu, win_list_t* win)
{
  if(!win_list_hover_item_exists(win)) return;

  win_input_t* win_rename = menu_name_win_input_get((menu_t*) menu, "rename");

  if(!win_rename) return;

  char* item = win_list_hover_item_string_get(win);

  win_input_buffer_paste(win_rename, item);

  menu_name_win_focus_set((menu_t*) menu, "rename");
}

int menu_dbs_win_dbs_event(win_head_t* win_head, int key)
{
  if(!win_head || win_head->type != WIN_LIST) return 0;

  menu_t* menu_head = win_head->menu;

  if(!menu_head || menu_head->type != MENU_DBS) return 0;


  if(win_list_event(win_head, key)) return 1;


  menu_dbs_t* menu = (menu_dbs_t*) menu_head;

  win_list_t* win = (win_list_t*) win_head;

  switch(key)
  {
    case 'o': case 'O': case KEY_ENTR:
      menu_dbs_win_dbs_event_open(menu, win);
      return 2;

    case 'd': case 'D':
      menu_dbs_win_dbs_event_delete(menu, win);
      return 3;

    case 'n': case 'N':
      menu_dbs_win_dbs_event_new(menu, win);
      return 4;

    case 'r': case 'R':
      menu_dbs_win_dbs_event_rename(menu, win);
      return 5;

    default:
      return 0;
  }
}

static void matrix_free(void** matrix, size_t count)
{
  if(!matrix) return;

  for(size_t index = 0; index < count; index++)
  {
    if(matrix[index]) free(matrix[index]);
  }

  free(matrix);
}

void menu_dbs_win_dbs_create(menu_dbs_t* menu, int x, int y, int w, int h)
{
  win_list_t* win = win_list_create("dbs", true, true, x, y + 2, w, h, -1, menu_dbs_win_dbs_event);

  char** names = NULL;
  size_t count = 0;

  dir_file_names(&names, &count, DBASE_DIR);

  for(size_t index = 0; index < count; index++)
  {
    size_t size = dbase_file_size(names[index]);

    if(size != DBASE_ENCRYPT_SIZE) continue;

    win_list_item_add(win, names[index], NULL);
  }

  matrix_free((void**) names, count);

  menu_win_add((menu_t*) menu, (win_t*) win);
}
