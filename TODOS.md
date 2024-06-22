# Todos
- link database values to inpwin_t buffer
- limit header files to the scripts that need them
- implement status codes for _refresh and _input
- check if database name is valid after rename, if not,
  change back to previous name
- ability to change database password in mendbe
- add "Press enter to continue..." text to infwin
- create bool screen_win_is_focus function,
  add hash variable in screen struct of the focus win (for comparison)
- change buffer_size and item_count from int to size_t
- make sure only one key event is called (with return values)
- add end_string to win_list_item_t

## Must

## Maybe
- rename inpwin scroll to shift?
- create win_head_t (ish) struct to pass to win_***_create
- send debug messages over socket to console client socket process

## Future
- add status codes to inpwin symbol_add and del,
  and make a error sound if the user could not del symbol for example
