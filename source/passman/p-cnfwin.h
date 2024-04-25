#ifndef P_CNFWIN_H
#define P_CNFWIN_H

#include "../passman.h"

typedef struct
{
  window_t* window;
  char*     prompt; // The question to answer
  int       pmtlen;
  char*     ytext;  // The text for answering yes
  int       ytxlen;
  char*     ntext;  // The text for answering no
  int       ntxlen;
  bool      answer; // The answer (yes or no)
} cnfwin_t;

extern void cnfwin_prompt_set(cnfwin_t* cnfwin, char* prompt);

extern cnfwin_t* cnfwin_create(int x, int y, int w, char* prompt, char* ytext, char* ntext, bool active);

extern void cnfwin_free(cnfwin_t* cnfwin);

extern void cnfwin_refresh(cnfwin_t* cnfwin);

extern void cnfwin_resize(cnfwin_t* cnfwin, int x, int y, int w);

extern void cnfwin_input(cnfwin_t* cnfwin, void (*key_handler)(int));

extern void cnfpop_input(cnfwin_t* cnfpop, void (*key_handler)(int));

#endif // P_CNFWIN_H
