#include "../passman.h"

void lstwin_resize(lstwin_t* lstwin, int x, int y, int w, int h)
{
  window_resize(lstwin->window, x, y, w, h);

  // Clamp the marked item to the floor
  if(lstwin->index >= (lstwin->scroll + (h - 2)))
  {
    lstwin->scroll = lstwin->index - (h - 2) + 1;
  }

  // Clamp the marked item to the ceiling
  if(lstwin->index < lstwin->scroll)
  {
    lstwin->scroll = lstwin->index;
  }

  // Prevent empty space when items can occupy the space
  if(lstwin->scroll + (h - 2) > lstwin->amount)
  {
    lstwin->scroll = MAX(0, lstwin->amount - (h - 2));
  }
}

lstwin_t* lstwin_create(int x, int y, int w, int h, char** items, int amount)
{
  lstwin_t* lstwin = malloc(sizeof(lstwin_t));

  lstwin->window = window_create(x, y, w, h);

  lstwin->items = items;
  lstwin->amount = amount;

  return lstwin;
}

void lstwin_free(lstwin_t* lstwin)
{
  if(lstwin == NULL) return;

  window_free(lstwin->window);

  free(lstwin);
}

void lstwin_refresh(lstwin_t* lstwin)
{
  window_clean(lstwin->window);

  WINDOW* window = lstwin->window->window;

  int ymax = lstwin->window->ymax;

  int amount = MIN(lstwin->amount, ymax - 2);

  for(int index = 0; index < amount; index++)
  {
    int pindex = index + lstwin->scroll;

    if(pindex == lstwin->index) wattron(window, A_REVERSE);

    mvwprintw(window, index + 1, 1, "%s", lstwin->items[pindex]);

    wattroff(window, A_REVERSE);
  }

  box(window, 0, 0);

  wrefresh(window);
}

int lstwin_scroll_down(lstwin_t* lstwin)
{
  int ymax = lstwin->window->ymax;

  if(lstwin->index >= lstwin->amount - 1) return 1;

  if((lstwin->index - lstwin->scroll) >= (ymax - 3))
  {
    lstwin->scroll++;
  }

  lstwin->index++;

  return 0;
}

int lstwin_scroll_up(lstwin_t* lstwin)
{
  if(lstwin->index <= lstwin->scroll && lstwin->scroll > 0)
  {
    lstwin->scroll--;
  }

  lstwin->index = MAX(lstwin->index - 1, 0);

  return 0;
}

void lstwin_key_handler(lstwin_t* lstwin, int key)
{
  switch(key)
  {
    case 'j':
      lstwin_scroll_down(lstwin);
      break;

    case 'k':
      lstwin_scroll_up(lstwin);
      break;
  }
}

/*
 * PARAMS
 * - lstwin_t* lstwin         | The list window to input to
 * - void (*key_handler)(int) | A possible custom key handler
 */
void lstwin_input(lstwin_t* lstwin, void (*key_handler)(int))
{
  screen_refresh();

  WINDOW* window = lstwin->window->window;

  int key;
  while(running && (key = wgetch(window)))
  {
    screen_key_handler(key);

    lstwin_key_handler(lstwin, key);

    if(key_handler) key_handler(key);
    
    else if(key == KEY_ENTR) break;

    screen_refresh();
  }
}
