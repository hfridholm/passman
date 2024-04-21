#ifndef P_MENDBS_H
#define P_MENDBS_H

typedef enum
{
  MENDBS_POPUP_NONE,
  MENDBS_POPUP_DELETE,
  MENDBS_POPUP_RENAME,
  MENDBS_POPUP_NEW
} mendbs_popup_t;

typedef struct
{
  dbswin_t* dbases;
  inpwin_t* search;
  cnfwin_t* delete;

  char buffer[64];

  mendbs_popup_t popup; // Visable popup
} mendbs_t;


extern mendbs_t mendbs;


extern void mendbs_init(void);

extern void mendbs_input(void);

extern void mendbs_free(void);

#endif // P_MENDBS_H
