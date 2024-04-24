#include "../passman.h"

bool running = true;

cnfwin_t* extpop; // Exit popup window
cnfwin_t* delpop; // Delete popup window
inpwin_t* pswpop; // Password popup window

popup_t popup;
menu_t  menu;

static void menu_refresh(void)
{
  switch(menu)
  {
    case MENU_DATABASES:
      mendbs_refresh();
      break;

    case MENU_PASSWORD:
      menpsw_refresh();
      break;

    default:
      break;
  }
}

static void popup_refresh(void)
{
  switch(popup)
  {
    case POPUP_DELETE:
      cnfwin_refresh(delpop);
      break;
    
    case POPUP_OPEN:
      inpwin_refresh(pswpop);
      break;

    case POPUP_EXIT:
      cnfwin_refresh(extpop);
      break;

    default:
      break;
  }
}

void screen_refresh(void)
{
  clear();        // Clear the screen
  refresh();      // Refreshes the textures

  menu_refresh();

  popup_refresh();
}

static void popups_resize(int xmax, int ymax)
{
  int x = xmax / 2;
  int y = ymax / 2;
  int w = 50;

  cnfwin_resize(extpop, x, y, w);

  cnfwin_resize(delpop, x, y, w);

  inpwin_resize(pswpop, x, y, w);
}

static void menus_resize(int xmax, int ymax)
{
  mendbs_resize(xmax, ymax);

  menpsw_resize(xmax, ymax);
}

void screen_resize(void)
{
  int xmax = getmaxx(stdscr);
  int ymax = getmaxy(stdscr);

  menus_resize(xmax, ymax);

  popups_resize(xmax, ymax);
}

static void menus_init(int xmax, int ymax)
{
  mendbs_init(xmax, ymax);

  menpsw_init(xmax, ymax);
}

static void menus_free(void)
{
  mendbs_free();

  menpsw_free();
}

static void popups_init(int xmax, int ymax)
{
  int x = xmax / 2;
  int y = ymax / 2;
  int w = 50;

  delpop = cnfwin_create(x, y, w, "Delete Database?", "Yes", "No");

  pswpop = inpwin_create(x, y, w, password, sizeof(password), true);

  extpop = cnfwin_create(x, y, w, "Do you want to exit?", "Yes", "No");
}

static void popups_free(void)
{
  cnfwin_free(extpop);

  cnfwin_free(delpop);

  inpwin_free(pswpop);
}

void screen_init(void)
{
  initscr();
  noecho();
  curs_set(0);
  raw();

  int xmax = getmaxx(stdscr);
  int ymax = getmaxy(stdscr);

  menus_init(xmax, ymax);
  popups_init(xmax, ymax);
}

void screen_free(void)
{
  menus_free();
  popups_free();

  endwin();
}

void screen_key_handler(int key)
{
  switch(key)
  {
    case KEY_CTRLC:
      extpop_input();
      break;

    case KEY_RESIZE:
      screen_resize();
      break;
  }
}
