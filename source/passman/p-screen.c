#include "../passman.h"

void screen_refresh(screen_t* screen)
{
  // clear();
  refresh();

  if(screen->menu_index >= 0 && screen->menu_index < screen->menu_count)
  {
    menu_t* menu = screen->menus[screen->menu_index];

    if(menu != NULL) menu_refresh(menu);
  }

  wins_refresh(screen->wins, screen->win_count);
}

static void screen_wins_resize(screen_t* screen, int xmax, int ymax)
{
  int x = xmax / 2;
  int y = ymax / 2;

  screen_win_confirm_resize(screen, "exit", x, y, 24);

  screen_win_text_resize(screen, "size", x, y, 104, 26);

  screen_win_text_resize(screen, "info", x, y, 40, -1);
}

static void screen_menus_resize(screen_t* screen, int xmax, int ymax)
{
  for(int index = 0; index < screen->menu_count; index++)
  {
    menu_resize(screen->menus[index], xmax, ymax);
  }
}

void screen_resize(screen_t* screen)
{
  int xmax = getmaxx(stdscr);
  int ymax = getmaxy(stdscr);

  screen_menus_resize(screen, xmax, ymax);

  screen_wins_resize(screen, xmax, ymax);
}

static void screen_menus_create(screen_t* screen, int xmax, int ymax)
{
  screen_menu_dbs_create(screen, "dbs", xmax, ymax);

  screen_menu_psw_create(screen, "psw", xmax, ymax);

  screen_menu_db_create(screen, "db", xmax, ymax);
}

void screen_win_exit_key_handler(win_head_t* win_head, int key)
{
  if(win_head == NULL || win_head->type != WIN_CONFIRM) return;

  win_confirm_key_handler(win_head, key);

  win_confirm_t* win = (win_confirm_t*) win_head;

  switch(key)
  {
    case KEY_ENTER:
      if(win->answer == true) win_head->screen->running = false;
  
      win_head->active = false;
      break;
  }
}

static void screen_wins_create(screen_t* screen, int xmax, int ymax)
{
  int x = xmax / 2;
  int y = ymax / 2;

  screen_win_confirm_create(screen, "exit", x, y, 24, "Do you want to exit?", "Yes", "No", false, screen_win_exit_key_handler);

  // screen_win_text_create(screen, "size", x, y, 104, 26, "Info", "Resize the terminal to match this window", false, pop_text_key_handler);

  // screen_win_text_create(screen, "info", x, y, 40, -1, NULL, NULL, false, pop_text_key_handler);
}

screen_t* screen_create(void)
{
  screen_t* screen = malloc(sizeof(screen_t));

  initscr();
  noecho();
  raw();
  keypad(stdscr, TRUE);

  int xmax = getmaxx(stdscr);
  int ymax = getmaxy(stdscr);

  // screen_menus_create(screen, xmax, ymax);

  screen_wins_create(screen, xmax, ymax);

  screen->running = true;

  return screen;
}

void screen_free(screen_t* screen)
{
  if(screen->menus != NULL)
  {
    menus_free(screen->menus, screen->menu_count);

    free(screen->menus);
  }

  if(screen->wins != NULL)
  {
    wins_free(screen->wins, screen->win_count);

    free(screen->wins);
  }

  endwin();
}

void screen_name_win_focus_set(screen_t* screen, char* win_name)
{
  wins_name_win_focus_set(screen->wins, screen->win_count, win_name);
}

void screen_base_key_handler(screen_t* screen, int key)
{
  switch(key)
  {
    case KEY_CTRLC:
      screen_name_win_focus_set(screen, "exit");
      break;

    case KEY_RESIZE:
      screen_resize(screen);
      break;
  }
}

void screen_key_handler(screen_t* screen, int key)
{
  screen_base_key_handler(screen, key);

  win_t* win = screen_active_win_get(screen);

  menu_t* menu = screen_menu_get(screen);

  if(win != NULL && win->key_handler)
  {
    win->key_handler(win, key);
  }
  else if(menu != NULL)
  {
    menu_key_handler(menu, key);
  }
}
