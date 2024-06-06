#include "../passman.h"

void screen_refresh(screen_t* screen)
{
  // clear();
  refresh();

  if(screen->menu_index >= 0 && screen->menu_index < screen->menu_count)
  {
    menu_refresh(screen->menus[screen->menu_index]);
  }

  if(screen->pop_index >= 0 && screen->pop_index < screen->pop_count)
  {
    win_refresh(screen->pops[screen->pop_index]);
  }
}

static void screen_pops_resize(screen_t* screen, int xmax, int ymax)
{
  int x = xmax / 2;
  int y = ymax / 2;

  screen_pop_confirm_resize(screen, "exit", x, y, 24);

  screen_pop_text_resize(screen, "size", x, y, 104, 26);

  screen_pop_text_resize(screen, "info", x, y, 40, -1);
}

void screen_resize(screen_t* screen)
{
  int xmax = getmaxx(stdscr);
  int ymax = getmaxy(stdscr);

  menus_resize(screen, xmax, ymax);

  screen_pops_resize(screen, xmax, ymax);
}

static void screen_menus_create(screen_t* screen, int xmax, int ymax)
{
  screen_menu_dbs_create(screen, "dbs", xmax, ymax);

  screen_menu_psw_create(screen, "psw", xmax, ymax);

  screen_mneu_db_create(screen, "db", xmax, ymax);
}

static void screen_pops_create(screen_t* screen, int xmax, int ymax)
{
  int x = xmax / 2;
  int y = ymax / 2;

  screen_pop_confirm_create(screen, "exit", x, y, 24, "Do you want to exit?", "Yes", "No", false, screen_pop_exit_key_handler);

  screen_pop_text_create(screen, "size", x, y, 104, 26, "Info", "Resize the terminal to match this window", false, screen_pop_text_key_handler);

  screen_pop_text_create(screen, "info", x, y, 40, -1, NULL, NULL, false, screen_pop_text_key_handler);
}

screen_t* screen_create(void)
{
  screen_t* screen = malloc(sizeof(screen_t));

  screen->running = true;

  initscr();
  noecho();
  raw();
  keypad(stdscr, TRUE);

  int xmax = getmaxx(stdscr);
  int ymax = getmaxy(stdscr);

  screen_menus_create(screen, xmax, ymax);

  screen_pops_create(screen, xmax, ymax);

  return screen;
}

void screen_free(screen_t* screen)
{
  menus_free(screen->menus, screen->menu_count);

  free(screen->menus);

  wins_free(screen->pops, screen->pop_count);

  free(screen->pops);

  endwin();
}

void screen_pop_exit_key_handler(screen_t* screen, win_t* pop, int key)
{
  if(pop == NULL) return;

  win_confirm_key_handler(pop, key);

  switch(key)
  {
    case KEY_ENTER:
      if(pop->answer == true) screen->running = false;
  
      pop->head->active = false;
      break;
  }
}

void screen_pop_text_key_handler(screen_t* screen, win_t* pop, int key)
{
  if(pop == NULL) return;

  win_text_key_handler(pop, key);

  switch(key)
  {
    case KEY_ENTER:
      pop->head->active = false;
      break;
  }
}

void screen_key_handler(screen_t* screen, int key)
{
  switch(key)
  {
    case KEY_CTRLC:
      screen->pop_index = wins_name_win_index(screen->pops, screen->pop_count, "exit");
      break;

    case KEY_RESIZE:
      screen_resize(screen);
      break;
  }
}
