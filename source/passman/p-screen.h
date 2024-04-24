#ifndef P_SCREEN_H
#define P_SCREEN_H

typedef enum
{
  MENU_DATABASES,
  MENU_PASSWORD,
  MENU_ACCOUNT,
  MENU_DATABASE
} menu_t;

extern bool running;

extern cnfwin_t* extpop;

extern menu_t menu;

extern void screen_key_handler(int key);

extern void screen_resize(void);

extern void screen_refresh(void);

#endif // P_SCREEN_H
