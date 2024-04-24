#include "../passman.h"

/*
 * PARAMS
 * - int x | x-value center of window
 * - int y | y-value center of window
 * - int w | width of window
 */
void infwin_resize(infwin_t* infwin, int x, int y, int w)
{
  int h = MAX((strlen(infwin->text) / (w - 2)) + 3, 0);

  window_resize(infwin->window, x, y, w, h);
}

/*
 * PARAMS
 * - int x | x-value center of window
 * - int y | y-value center of window
 * - int w | width of window
 *
 * RETURN (infwin_t* infwin)
 */
infwin_t* infwin_create(int x, int y, int w, char* title, char* text)
{
  infwin_t* infwin = malloc(sizeof(infwin_t));

  infwin->window = window_create(1, 1, 1, 1);

  infwin->title = title;
  infwin->text  = text;

  infwin_resize(infwin, x, y, w);

  return infwin;
}

void infwin_free(infwin_t* infwin)
{
  if(infwin == NULL) return;

  window_free(infwin->window);

  free(infwin);
}

void infwin_refresh(infwin_t* infwin)
{
  if(!infwin->window->active) return;

  window_clean(infwin->window);

  WINDOW* window = infwin->window->window;

  int ymax = infwin->window->ymax;
  int xmax = infwin->window->xmax;

  curs_set(0);

  box(window, 0, 0);

  int shift = (xmax - strlen(infwin->title)) / 2;

  mvwprintw(window, 0, shift, "%s", infwin->title);

  int length = strlen(infwin->text);

  for(int height = 0; height < ymax - 2; height++)
  {
    wmove(window, 1 + height, 1);

    for(int width = 0; width < xmax - 2; width++)
    {
      int index = (height * (xmax - 2) + width);

      if(index >= length) break; 

      waddch(window, infwin->text[index]);
    }
  }

  wrefresh(window);
}

/*
 * PARAMS
 * - infwin_t* infwin         | The info window to input to
 * - void (*key_handler)(int) | A possible custom key handler
 */
void infwin_input(infwin_t* infwin, void (*key_handler)(int))
{
  screen_refresh();

  WINDOW* window = infwin->window->window;

  int key;
  while(running && (key = wgetch(window)))
  {
    screen_key_handler(key);

    if(key_handler) key_handler(key);
    
    else if(key == KEY_ENTR) break;

    screen_refresh();
  }
}
