#include "../passman.h"
#include "passman-intern.h"

inpwin_t* inpwin_create(int w, int y, int x, char* buffer, size_t size)
{
  int ymax = getmaxy(stdscr);

  if((y + 3) > ymax) return NULL; 


  inpwin_t* inpwin = malloc(sizeof(inpwin_t));

  inpwin->window = newwin(3, w, y, x);

  inpwin->xmax = getmaxx(inpwin->window);

  keypad(inpwin->window, TRUE);

  inpwin->buffer = buffer;
  inpwin->msize  = size;

  return inpwin;
}

void inpwin_free(inpwin_t* inpwin)
{
  if(inpwin == NULL) return;

  window_free(inpwin->window);

  free(inpwin);
}

inpwin_t* inpwin_center_create(window_t* parent, int y, int x, char* buffer, int size)
{
  int xmax = getmaxx(parent);

  int w = (xmax - x - x);
  if(w <= 0) return NULL;

  return inpwin_create(w, y, x, buffer, size);
}

/*
 * Refresh the content of the buffer being shown
 */
void inpwin_refresh(inpwin_t* inpwin, bool hidden)
{
  wmove(inpwin->window, 1, 1);

  // The amount of characters to print
  int amount = MIN(inpwin->length, inpwin->xmax - 3);

  for(int index = 0; index < amount; index++)
  {
    int pindex = inpwin->scroll + index;

    char symbol = (hidden ? '*' : inpwin->buffer[pindex]);

    if(symbol == '\0') symbol = ' ';

    waddch(inpwin->window, symbol);
  }

  // Fill in the rest with empty space
  for(int index = amount; index < inpwin->xmax - 3; index++)
  {
    waddch(inpwin->window, ' ');
  }

  box(inpwin->window, 0, 0);

  mvwprintw(inpwin->window, 2, inpwin->xmax - 8, "%03d/%03d",
            inpwin->length, inpwin->msize);

  wmove(inpwin->window, 1, 1 + (inpwin->cursor - inpwin->scroll));

  wrefresh(inpwin->window);
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

  // Shift characters forward to make room for new character
  for(int index = inpwin->length + 1; index-- > inpwin->cursor;)
  {
    inpwin->buffer[index] = inpwin->buffer[index - 1];
  }

  inpwin->buffer[inpwin->cursor++] = symbol;

  inpwin->length++;

  // The cursor is at the end of the input window
  if((inpwin->cursor - inpwin->scroll) >= inpwin->xmax - 2)
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

/*
 * Input text from inside a specified window
 *
 * Don't let the user input more text than specified,
 * or what the window can hold
 */
void inpwin_input(inpwin_t* inpwin, bool hidden)
{
  curs_set(1);

  inpwin_refresh(inpwin, hidden);

  int key;
  while((key = wgetch(inpwin->window)))
  {
    if(key == 10) break;

    switch(key)
    {
      case KEY_RIGHT:
        // The cursor can not be further than the text itself
        inpwin->cursor = MIN(inpwin->cursor + 1, inpwin->length);

        // The cursor is at the end of the input window
        if((inpwin->cursor - inpwin->scroll) >= inpwin->xmax - 2)
        {
          inpwin->scroll++; // Scroll one more character
        }
        break;

      case KEY_LEFT:
        // The cursor is at the end of the input window
        if(inpwin->cursor <= inpwin->scroll && inpwin->scroll > 0)
        {
          inpwin->scroll--; // Scroll back one character
        }

        inpwin->cursor = MAX(inpwin->cursor - 1, 0);

        break;

      case KEY_BACKSPACE:
        inpwin_symbol_del(inpwin);
        break;
      
      default:
        inpwin_symbol_add(inpwin, key);
        break;
    }
  
    inpwin_refresh(inpwin, hidden);
  }
  curs_set(0);
}

/*
 * Input password from a popup window prompting you
 */
void string_input(char* string, size_t size, const char* prompt, bool hidden)
{
  int ymax = getmaxy(stdscr);

  inpwin_t* inpwin = inpwin_center_create(stdscr, (ymax / 2) - 2, 10, string, size);
  if(inpwin == NULL) return;

  window_title_center_print(inpwin->window, prompt);

  refresh();

  inpwin_input(inpwin, hidden);

  inpwin_free(inpwin);
}
