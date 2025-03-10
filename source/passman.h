#ifndef PASSMAN_H
#define PASSMAN_H

#include "aes.h"
#include "file.h"
#include "sha256.h"

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

#include <ncurses.h>

#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) > (b)) ? (b) : (a))

#define BETWEEN(x, min, max) (((x) >= (min)) && ((x) <= (max)))

#include "passman/p-dbase.h"

#define KEY_CTRLC 3
#define KEY_CTRLZ 26
#define KEY_ESC   27
#define KEY_CTRLS 19
#define KEY_CTRLH 8
#define KEY_CTRLD 4

#define KEY_ENTR  10
#define KEY_TAB   9

#include "passman/p-screen.h"

#endif // PASSMAN_H
