# Notes

core_pattern
|/usr/share/apport/apport -p%p -s%s -c%c -d%d -P%P -u%u -g%g -- %E

- either clear input buffer before inputting or after inputted?
- make up my mind about when to check for valid allocations (variables)

- create ~/.passman/ directory instead of ./assets/
- link database values to inpwin_t buffer
- limit header files to the scripts that need them
- implement status codes for _refresh and _input
- check if database name is valid after rename, if not,
  change back to previous name
- add "Press enter to continue..." text to infwin
- change buffer_size and item_count from int to size_t
- require password to rename database. Use password to load and change name in struct.
- require password to delete database.
- add "example" field to win_input_t (gray overwritable text)
- strip string when creating database and account
- add more text windows in menus to give information
- create text box that describes keys in the bottom of every menu
- create grid system for windows, instead of manually set x, y, w, h pixels
- new input win field "allow_spaces" for ex password
- add "locked" field to win_head_t, like "active". Can't interact with. Is rendered grayed out
- change exit keybind to ctrl-x and let ctrl-c be copy.

## Must
- BUG: input delete when scroll has occured
- password input does not get cleared properly

## Maybe
- rename inpwin scroll to shift?
- create win_head_t (ish) struct to pass to win_***_create
- add hash variable in screen struct of the focus win (for comparison)

## Future
- add status codes to inpwin symbol_add and del,
  and make a error sound if the user could not del symbol for example
- change red outline to thicker outline
