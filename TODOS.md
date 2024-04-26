# Todos
- press TAB to switch between windows in menu
- link database values to inpwin_t buffer
- limit header files to the scripts that need them
- implement status codes for _refresh and _input
- check if database name is valid after rename, if not,
  change back to previous name
- ability to change database password in mendbe
- add "Press enter to continue..." text to infwin
- crop title on box to (xmax - 2)

## Must
- bug: txtwin prints text all wrong
  this was caused by txtwin height not updating when text is set
- don't let txtwin print more chars than possible (ymax - 2)

## Maybe
- rename inpwin scroll to shift?

## Future
- add status codes to inpwin symbol_add and del,
  and make a error sound if the user could not del symbol for example
