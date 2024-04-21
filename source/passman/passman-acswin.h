#ifndef PASSMAN_ACSWIN_H
#define PASSMAN_ACSWIN_H

#include "../passman.h"

typedef struct
{
  window_t* window;
  int ymax;
  int xmax;
  int index;      // Which account the user hovers over
  int scroll;     // How many lines has been scrolled down
} acswin_t;       // Accounts Window

extern void acswin_free(acswin_t* acswin);

extern acswin_t* acswin_center_create(window_t* parent, int h, int y, int x);

extern void acswin_refresh(acswin_t* acswin);

#endif // PASSMAN_ACSWIN_H
