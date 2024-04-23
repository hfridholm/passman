#include "../passman.h"

/*
 * PARAMS
 * - int x | x-value center of window
 * - int y | y-value center of window
 * - int w | width of window
 */
void inpwin_resize(inpwin_t* inpwin, int x, int y, int w)
{
  window_resize(inpwin->window, x, y, w, 3);
}

/*
 * PARAMS
 * - int x | x-value center of window
 * - int y | y-value center of window
 * - int w | width of window
 *
 * RETURN (inpwin_t* inpwin)
 */
inpwin_t* inpwin_create(int x, int y, int w, char* buffer, size_t size, bool secret)
{
  inpwin_t* inpwin = malloc(sizeof(inpwin_t));

  inpwin->window = window_create(x, y, w, 3);

  inpwin->buffer = buffer;
  inpwin->msize  = size;

  inpwin->secret = secret;
  inpwin->hidden = secret;

  return inpwin;
}

void inpwin_free(inpwin_t* inpwin)
{
  if(inpwin == NULL) return;

  window_free(inpwin->window);

  free(inpwin);
}

/*
 * Refresh the content of the buffer being shown
 */
void inpwin_refresh(inpwin_t* inpwin)
{
  window_clean(inpwin->window);

  WINDOW* window = inpwin->window->window;

  int xmax = inpwin->window->xmax;

  wmove(window, 1, 1);

  // The amount of characters to print
  int amount = MIN(inpwin->length, xmax - 3);

  for(int index = 0; index < amount; index++)
  {
    int pindex = inpwin->scroll + index;

    bool hidden = (inpwin->hidden && inpwin->secret);

    char symbol = (hidden ? '*' : inpwin->buffer[pindex]);

    if(symbol == '\0') symbol = ' ';

    waddch(window, symbol);
  }

  // Fill in the rest with empty space
  for(int index = amount; index < xmax - 3; index++)
  {
    waddch(window, ' ');
  }

  box(window, 0, 0);

  mvwprintw(window, 2, xmax - 8, "%03d/%03d",
            inpwin->length, inpwin->msize);

  wmove(window, 1, 1 + (inpwin->cursor - inpwin->scroll));

  wrefresh(window);
}

/*
 * RETURN (int status)
 * - 0 | Success!
 * - 1 | Symbol not supported
 * - 2 | Buffer is full
 */
int inpwin_symbol_add(inpwin_t* inpwin, char symbol)
{
  if(symbol < 32 || symbol > 126) return 1;

  if(inpwin->length >= inpwin->msize) return 2;

  int xmax = inpwin->window->xmax;

  // Shift characters forward to make room for new character
  for(int index = inpwin->length + 1; index-- > inpwin->cursor;)
  {
    inpwin->buffer[index] = inpwin->buffer[index - 1];
  }

  inpwin->buffer[inpwin->cursor++] = symbol;

  inpwin->length++;

  // The cursor is at the end of the input window
  if((inpwin->cursor - inpwin->scroll) >= xmax - 2)
  {
    inpwin->scroll++; // Scroll one more character
  }

  return 0; // Success!
}

int inpwin_symbol_del(inpwin_t* inpwin)
{
  if(inpwin->cursor <= 0) return 1;

  // Fill in the room left by the deleted character
  for(int index = inpwin->cursor - 1; index < inpwin->length; index++)
  {
    inpwin->buffer[index] = inpwin->buffer[index + 1];
  }

  // Remove the last character in the buffer
  inpwin->buffer[--inpwin->length] = '\0';
  inpwin->cursor--;

  return 0; // Success!
}

static void inpwin_scroll_right(inpwin_t* inpwin)
{
  int xmax = inpwin->window->xmax;

  // The cursor can not be further than the text itself
  inpwin->cursor = MIN(inpwin->cursor + 1, inpwin->length);

  // The cursor is at the end of the input window
  if((inpwin->cursor - inpwin->scroll) >= xmax - 2)
  {
    inpwin->scroll++; // Scroll one more character
  }
}

static void inpwin_scroll_left(inpwin_t* inpwin)
{
  // The cursor is at the end of the input window
  if(inpwin->cursor <= inpwin->scroll && inpwin->scroll > 0)
  {
    inpwin->scroll--; // Scroll back one character
  }

  inpwin->cursor = MAX(inpwin->cursor - 1, 0);
}

/*
 *
 */
void inpwin_key_handler(inpwin_t* inpwin, int key)
{
  switch(key)
  {
    case KEY_CTRLH:
      if(inpwin->secret) inpwin->hidden = !inpwin->hidden;
      break;

    case KEY_RIGHT:
      inpwin_scroll_right(inpwin);
      break;

    case KEY_LEFT:
      inpwin_scroll_left(inpwin);
      break;

    case KEY_BACKSPACE:
      inpwin_symbol_del(inpwin);
      break;
    
    default:
      inpwin_symbol_add(inpwin, key);
      break;
  }
}

/*
 * PARAMS
 * - inpwin_t* inpwin         | The input window to input to
 * - void (*key_handler)(int) | A possible custom key handler
 */
void inpwin_input(inpwin_t* inpwin, void (*key_handler)(int))
{
  screen_refresh();

  WINDOW* window = inpwin->window->window;

  int key;
  while(running && (key = wgetch(window)))
  {
    screen_key_handler(key);

    inpwin_key_handler(inpwin, key);

    if(key_handler) key_handler(key);
    
    else if(key == KEY_ENTR) break;

    screen_refresh();
  }
}
