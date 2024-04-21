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

#include "passman/passman-database.h"

typedef WINDOW window_t;

#include "passman/passman-cnfwin.h"
#include "passman/passman-inpwin.h"
#include "passman/passman-infwin.h"
#include "passman/passman-dbswin.h"
#include "passman/passman-acswin.h"

#include "passman/p-mendbs.h"

#endif // PASSMAN_H
