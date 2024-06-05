#ifndef PASSMAN_H
#define PASSMAN_H

#include "aes.h"
#include "file.h"
#include "hash.h"

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

#include <ncurses.h>

#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) > (b)) ? (b) : (a))

#include "passman/p-database.h"

#define KEY_CTRLC 3
#define KEY_CTRLH 8
#define KEY_ENTR  10
#define KEY_TAB   9

#include "passman/p-win.h"

#include "passman/p-win-confirm.h"
#include "passman/p-win-input.h"
#include "passman/p-win-text.h"
#include "passman/p-win-list.h"

#include "passman/p-mendbs.h"
#include "passman/p-mendbe.h"
#include "passman/p-menpsw.h"
#include "passman/p-menact.h"

#include "passman/p-screen.h"

extern void screen_init(void);

extern void screen_free(void);

extern txtwin_t* szepop;

#endif // PASSMAN_H
