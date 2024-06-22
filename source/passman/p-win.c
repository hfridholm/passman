#include "../passman.h"

/*
 * PARAMS
 * - win_t* win |
 * - int x      | x-value center of win
 * - int y      | y-value center of win
 * - int w      | width of win
 * - int h      | height of win
 *
 * Note: Maybe return win_t* win
 */
void win_head_resize(win_head_t* win, int x, int y, int w, int h)
{
  wresize(win->window, h, w);

  mvwin(win->window, y - (h / 2), x - (w / 2));

  win->xmax = w;
  win->ymax = h;
}

/*
 * PARAMS
 * - int x | x-value center of win
 * - int y | y-value center of win
 * - int w | width of win
 * - int h | height of win
 *
 * RETURN (win_t* win)
 */
win_head_t win_head_create(win_type_t type, char* name, bool active, bool tab_ability, int x, int y, int w, int h, win_event_t* event)
{
  win_head_t win;

  win.type = type;
  win.name = name;

  win.window = newwin(h, w, y - (h / 2), x - (w / 2));
  
  win.xmax = w;
  win.ymax = h;

  keypad(win.window, TRUE);

  win.active      = active;
  win.tab_ability = tab_ability;

  win.menu   = NULL;
  win.screen = NULL;

  win.event = event;

  return win;
}

void win_head_clean(win_head_t* win)
{
  for(int y = 0; y < win->ymax; y++)
  {
    wmove(win->window, y, 0);

    for(int x = 0; x < win->xmax; x++)
    {
      waddch(win->window, ' ');
    }
  }
}

void win_head_free(win_head_t win)
{
  if(win.window != NULL)
  {
    wclear(win.window);

    wrefresh(win.window);

    delwin(win.window);
  }
}

void win_refresh(win_t* win)
{
  curs_set(0);

  switch(win->type)
  {
    case WIN_CONFIRM:
      win_confirm_refresh((win_confirm_t*) win);
      break;

    case WIN_INPUT:
      win_input_refresh((win_input_t*) win);
      break;

    case WIN_TEXT:
      win_text_refresh((win_text_t*) win);
      break;

    case WIN_LIST:
      win_list_refresh((win_list_t*) win);
      break;

    default:
      break;
  }
}

void win_free(win_t* win)
{
  switch(win->type)
  {
    case WIN_CONFIRM:
      win_confirm_free((win_confirm_t*) win);
      break;

    case WIN_INPUT:
      win_input_free((win_input_t*) win);
      break;

    case WIN_TEXT:
      win_text_free((win_text_t*) win);
      break;

    case WIN_LIST:
      win_list_free((win_list_t*) win);
      break;

    default:
      break;
  }
}

void wins_free(win_t** wins, int count)
{
  for(int index = 0; index < count; index++)
  {
    win_free(wins[index]);
  }
}

void wins_refresh(win_t** wins, int count)
{
  for(int index = count; index-- > 0;)
  {
    win_t* win = wins[index];

    if(win->active) win_refresh(wins[index]);
  }
}

win_t* wins_name_win_get(win_t** wins, int count, char* name)
{
  if(!wins) return NULL;

  for(int index = 0; index < count; index++)
  {
    win_t* win = wins[index];

    if(!win) continue;

    if(strcmp(win->name, name) == 0) return win;
  }
  return NULL;
}

int wins_name_win_index(win_t** wins, int count, char* name)
{
  if(!wins) return -1;

  for(int index = 0; index < count; index++)
  {
    win_t* win = wins[index];

    if(!win) continue;

    if(strcmp(win->name, name) == 0) return index;
  }
  return -1;
}

win_t* wins_active_win_get(win_t** wins, int count)
{
  if(!wins) return NULL;

  for(int index = 0; index < count; index++)
  {
    win_t* win = wins[index];

    if(!win) continue;

    if(win->active) return win;
  }
  return NULL;
}

void wins_rotate(win_t** wins, int count, int turns)
{
  if(turns <= 0) return;

  int shift = turns % count;

  win_t* temp_wins[shift];

  for(int index = 0; index < shift; index++)
  {
    temp_wins[index] = wins[index];
  }

  for(int index = 0; index < (count - shift); index++)
  {
    wins[index] = wins[shift + index];
  }

  for(int index = 0; index < shift; index++)
  {
    wins[count - shift + index] = temp_wins[index];
  }
}

void wins_index_win_focus_set(win_t** wins, int count, int win_index)
{
  if(!wins || win_index < 0 || win_index >= count) return;

  win_t* temp_win = wins[win_index];

  for(int index = (win_index + 1); index-- > 1;)
  {
    wins[index] = wins[index - 1];
  }

  wins[0] = temp_win;

  wins[0]->active = true;
}

void wins_name_win_focus_set(win_t** wins, int count, char* name)
{
  int index = wins_name_win_index(wins, count, name);

  wins_index_win_focus_set(wins, count, index);
}

void win_border_print(win_t* win)
{
  if(!win || !win->window) return;

  WINDOW* window = win->window;

  if(win_is_active_win(win))
  {
    wattron(window, COLOR_PAIR(TEXT_RED_PAIR));

    box(window, 0, 0);

    wattroff(window, COLOR_PAIR(TEXT_RED_PAIR));
  }
  else box(window, 0, 0);
}
