#ifndef PASSMAN_INFWIN_H
#define PASSMAN_INFWIN_H

typedef struct
{
  window_t* window;
  int ymax;
  int xmax;
  char* title;    // The title (error, info)
  char* text;     // The text / information
} infwin_t;       // Info Window

#endif // PASSMAN_INFWIN_H
