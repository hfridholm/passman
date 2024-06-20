#ifndef P_WIN_TEXT_H
#define P_WIN_TEXT_H

typedef struct
{
  win_head_t head;
  char*      title;     // The title (error, info)
  int        title_len; // Length of title
  char*      text;      // The text / information
  int        text_len;  // Length of text
} win_text_t;

extern win_text_t* win_text_create(char* name, bool active, bool tab_ability, int x, int y, int w, int h, char* title, char* text, win_event_t* event);

extern void        win_text_free(win_text_t* win);

extern void        win_text_refresh(win_text_t* win);

extern void        win_text_resize(win_text_t* win, int x, int y, int w, int h);


extern void win_text_title_set(win_text_t* win, char* title);

extern void win_text_text_set(win_text_t* win, char* text);


extern void pop_text_event(win_head_t* win_head, int key);


extern win_text_t* wins_name_win_text_get(win_t** wins, int count, char* name);

#endif // P_WIN_TEXT_H
