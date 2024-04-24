# Todos
- press TAB to switch between windows in menu
- link database values to inpwin_t buffer
- add prompt to input (top left corner of box)
- add individual popups to each menu struct (delete, rename, new, usw)
- fix scroll + resize in inpwin_t
- limit header files to the scripts that need them
- implement status codes for _refresh and _input

## Maybe
- Why is windows allocated on the heap?
  Can't you just pass the address to the window?
  (It is easier for the API to use pointers, 
   instead of the user having to pass by reference)
- add "show cursor" attribute to inpwin_t struct
