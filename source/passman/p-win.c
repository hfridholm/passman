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
  wresize(win->win, h, w);

  mvwin(win->win, y - (h / 2), x - (w / 2));

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
win_t* win_head_create(int x, int y, int w, int h, bool active)
{
  win_t* win = malloc(sizeof(win_t));

  win->win = newwin(h, w, y - (h / 2), x - (w / 2));
  
  win->xmax = w;
  win->ymax = h;

  keypad(win->win, TRUE);

  win->active = active;

  return win;
}

void win_head_clean(win_head_t* win)
{
  for(int y = 0; y < win->ymax; y++)
  {
    wmove(win->win, y, 0);

    for(int x = 0; x < win->xmax; x++)
    {
      waddch(win->win, ' ');
    }
  }
}

void win_head_free(win_head_t* win)
{
  if(win == NULL) return;

  wclear(win->win);

  wrefresh(win->win);

  delwin(win->win);

  free(win);
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
