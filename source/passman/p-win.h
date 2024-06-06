#ifndef P_WIN_H
#define P_WIN_H

typedef enum
{
  WIN_CONFIRM,
  WIN_TEXT,
  WIN_INPUT,
  WIN_LIST
  WIN_COUNT
} win_type_t;

typedef struct
{
  win_type_t     type;
  char*          name;        // Used to identify window
  WINDOW*        window;
  int            ymax;
  int            xmax;
  bool           active;
  key_handler_t* key_handler;
} win_head_t;

typedef win_t win_head_t;


extern win_t* win_head_create(win_type_t type, char* name, int x, int y, int w, int h, bool active, key_handler_t* key_handler);

extern void   win_head_free(win_head_t* win);

extern void   win_head_clean(win_head_t* win);

extern void   win_head_resize(win_head_t* win, int x, int y, int w, int h);


extern void win_free(win_t* win);

extern void win_refresh(win_t* win);

#endif // P_WINDOW_H
