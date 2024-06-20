#include "../passman.h"

void screen_win_add(screen_t* screen, win_t* win)
{
  screen->wins = realloc(screen->wins, sizeof(win_t*) * (screen->win_count + 1));

  screen->wins[screen->win_count++] = win;

  win->screen = screen;
}

win_t* screen_active_win_get(screen_t* screen)
{
  return wins_active_win_get(screen->wins, screen->win_count);
}

void screen_win_input_create(screen_t* screen, char* name, bool active, bool tab_ability, int x, int y, int w, char* buffer, size_t size, char* title, bool secret, win_event_t* event)
{
  win_input_t* win = win_input_create(name, active, tab_ability, x, y, w, buffer, size, title, secret, event);

  screen_win_add(screen, (win_t*) win);
}

void screen_win_confirm_create(screen_t* screen, char* name, bool active, bool tab_ability, int x, int y, int w, char* prompt, char* text_yes, char* text_no, win_event_t* event)
{
  win_confirm_t* win = win_confirm_create(name, active, tab_ability, x, y, w, prompt, text_yes, text_no, event);

  screen_win_add(screen, (win_t*) win);
}

void screen_win_text_create(screen_t* screen, char* name, bool active, bool tab_ability, int x, int y, int w, int h, char* title, char* text, win_event_t* event)
{
  win_text_t* win = win_text_create(name, active, tab_ability, x, y, w, h, title, text, event);

  screen_win_add(screen, (win_t*) win);
}

void screen_win_list_create(screen_t* screen, char* name, bool active, bool tab_ability, int x, int y, int w, int h, win_event_t* event)
{
  win_list_t* win = win_list_create(name, active, tab_ability, x, y, w, h, event);

  screen_win_add(screen, (win_t*) win);
}

win_confirm_t* screen_name_win_confirm_get(screen_t* screen, char* win_name)
{
  return wins_name_win_confirm_get(screen->wins, screen->win_count, win_name);
}

win_text_t* screen_name_win_text_get(screen_t* screen, char* win_name)
{
  return wins_name_win_text_get(screen->wins, screen->win_count, win_name);
}

void screen_win_confirm_resize(screen_t* screen, char* name, int x, int y, int w)
{
  win_confirm_t* win = screen_name_win_confirm_get(screen, name);

  win_confirm_resize(win, x, y, w);
}

void screen_win_text_resize(screen_t* screen, char* name, int x, int y, int w, int h)
{
  win_text_t* win = screen_name_win_text_get(screen, name);

  win_text_resize(win, x, y, w, h);
}

void screen_name_win_focus_set(screen_t* screen, char* win_name)
{
  wins_name_win_focus_set(screen->wins, screen->win_count, win_name);
}
