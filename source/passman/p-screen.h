#ifndef P_SCREEN_H
#define P_SCREEN_H

typedef enum
{
  MENU_DATABASES,
  MENU_PASSWORD,
  MENU_ACCOUNT,
  MENU_DATABASE
} menu_t;

typedef enum
{
  POPUP_NONE,
  POPUP_EXIT,
  POPUP_OPEN,
  POPUP_NEW,
  POPUP_DELETE,
  POPUP_RENAME
} popup_t;


extern bool running;

extern cnfwin_t* delpop;
extern inpwin_t* pswpop;
extern cnfwin_t* extpop;

extern mendbs_t mendbs;
extern menpsw_t menpsw;

extern menu_t menu;
extern popup_t popup;

extern void screen_key_handler(int key);

extern void screen_resize(void);

extern void screen_refresh(void);

#endif // P_SCREEN_H
