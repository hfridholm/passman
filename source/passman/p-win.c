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
win_head_t win_head_create(win_type_t type, char* name, int x, int y, int w, int h, bool active, key_handler_t* key_handler)
{
  win_head_t win;

  win.type = type;

  win.name = name;

  win.window = newwin(h, w, y - (h / 2), x - (w / 2));
  
  win.xmax = w;
  win.ymax = h;

  keypad(win.window, TRUE);

  win.active = active;

  win.key_handler = key_handler;

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
  if(win.window)
  {
    wclear(win.window);

    wrefresh(win.window);

    delwin(win.window);
  }
}

void win_refresh(win_t* win)
{
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
  for(int index = 0; index < count; index++)
  {
    win_t* win = wins[index];

    if(win->active) win_refresh(wins[index]);
  }
}

win_t* wins_name_win_get(win_t** wins, int count, char* name)
{
  for(int index = 0; index < count; index++)
  {
    win_t* win = wins[index];

    if(strcmp(win->name, name) == 0) return win;
  }
  return NULL;
}

int wins_name_win_index(win_t** wins, int count, char* name)
{
  for(int index = 0; index < count; index++)
  {
    win_t* win = wins[index];

    if(strcmp(win->name, name) == 0) return index;
  }
  return -1;
}

win_t* wins_active_win_get(win_t** wins, int count)
{
  for(int index = 0; index < count; index++)
  {
    win_t* win = wins[index];

    if(win->active) return win;
  }
  return NULL;
}

int wins_next_active_win_index(win_t** wins, int count)
{
  int active_num = 0;

  for(int index = 0; index < count; index++)
  {
    win_t* win = wins[index];

    if(win->active) active_num++;

    if(active_num == 2) return index;
  }
  return -1;
}

void wins_rotate(win_t** wins, int count, int turns)
{
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
  if(win_index < 0 || win_index >= count) return;

  win_t* temp_win = wins[win_index];

  for(int index = 1; index <= win_index; index++)
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
