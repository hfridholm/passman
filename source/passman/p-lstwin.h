#ifndef P_LSTWIN_H
#define P_LSTWIN_H

typedef struct
{
  window_t* window;
  int ymax;
  int xmax;
  int index;      // Which database the user hovers over
  char** dbases;  // The names of the available databases
  int amount;     // The amount of databases
  int scroll;     // How many lines has been scrolled down
} lstwin_t;       // Databases Window

extern void lstwin_free(lstwin_t* lstwin);

extern lstwin_t* lstwin_create(int x, int y, int w, int h, char** dbases, int amount);

extern void lstwin_resize(lstwin_t* lstwin, int x, int y, int w, int h);

extern void lstwin_refresh(lstwin_t* lstwin);

extern void lstwin_key_handler(lstwin_t* lstwin, int key);

#endif // PASSMAN_DBSWIN_H
