#include "../passman.h"

char* dbenms[] = {"Secret", "Home", "School", "This", "That", "Sweden", "Hampus", "Nogger"};

lstwin_t* dbases;
inpwin_t* search;
char srcbuf[64];

cnfwin_t* delpop;
inpwin_t* rnmpop;
inpwin_t* newpop;
inpwin_t* opnpop;

char buffer[64];

void mendbs_refresh(void)
{
  lstwin_refresh(dbases);

  inpwin_refresh(search);

  cnfwin_refresh(delpop);

  inpwin_refresh(opnpop);
}

static void mendbs_popups_resize(int xmax, int ymax)
{
  int x = xmax / 2;
  int y = ymax / 2;
  int w = 50;

  cnfwin_resize(delpop, x, y, w);

  inpwin_resize(opnpop, x, y, w);
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
  int w = 50;

  delpop = cnfwin_create(x, y, w, "Delete Database?", "Yes", "No", false);

  opnpop = inpwin_create(x, y, w, password, sizeof(password), true, false);
}

void mendbs_init(int xmax, int ymax)
{
  int x = xmax / 2;
  int y = (ymax / 2) + 2;
  int w = xmax - 12;
  int h = ymax - 10;

  search = inpwin_create(x, 5, w, srcbuf, sizeof(srcbuf), false, true);
  
  dbases = lstwin_create(x, y, w, h, dbenms, 8, true);

  mendbs_popups_init(xmax, ymax);
}

void mendbs_free(void)
{
  lstwin_free(dbases);

  inpwin_free(search);

  cnfwin_free(delpop);

  inpwin_free(opnpop);
}

static void mendbs_dbases_key_handler(int key)
{
  switch(key)
  {
    case KEY_ENTR:
      inppop_input(opnpop, NULL);
      break;

    case 'd':
      cnfpop_input(delpop, NULL);
      break;

    case 'n':
      break;

    case 'r':
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
