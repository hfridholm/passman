#include "../passman.h"

char* names[] = {"Secret", "Home", "School", "This", "That", "Sweden", "Hampus", "Nogger"};
int dbeamt = 8;

char** dbenms;

lstwin_t* dbases;
inpwin_t* search;
char srcbuf[64];

cnfwin_t* delpop;
inpwin_t* rnmpop;
inpwin_t* opnpop;

inpwin_t* newpop;
char newbuf[64];

static void mendbs_dbenms_create(void)
{
  dbenms = malloc(sizeof(char*) * dbeamt);

  for(int index = 0; index < dbeamt; index++)
  {
    dbenms[index] = malloc(sizeof(char) * 64);

    strcpy(dbenms[index], names[index]);
  }
}

static void mendbs_dbenms_free(void)
{
  for(int index = 0; index < dbeamt; index++)
  {
    free(dbenms[index]);
  }
  free(dbenms);
}

void mendbs_refresh(void)
{
  lstwin_refresh(dbases);

  inpwin_refresh(search);

  cnfwin_refresh(delpop);

  inpwin_refresh(opnpop);

  inpwin_refresh(newpop);

  inpwin_refresh(rnmpop);
}

static void mendbs_popups_resize(int xmax, int ymax)
{
  int x = xmax / 2;
  int y = ymax / 2;

  cnfwin_resize(delpop, x, y, 30);

  inpwin_resize(opnpop, x, y, 50);

  inpwin_resize(newpop, x, y, 50);

  inpwin_resize(rnmpop, x, y, 50);
}

void mendbs_resize(int xmax, int ymax)
{
  int x = xmax / 2;
  int y = (ymax / 2) + 2;
  int w = xmax - 12;
  int h = ymax - 10;

  inpwin_resize(search, x, 5, w);

  lstwin_resize(dbases, x, y, w, h);

  mendbs_popups_resize(xmax, ymax);
}

static void mendbs_popups_init(int xmax, int ymax)
{
  int x = xmax / 2;
  int y = ymax / 2;

  delpop = cnfwin_create(x, y, 30, "Delete Database?", "Yes", "No", false);

  opnpop = inpwin_create(x, y, 50, password, sizeof(password), "Password", true, false);

  newpop = inpwin_create(x, y, 50, newbuf, sizeof(newbuf), "New", false, false);

  rnmpop = inpwin_create(x, y, 50, NULL, 0, "Rename", false, false);
}

void mendbs_init(int xmax, int ymax)
{
  mendbs_dbenms_create();

  int x = xmax / 2;
  int y = (ymax / 2) + 2;
  int w = xmax - 12;
  int h = ymax - 10;

  search = inpwin_create(x, 5, w, srcbuf, sizeof(srcbuf), NULL, false, true);
  
  dbases = lstwin_create(x, y, w, h, dbenms, dbeamt, true);

  mendbs_popups_init(xmax, ymax);
}

void mendbs_free(void)
{
  lstwin_free(dbases);

  inpwin_free(search);

  cnfwin_free(delpop);

  inpwin_free(opnpop);

  inpwin_free(newpop);

  inpwin_free(rnmpop);

  mendbs_dbenms_free();
}

static void mendbs_dbases_key_handler(int key)
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

      cnfwin_prompt_set(delpop, prompt);

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
      inpwin_buffer_set(rnmpop, item, 64);

      screen_resize();

      inppop_input(rnmpop, NULL);
      break;

    case KEY_TAB:
      inpwin_input(search, NULL);
      break;

    default:
      break;
  }
}

void mendbs_input(void)
{
  menu = MENU_DATABASES;

  lstwin_input(dbases, &mendbs_dbases_key_handler);
}
