# Todos
- press TAB to switch between windows in menu
- link database values to inpwin_t buffer
- add prompt to input (top left corner of box)
- add individual popups to each menu struct (delete, rename, new, usw)
- fix scroll + resize in inpwin_t
- limit header files to the scripts that need them
- remove screen popup and only keep extpop

## Maybe
- Why is windows allocated on the heap?
  Can't you just pass the address to the window?
  (It is easier for the API to use pointers, 
   instead of the user having to pass by reference)
