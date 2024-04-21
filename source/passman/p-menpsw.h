#ifndef P_MENPSW_H
#define P_MENPSW_H

typedef struct
{
  inpwin_t* pswwin;

  char password[64];
} menpsw_t;

extern menpsw_t menpsw;


extern void menpsw_init(void);

extern void menpsw_input(void);

extern void menpsw_free(void);

#endif // P_MENPSW_H
