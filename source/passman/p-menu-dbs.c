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

  win_input_resize(menu->wins[0], x, 5, w);

  win_list_resize(menu->wins[1], x, y + 2, w, h);


  win_confirm_resize(menu->pops[0], x, y, 30);

  win_input_resize(menu->pops[1], x, y, 50);

  win_input_resize(menu->pops[2], x, y, 50);

  win_input_resize(menu->pops[3], x, y, 50);
}

menu_dbs_t* menu_dbs_create(int xmax, int ymax)
{
  menu_dbs_t* menu = malloc(sizeof(menu_dbs_t));

  menu->type = MENU_DBS;

  menu_dbs_names_create(menu);

  menu->win_count = 2;

  menu->wins = malloc(sizeof(win_t*) * menu->win_count);

  int x = xmax / 2;
  int y = ymax / 2;
  int w = xmax - 12;
  int h = ymax - 10;

  menu->wins[0] = win_input_create(x, 5, w, menu->buffer_search, sizeof(menu->buffer_search), NULL, false, true);
  
  menu->wins[1] = win_list_create(x, y + 2, w, h, menu->dbs_names, menu->dbs_count, true);


  menu->pop_count = 4;

  menu->pops = malloc(sizeof(win_t*) * menu->pop_count);

  menu->pops[0] = win_confirm_create(x, y, 30, "Delete Database?", "Yes", "No", false);

  menu->pops[1] = win_input_create(x, y, 50, password, sizeof(password), "Password", true, false);

  menu->pops[2] = win_input_create(x, y, 50, menu->buffer_new, sizeof(menu->buffer_new), "New", false, false);

  mneu->pops[3] = win_input_create(x, y, 50, NULL, 0, "Rename", false, false);
}

void menu_dbs_free(menu_psw_t* menu)
{
  if(menu == NULL) return;

  menu_dbs_names_free(menu);

  menu_head_free(menu);

  free(menu);
}

static void menu_dbs_win_dbs_key_handler(int key)
{
  char* item = dbases->items[dbases->index];

  switch(key)
  {
    case KEY_ENTR:
      inppop_input(opnpop, NULL);

      infpop_input("Info", "The password was wrong");
      break;

    case 'd':
      char prompt[128];
  
      sprintf(prompt, "Delete \"%s\"?", item);

      win_confirm_prompt_set(delpop, prompt);

      screen_resize();

      delpop->answer = false;

      cnfpop_input(delpop, NULL);

      if(delpop->answer)
      {
        char text[128];

        sprintf(text, "The database \"%s\" was deleted", item);
        
        infpop_input("Info", text);
      }
      break;

    case 'n':
      inppop_input(newpop, NULL);

      if(newpop->length)
      {
        char text[128];

        sprintf(text, "New database \"%s\" was created", newpop->buffer);
        
        infpop_input("Info", text);
      }
      break;

    case 'r':
      win_input_buffer_set(rnmpop, item, 64);

      screen_resize();

      inppop_input(rnmpop, NULL);
      break;

    case KEY_TAB:
      win_input_input(search, NULL);
      break;

    default:
      break;
  }
}
