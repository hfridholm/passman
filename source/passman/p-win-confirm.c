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
static int win_confirm_height(int pmtlen, int w)
{
  return MAX((pmtlen / (w - 2)) + 5, 0);
}

/*
 * PARAMS
 * - int x | x-value center of window
 * - int y | y-value center of window
 * - int w | width of window
 */
void win_confirm_resize(win_confirm_t* win, int x, int y, int w)
{
  int h = win_confirm_height(win->prompt_len, w);

  win_head_resize((win_head_t*) win, x, y, w, h);
}

/*
 * Note: After you change the prompt, you have to resize the window
 */
void win_confirm_prompt_set(win_confirm_t* win, char* prompt)
{
  win->prompt = prompt;

  win->prompt_len = (prompt ? strlen(prompt) : 0);
}

/*
 * PARAMS
 * - int x | x-value center of window
 * - int y | y-value center of window
 * - int w | width of window
 *
 * RETURN (win_confirm_t* win)
 */
win_confirm_t* win_confirm_create(char* name, bool active, bool tab_ability, int x, int y, int w, char* prompt, char* text_yes, char* text_no, win_event_t* event)
{
  win_confirm_t* win = malloc(sizeof(win_confirm_t));

  win_confirm_prompt_set(win, prompt);

  win->text_yes     = text_yes;
  win->text_yes_len = (text_yes ? strlen(text_yes) : 0);

  win->text_no      = text_no;
  win->text_no_len  = (text_no ? strlen(text_no) : 0);

  int h = win_confirm_height(win->prompt_len, w);

  win->head = win_head_create(WIN_CONFIRM, name, active, tab_ability, x, y, w, h, event);

  return win;
}

/*
 *
 */
void win_confirm_free(win_confirm_t* win)
{
  if(win == NULL) return;

  win_head_free(win->head);

  free(win);
}

/*
 *
 */
static void win_confirm_prompt_print(win_confirm_t* win)
{
  if(!win->prompt || !win->prompt_len) return;

  WINDOW* window = win->head.window;

  if(window == NULL) return;

  int ymax = win->head.ymax;
  int xmax = win->head.xmax;

  int index = 0;
  for(int height = 0; height < ymax - 4; height++)
  {
    // 1. Shift prompt to center of window
    if(height == (ymax - 5))
    {
      int line_len = MIN(xmax - 2, win->prompt_len - index);

      int xshift = (xmax - line_len) / 2;

      wmove(window, 1 + height, xshift);
    }
    else wmove(window, 1 + height, 1);

    // 2. Print the current line of text
    for(int width = 0; width < xmax - 2; width++)
    {
      index = (height * (xmax - 2) + width);

      if(index >= win->prompt_len) break; 

      waddch(window, win->prompt[index]);
    }
  }
}

/*
 *
 */
static void win_confirm_yesno_print(win_confirm_t* win)
{
  if(!win->text_yes || !win->text_yes_len) return;

  if(!win->text_no || !win->text_no_len) return;

  WINDOW* window = win->head.window;

  if(window == NULL) return;

  int ymax = win->head.ymax;
  int xmax = win->head.xmax;

  if(win->answer == true) wattron(window, A_REVERSE);

  int lenght = win->text_yes_len + 1 + win->text_no_len;

  int shift = (xmax - lenght) / 2;

  wmove(window, ymax - 2, shift);

  wprintw(window, "%s", win->text_yes);
  
  wattroff(window, A_REVERSE);

  waddch(window, ' ');

  if(win->answer == false) wattron(window, A_REVERSE);

  wprintw(window, "%s", win->text_no);

  wattroff(window, A_REVERSE);
}

/*
 *
 */
void win_confirm_refresh(win_confirm_t* win)
{
  if(!win->head.active) return;

  win_head_clean((win_head_t*) win);

  WINDOW* window = win->head.window;

  if(window == NULL) return;

  win_border_print((win_t*) win);

  win_confirm_prompt_print(win);

  win_confirm_yesno_print(win);

  wrefresh(window);
}

/*
 *
 */
int win_confirm_event(win_head_t* win_head, int key)
{
  if(win_head == NULL || win_head->type != WIN_CONFIRM) return 0;

  win_confirm_t* win = (win_confirm_t*) win_head;

  switch(key)
  {
    case 'l':
      win->answer = false;
      return 1;

    case 'h':
      win->answer = true;
      return 2;

    default:
      return 0;
  }
}

int pop_confirm_event(win_head_t* win_head, int key)
{
  if(win_head == NULL || win_head->type != WIN_CONFIRM) return 0;

  if(win_confirm_event(win_head, key)) return 1;

  switch(key)
  {
    case KEY_ENTR:
      win_head->active = false;
      return 2;
    
    default:
      return 0;
  }
}

win_confirm_t* wins_name_win_confirm_get(win_t** wins, int count, char* name)
{
  win_t* win = wins_name_win_get(wins, count, name);

  if(!win || win->type != WIN_CONFIRM) return NULL;

  return (win_confirm_t*) win;
}
