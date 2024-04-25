#ifndef P_INFWIN_H
#define P_INFWIN_H

typedef struct
{
  window_t* window;

  char* title;      // The title (error, info)
  int ttllen;       // Length of title

  char* text;       // The text / information
  int txtlen;       // Length of text
} infwin_t;         // Info window

extern void infwin_input(infwin_t* infwin, void (*key_handler)(int));

extern void infwin_resize(infwin_t* infwin, int x, int y, int w, int h);

extern infwin_t* infwin_create(int x, int y, int w, int h, char* title, char* text, bool active);

extern void infwin_free(infwin_t* infwin);

extern void infwin_refresh(infwin_t* infwin);

extern void infpop_input(infwin_t* infpop, void (*key_handler)(int));

#endif // P_INFWIN_H
