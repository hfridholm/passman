#ifndef PASSMAN_INPWIN_H
#define PASSMAN_INPWIN_H

#include "../passman.h"

typedef struct
{
  window_t* window;
  int xmax;
  char* buffer;   // The inputted string
  int msize;      // Max size of buffer
  int length;     // The length of the string
  int cursor;     // The index which the cursor is on
  int scroll;     // How many characters that is scrolled
} inpwin_t;       // Input Window

extern void string_input(char* string, size_t size, const char* prompt, bool hidden);

extern inpwin_t* inpwin_create(int x, int y, int w, char* buffer, size_t size);

extern void inpwin_resize(inpwin_t* inpwin, int x, int y, int w);

extern void inpwin_free(inpwin_t* inpwin);

extern void inpwin_key_handler(inpwin_t* inpwin, int key);

extern void inpwin_refresh(inpwin_t* inpwin, bool hidden);

#endif // PASSMAN_INPWIN_H
