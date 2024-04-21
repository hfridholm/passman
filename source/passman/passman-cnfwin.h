#ifndef PASSMAN_CNFWIN_H
#define PASSMAN_CNFWIN_H

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

extern void cnfwin_key_handler(cnfwin_t* cnfwin, int key);

#endif // PASSMAN_CNFWIN_H
