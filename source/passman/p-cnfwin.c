#include "../passman.h"

/*
 * PARAMS
 * - int x | x-value center of window
 * - int y | y-value center of window
 * - int w | width of window
 */
void cnfwin_resize(cnfwin_t* cnfwin, int x, int y, int w)
{
  int h = (strlen(cnfwin->prompt) / (w - 2)) + 5;

  window_resize(cnfwin->window, x, y, w, h);
}

/*
 * PARAMS
 * - int x | x-value center of window
 * - int y | y-value center of window
 * - int w | width of window
 */
cnfwin_t* cnfwin_create(int x, int y, int w, char* prompt, char* ytext, char* ntext)
{
  cnfwin_t* cnfwin = malloc(sizeof(cnfwin_t));

  int h = MIN((strlen(prompt) / (w - 2)) + 5, 0);

  cnfwin->window = window_create(x, y, w, h);

  cnfwin->prompt = prompt;
  cnfwin->ytext  = ytext;
  cnfwin->ntext  = ntext;

  return cnfwin;
}

void cnfwin_free(cnfwin_t* cnfwin)
{
  if(cnfwin == NULL) return;

  window_free(cnfwin->window);

  free(cnfwin);
}

void cnfwin_refresh(cnfwin_t* cnfwin)
{
  window_clean(cnfwin->window);

  curs_set(0);

  WINDOW* window = cnfwin->window->window;

  int ymax = cnfwin->window->ymax;
  int xmax = cnfwin->window->xmax;

  box(window, 0, 0);

  int length = strlen(cnfwin->prompt);
  int index = 0;

  for(int height = 0; height < ymax - 4; height++)
  {
    if(height >= ymax - 5)
    {
      int rlength = strlen(cnfwin->prompt + index);
      int cshift = (xmax - rlength) / 2;

      wmove(window, 1 + height, cshift);
    }
    else wmove(window, 1 + height, 1);

    for(int width = 0; width < xmax - 2; width++)
    {
      index = (height * (xmax - 2) + width);

      if(index >= length) break; 

      waddch(window, cnfwin->prompt[index]);
    }
  }

  int ylength = strlen(cnfwin->ytext);
  int nlength = strlen(cnfwin->ntext);

  int alength = ylength + 1 + nlength;

  int ashift = (xmax - alength) / 2;

  if(cnfwin->answer == true) wattron(window, A_REVERSE);

  wmove(window, ymax - 2, ashift);

  wprintw(window, "%s", cnfwin->ytext);
  
  wattroff(window, A_REVERSE);

  waddch(window, ' ');

  if(cnfwin->answer == false) wattron(window, A_REVERSE);

  wprintw(window, "%s", cnfwin->ntext);

  wattroff(window, A_REVERSE);

  wrefresh(window);
}

void cnfwin_key_handler(cnfwin_t* cnfwin, int key)
{
  switch(key)
  {
    case 'l':
      cnfwin->answer = false;
      break;

    case 'h':
      cnfwin->answer = true;
      break;
  }
}

/*
 * PARAMS
 * - cnfwin_t* cnfwin         | The confirm window to input to
 * - void (*key_handler)(int) | A possible custom key handler
 */
void cnfwin_input(cnfwin_t* cnfwin, void (*key_handler)(int))
{
  screen_refresh();

  WINDOW* window = cnfwin->window->window;

  int key;
  while(running && (key = wgetch(window)))
  {
    screen_key_handler(key);

    cnfwin_key_handler(cnfwin, key);

    if(key_handler) key_handler(key);
    
    else if(key == KEY_ENTR) break;

    screen_refresh();
  }
}
