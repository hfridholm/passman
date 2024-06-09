#ifndef P_WIN_CONFIRM_H
#define P_WIN_CONFIRM_H

typedef struct
{
  win_head_t head;
  char*      prompt;       // The question to answer
  int        prompt_len;
  char*      text_yes;     // The text for answering yes
  int        text_yes_len;
  char*      text_no;      // The text for answering no
  int        text_no_len;
  bool       answer;       // The answer (yes or no)
} win_confirm_t;

extern win_confirm_t* win_confirm_create(char* name, int x, int y, int w, char* prompt, char* text_yes, char* text_no, bool active, key_handler_t* key_handler);

extern void           win_confirm_free(win_confirm_t* win);

extern void           win_confirm_refresh(win_confirm_t* win);

extern void           win_confirm_resize(win_confirm_t* win, int x, int y, int w);


extern void win_confirm_prompt_set(win_confirm_t* win, char* prompt);

extern void win_confirm_key_handler(win_head_t* win_head, int key);

#endif // P_WIN_CONFIRM_H
