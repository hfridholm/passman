#ifndef P_SCREEN_MENU_H
#define P_SCREEN_MENU_H

extern menu_t* screen_active_menu_get(screen_t* screen);

extern menu_t* screen_menu_get(screen_t* screen, char* menu_name);

extern menu_act_t* screen_menu_act_get(screen_t* screen, char* menu_name);


extern void screen_menu_focus_set(screen_t* screen, char* menu_name);

extern void screen_menu_add(screen_t* screen, menu_t* menu);


extern void screen_menu_dbs_create(screen_t* screen, char* name, int xmax, int ymax);

extern void screen_menu_act_create(screen_t* screen, char* name, int xmax, int ymax);

extern void screen_menu_db_create(screen_t* screen, char* name, int xmax, int ymax);

extern void screen_menu_psw_create(screen_t* screen, char* name, int xmax, int ymax);


extern int screen_menu_db_dbase_fill(screen_t* screen, char* menu_name, dbase_t* dbase);

extern int screen_menu_act_accnt_fill(screen_t* screen, char* menu_name, accnt_t* accnt);


extern win_list_t* screen_menu_win_list_get(screen_t* screen, char* menu_name, char* win_name);

#endif // P_SCREEN_MENU_H
