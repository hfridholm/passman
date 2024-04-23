#ifndef P_INFWIN_H
#define P_INFWIN_H

typedef struct
{
  window_t* window;
  char* title;      // The title (error, info)
  char* text;       // The text / information
} infwin_t;         // Info window

extern void infwin_input(infwin_t* infwin, void (*key_handler)(int));

extern void infwin_resize(infwin_t* infwin, int x, int y, int w);

extern infwin_t* infwin_create(int x, int y, int w, char* title, char* text);

extern void infwin_free(infwin_t* infwin);

extern void infwin_refresh(infwin_t* infwin);

#endif // P_INFWIN_H
