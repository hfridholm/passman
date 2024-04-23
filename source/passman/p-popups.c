#include "../passman.h"

void delpop_input(void)
{
  popup = POPUP_DELETE;

  cnfwin_input(delpop, NULL);

  popup = POPUP_NONE;
}

void pswpop_input(void)
{
  popup = POPUP_OPEN;
  
  inpwin_input(pswpop, NULL);

  popup = POPUP_NONE;
}

void extpop_input(void)
{
  popup = POPUP_EXIT;

  cnfwin_input(extpop, NULL);

  if(extpop->answer == true) running = false;

  popup = POPUP_NONE;
}
