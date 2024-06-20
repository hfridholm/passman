#include "../passman.h"

void screen_refresh(screen_t* screen)
{
  curs_set(0);

  clear();
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

void screen_win_exit_event(win_head_t* win_head, int key)
{
  if(win_head == NULL || win_head->type != WIN_CONFIRM) return;

  win_confirm_event(win_head, key);

  win_confirm_t* win = (win_confirm_t*) win_head;

  switch(key)
  {
    case KEY_ENTR:
      if(win->answer == true)
      {
        if(win_head->screen)
        {
          win_head->screen->running = false;
        }
      }
  
      win_head->active = false;
      break;
  }
}

static void screen_wins_create(screen_t* screen, int xmax, int ymax)
{
  int x = xmax / 2;
  int y = ymax / 2;

  screen_win_confirm_create(screen, "exit", false, false,
    x, y, 24, "Do you want to exit?", "Yes", "No", screen_win_exit_event);

  screen_win_text_create(screen, "size", false, false,
    x, y, 104, 26, "Info", "Resize the terminal to match this window", pop_text_event);

  screen_win_text_create(screen, "info", false, false,
    x, y, 40, -1, NULL, NULL, pop_text_event);
}

screen_t* screen_create(void)
{
  screen_t* screen = malloc(sizeof(screen_t));

  screen->wins = NULL;
  screen->win_count = 0;

  screen->menus = NULL;
  screen->menu_count = 0;
  screen->menu_index = -1;

  initscr();
  noecho();
  raw();
  keypad(stdscr, TRUE);

  int xmax = getmaxx(stdscr);
  int ymax = getmaxy(stdscr);

  screen_menus_create(screen, xmax, ymax);

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

  free(screen);

  endwin();
}

void screen_base_event(screen_t* screen, int key)
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

void screen_event(screen_t* screen, int key)
{
  screen_base_event(screen, key);

  win_t* win = screen_active_win_get(screen);

  menu_t* menu = screen_menu_get(screen);

  if(win && win->event)
  {
    win->event(win, key);
  }
  else if(menu)
  {
    menu_event(menu, key);
  }
}

/*
 *
 */
void screen_text_popup(screen_t* screen, char* title, char* text)
{
  win_text_t* win = screen_name_win_text_get(screen, "info");

  win_text_title_set(win, title);

  win_text_text_set(win, text);

  screen_name_win_focus_set(screen, "info");
}
