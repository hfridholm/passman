#ifndef PASSMAN_DBSWIN_H
#define PASSMAN_DBSWIN_H

typedef struct
{
  window_t* window;
  int ymax;
  int xmax;
  int index;      // Which database the user hovers over
  char** dbases;  // The names of the available databases
  int amount;     // The amount of databases
  int scroll;     // How many lines has been scrolled down
} dbswin_t;       // Databases Window

extern void dbswin_free(dbswin_t* dbswin);

extern dbswin_t* dbswin_create(int x, int y, int w, int h, char** dbases, int amount);

extern void dbswin_resize(dbswin_t* dbswin, int x, int y, int w, int h);

extern void dbswin_refresh(dbswin_t* dbswin);

extern void dbswin_key_handler(dbswin_t* dbswin, int key);

#endif // PASSMAN_DBSWIN_H
