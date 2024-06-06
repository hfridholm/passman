#ifndef P_WIN_INPUT_H
#define P_WIN_INPUT_H

typedef struct
{
  win_head_t* head;
  char*       buffer;      // The inputted string
  int         buffer_size; // Max size of buffer
  int         buffer_len;  // The length of the string
  int         cursor;      // The index which the cursor is on
  int         scroll;      // How many characters that is scrolled
  bool        hidden;      // Render like a hidden password
  bool        secret;      // Only secret inputs can hide content
  char*       title;       // Title
  int         title_len;   // Title length
} win_input_t;

extern win_input_t* win_input_create(char* name, int x, int y, int w, char* buffer, size_t size, char* prompt, bool secret, bool active);

extern void         win_input_resize(win_input_t* win, int x, int y, int w);

extern void         win_input_free(win_input_t* win);

extern void         win_input_refresh(win_input_t* win);


extern void win_input_buffer_set(win_input_t* win, char* buffer, size_t size);

extern void win_input_key_handler(win_input_t* win, int key);

#endif // P_WIN_INPUT_H
