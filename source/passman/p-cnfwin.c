#include "../passman.h"

/*
 * Calculate the height of the window depending on prompt and width
 *
 * PARAMS
 * - int pmtlen | Length of prompt
 * - int w      | The width of the window
 *
 * RETURN (int h)
 */
static int cnfwin_height(int pmtlen, int w)
{
  return MAX((pmtlen / (w - 2)) + 5, 0);
}

/*
 * PARAMS
 * - int x | x-value center of window
 * - int y | y-value center of window
 * - int w | width of window
 */
void cnfwin_resize(cnfwin_t* cnfwin, int x, int y, int w)
{
  int h = cnfwin_height(cnfwin->pmtlen, w);

  window_resize(cnfwin->window, x, y, w, h);
}

/*
 * Note: After you change the prompt, you have to resize the window
 */
void cnfwin_prompt_set(cnfwin_t* cnfwin, char* prompt)
{
  cnfwin->prompt = prompt;

  cnfwin->pmtlen = (prompt ? strlen(prompt) : 0);
}

/*
 * PARAMS
 * - int x | x-value center of window
 * - int y | y-value center of window
 * - int w | width of window
 *
 * RETURN (cnfwin_t* cnfwin)
 */
cnfwin_t* cnfwin_create(int x, int y, int w, char* prompt, char* ytext, char* ntext, bool active)
{
  cnfwin_t* cnfwin = malloc(sizeof(cnfwin_t));

  cnfwin_prompt_set(cnfwin, prompt);

  cnfwin->ytext  = ytext;
  cnfwin->ytxlen = (ytext ? strlen(ytext) : 0);

  cnfwin->ntext  = ntext;
  cnfwin->ntxlen = (ntext ? strlen(ntext) : 0);

  int h = cnfwin_height(cnfwin->pmtlen, w);

  cnfwin->window = window_create(x, y, w, h, active);

  return cnfwin;
}

/*
 *
 */
void cnfwin_free(cnfwin_t* cnfwin)
{
  if(cnfwin == NULL) return;

  window_free(cnfwin->window);

  free(cnfwin);
}

/*
 *
 */
static void cnfwin_prompt_print(cnfwin_t* cnfwin)
{
  if(!cnfwin->prompt || !cnfwin->pmtlen) return;

  WINDOW* window = cnfwin->window->window;

  int ymax = cnfwin->window->ymax;
  int xmax = cnfwin->window->xmax;

  int index = 0;
  for(int height = 0; height < ymax - 4; height++)
  {
    // 1. Shift prompt to center of window
    if(height == (ymax - 5))
    {
      int linlen = MIN(xmax - 2, cnfwin->pmtlen - index);

      int xshift = (xmax - linlen) / 2;

      wmove(window, 1 + height, xshift);
    }
    else wmove(window, 1 + height, 1);

    // 2. Print the current line of text
    for(int width = 0; width < xmax - 2; width++)
    {
      index = (height * (xmax - 2) + width);

      if(index >= cnfwin->pmtlen) break; 

      waddch(window, cnfwin->prompt[index]);
    }
  }
}

/*
 *
 */
static void cnfwin_yesno_print(cnfwin_t* cnfwin)
{
  if(!cnfwin->ytext || !cnfwin->ytxlen) return;

  if(!cnfwin->ntext || !cnfwin->ntxlen) return;

  WINDOW* window = cnfwin->window->window;

  int ymax = cnfwin->window->ymax;
  int xmax = cnfwin->window->xmax;

  int alength = cnfwin->ytxlen + 1 + cnfwin->ntxlen;

  int ashift = (xmax - alength) / 2;

  if(cnfwin->answer == true) wattron(window, A_REVERSE);

  wmove(window, ymax - 2, ashift);

  wprintw(window, "%s", cnfwin->ytext);
  
  wattroff(window, A_REVERSE);

  waddch(window, ' ');

  if(cnfwin->answer == false) wattron(window, A_REVERSE);

  wprintw(window, "%s", cnfwin->ntext);

  wattroff(window, A_REVERSE);
}

/*
 *
 */
void cnfwin_refresh(cnfwin_t* cnfwin)
{
  if(!cnfwin->window->active) return;

  window_clean(cnfwin->window);

  WINDOW* window = cnfwin->window->window;

  box(window, 0, 0);

  cnfwin_prompt_print(cnfwin);

  cnfwin_yesno_print(cnfwin);

  wrefresh(window);
}

/*
 *
 */
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
  if(!cnfwin->window->active) return;

  screen_refresh();

  int key;
  while(running && (key = wgetch(cnfwin->window->window)))
  {
    screen_key_handler(key);

    cnfwin_key_handler(cnfwin, key);

    if(key_handler) key_handler(key);
    
    else if(key == KEY_ENTR) break;

    screen_refresh();
  }
}

/*
 * Open a popup, input and then close the popup again
 *
 * PARAMS
 * - cnfwin_t* cnfpop         | The confirm popup to input to
 * - void (*key_handler)(int) | A possible custom key handler
 */
void cnfpop_input(cnfwin_t* cnfpop, void (*key_handler)(int))
{
  cnfpop->window->active = true;

  cnfwin_input(cnfpop, key_handler);

  cnfpop->window->active = false;
}
