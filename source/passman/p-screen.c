#include "../passman.h"

void screen_refresh(screen_t* screen)
{
  curs_set(0);

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

  screen_menu_act_create(screen, "act", xmax, ymax);
}

/*
 *
 */
int screen_win_exit_event(win_head_t* win_head, int key)
{
  if(win_head == NULL || win_head->type != WIN_CONFIRM) return 0;

  if(win_confirm_event(win_head, key)) return 1;

  win_confirm_t* win = (win_confirm_t*) win_head;

  switch(key)
  {
    case KEY_ENTR:
      if(win->answer == true && win_head->screen)
      {
        win_head->screen->running = false;
      }
  
      win_head->active = false;
      return 2;

    default:
      return 0;
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

  if(start_color() == ERR || !has_colors() || !can_change_color())
  {
    endwin();
  
    return NULL;
  }
  
  init_pair(TEXT_RED_PAIR, COLOR_RED, 0);

  screen->dbase = malloc(sizeof(dbase_t));

  int xmax = getmaxx(stdscr);
  int ymax = getmaxy(stdscr);

  screen_menus_create(screen, xmax, ymax);

  screen_wins_create(screen, xmax, ymax);

  screen->running = true;

  return screen;
}

void screen_free(screen_t* screen)
{
  if(!screen) return;

  if(screen->menus)
  {
    menus_free(screen->menus, screen->menu_count);

    free(screen->menus);
  }

  if(screen->wins)
  {
    wins_free(screen->wins, screen->win_count);

    free(screen->wins);
  }

  if(screen->dbase) free(screen->dbase);

  free(screen);

  endwin();
}

/*
 *
 */
int screen_base_event(screen_t* screen, int key)
{
  switch(key)
  {
    case KEY_CTRLC:
      screen_win_focus_set(screen, "exit");
      return 1;

    case KEY_RESIZE:
      screen_resize(screen);
      return 2;

    default:
      return 0;
  }
}

/*
 *
 */
int screen_event(screen_t* screen, int key)
{
  win_t* win = screen_active_win_get(screen);

  menu_t* menu = screen_active_menu_get(screen);

  // 1. Interact with screen popup
  if(win && win->event)
  {
    if(win->event(win, key)) return 1;
  }
  // 2. Interact with menu
  else if(menu && menu_event(menu, key)) return 2;

  // Always handle screen events
  if(screen_base_event(screen, key)) return 3;

  else return 0;
}

/*
 *
 */
void screen_text_popup(screen_t* screen, char* title, char* text)
{
  win_text_t* win = screen_win_text_get(screen, "info");

  win_text_title_set(win, title);

  win_text_text_set(win, text);

  screen_win_focus_set(screen, "info");
}

win_t* active_win_get(screen_t* screen)
{
  win_t* screen_win = screen_active_win_get(screen);

  if(screen_win) return screen_win;

  menu_t* menu = screen_active_menu_get(screen);

  if(menu) return menu_active_win_get(menu);

  return NULL;
}

bool win_is_active_win(win_t* win)
{
  if(!win) return false;

  screen_t* screen = win->screen;

  if(!screen) return false;

  win_t* active_win = active_win_get(screen);

  if(!active_win) return false;

  return (strcmp(active_win->name, win->name) == 0);
}
