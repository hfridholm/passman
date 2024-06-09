# Todos
- press TAB to switch between windows in menu
- link database values to inpwin_t buffer
- limit header files to the scripts that need them
- implement status codes for _refresh and _input
- check if database name is valid after rename, if not,
  change back to previous name
- ability to change database password in mendbe
- add "Press enter to continue..." text to infwin

## Must

## Maybe
- rename inpwin scroll to shift?

## Future
- add status codes to inpwin symbol_add and del,
  and make a error sound if the user could not del symbol for example

## Now
- remove win_index and create system that relies on wins order
  (the last win is the interactive one and gets rendered on top)
  (tabbing rotates the whole array)
- change order of win_create params (name, active, tab_ability, x, y, ...)
