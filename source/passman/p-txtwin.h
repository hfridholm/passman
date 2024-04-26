#ifndef P_TXTWIN_H
#define P_TXTWIN_H

typedef struct
{
  window_t* window;
  char*     title;  // The title (error, info)
  int       ttllen; // Length of title
  char*     text;   // The text / information
  int       txtlen; // Length of text
} txtwin_t;

extern void txtwin_input(txtwin_t* txtwin, void (*key_handler)(int));

extern void txtwin_resize(txtwin_t* txtwin, int x, int y, int w, int h);

extern txtwin_t* txtwin_create(int x, int y, int w, int h, char* title, char* text, bool active);

extern void txtwin_free(txtwin_t* txtwin);

extern void txtwin_refresh(txtwin_t* txtwin);

extern void txtpop_input(txtwin_t* txtpop, void (*key_handler)(int));

extern void txtwin_title_set(txtwin_t* txtwin, char* title);

extern void txtwin_text_set(txtwin_t* txtwin, char* text);

#endif // P_TXTWIN_H
