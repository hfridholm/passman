#include "../passman.h"

void screen_win_add(screen_t* screen, win_t* win)
{
  if(screen->wins == NULL)
  {
    screen->wins = malloc(sizeof(win_t*));
  }
  else screen->wins = realloc(screen->wins, sizeof(win_t*) * (screen->win_count + 1));

  screen->wins[screen->win_count++] = win;

  win->screen = screen;
}

win_t* screen_active_win_get(screen_t* screen)
{
  return wins_active_win_get(screen->wins, screen->win_count);
}

void screen_win_input_create(screen_t* screen, char* name, bool active, bool tab_ability, int x, int y, int w, char* buffer, size_t size, char* title, bool secret, key_handler_t* key_handler)
{
  win_input_t* win = win_input_create(name, active, tab_ability, x, y, w, buffer, size, title, secret, key_handler);

  screen_win_add(screen, (win_t*) win);
}

void screen_win_confirm_create(screen_t* screen, char* name, bool active, bool tab_ability, int x, int y, int w, char* prompt, char* text_yes, char* text_no, key_handler_t* key_handler)
{
  win_confirm_t* win = win_confirm_create(name, active, tab_ability, x, y, w, prompt, text_yes, text_no, key_handler);

  screen_win_add(screen, (win_t*) win);
}

void screen_win_text_create(screen_t* screen, char* name, bool active, bool tab_ability, int x, int y, int w, int h, char* title, char* text, key_handler_t* key_handler)
{
  win_text_t* win = win_text_create(name, active, tab_ability, x, y, w, h, title, text, key_handler);

  screen_win_add(screen, (win_t*) win);
}

void screen_win_list_create(screen_t* screen, char* name, bool active, bool tab_ability, int x, int y, int w, int h, key_handler_t* key_handler)
{
  win_list_t* win = win_list_create(name, active, tab_ability, x, y, w, h, key_handler);

  screen_win_add(screen, (win_t*) win);
}

int screen_win_confirm_resize(screen_t* screen, char* name, int x, int y, int w)
{
  win_t* win = wins_name_win_get(screen->wins, screen->win_count, name);

  if(win == NULL) return 1;

  if(win->type != WIN_CONFIRM) return 2;

  win_confirm_resize((win_confirm_t*) win, x, y, w);

  return 0;
}

int screen_win_text_resize(screen_t* screen, char* name, int x, int y, int w, int h)
{
  win_t* win = wins_name_win_get(screen->wins, screen->win_count, name);

  if(win == NULL) return 1;

  if(win->type != WIN_TEXT) return 2;

  win_text_resize((win_text_t*) win, x, y, w, h);

  return 0;
}
