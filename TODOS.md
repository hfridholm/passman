# Todos
- create men..._t structs
- create x, y, w, h standard for all ...win_create
- press TAB to switch between windows in menu
- link database values to inpwin_t buffer
- add prompt to input (top left corner of box)
- delete infwin_center_create and fix infwin_create
- add "active" attribute to window_t (refresh only the active windows)
- add individual popups to each menu struct (delete, rename, new, usw)
- fix scroll + resize in inpwin_t

## Maybe
- Why is windows allocated on the heap?
  Can't you just pass the address to the window?
  (It is easier for the API to use pointers, 
   instead of the user having to pass by reference)
