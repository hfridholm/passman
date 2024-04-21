#ifndef P_INFWIN_H
#define P_INFWIN_H

typedef struct
{
  window_t* window;
  int ymax;
  int xmax;
  char* title;      // The title (error, info)
  char* text;       // The text / information
} infwin_t;         // Info window

#endif // P_INFWIN_H
