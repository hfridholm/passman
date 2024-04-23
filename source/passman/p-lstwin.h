#ifndef P_LSTWIN_H
#define P_LSTWIN_H

typedef struct
{
  window_t* window;
  int index;       // Which item the user hovers over
  char** items;    // The items in the list
  int amount;      // The amount of items
  int scroll;      // How many lines has been scrolled down
} lstwin_t;        // List window

extern void lstwin_free(lstwin_t* lstwin);

extern lstwin_t* lstwin_create(int x, int y, int w, int h, char** dbases, int amount);

extern void lstwin_resize(lstwin_t* lstwin, int x, int y, int w, int h);

extern void lstwin_refresh(lstwin_t* lstwin);

extern void lstwin_input(lstwin_t* lstwin, void (*key_handler)(int));

#endif // P_LSTWIN_H
