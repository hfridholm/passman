#ifndef P_MENDBS_H
#define P_MENDBS_H

typedef struct
{
  lstwin_t* dbases;
  inpwin_t* search;

  char buffer[64];
} mendbs_t;

extern void mendbs_init(void);

extern void mendbs_input(void);

extern void mendbs_free(void);

extern void mendbs_refresh(void);

extern void mendbs_resize(int xmax, int ymax);

#endif // P_MENDBS_H
