#ifndef P_CNFWIN_H
#define P_CNFWIN_H

#include "../passman.h"

typedef struct
{
  window_t* window;
  int ymax;
  int xmax;
  char* prompt;   // The question to answer
  char* ytext;    // The text for answering yes
  char* ntext;    // The text for answering no
  bool answer;    // The answer (yes or no)
} cnfwin_t;       // Confirm Window

extern cnfwin_t* cnfwin_create(int x, int y, int w, char* prompt, char* ytext, char* ntext);

extern void cnfwin_free(cnfwin_t* cnfwin);

extern void cnfwin_refresh(cnfwin_t* cnfwin);

extern void cnfwin_resize(cnfwin_t* cnfwin, int x, int y, int w);

extern void cnfwin_input(cnfwin_t* cnfwin, void (*key_handler)(int));

#endif // P_CNFWIN_H
