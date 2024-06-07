#ifndef P_SCREEN_MENU_H
#define P_SCREEN_MENU_H

extern menu_t* screen_menu_get(screen_t* screen);

extern void screen_menu_add(screen_t* screen, menu_t* menu);

extern void screen_menu_dbs_create(screen_t* screen, char* name, int xmax, int ymax);

extern void screen_menu_db_create(screen_t* screen, char* name, int xmax, int ymax);

extern void screen_menu_psw_create(screen_t* screen, char* name, int xmax, int ymax);

#endif // P_SCREEN_MENU_H
