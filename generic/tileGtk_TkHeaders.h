/*
 *  tileGtk_TkHeaders.h
 * ----------------------
 *
 * This file is part of the Tile-Gtk package, a Tk/Tile based theme that uses
 * Gtk/GNOME for drawing.
 *
 * Copyright (C) 2004-2008 by:
 * Georgios Petasis, petasis@iit.demokritos.gr,
 * Software and Knowledge Engineering Laboratory,
 * Institute of Informatics and Telecommunications,
 * National Centre for Scientific Research (NCSR) "Demokritos",
 * Aghia Paraskevi, 153 10, Athens, Greece.
 */

#ifndef __TILEGTK_TK_HEADERS
#define __TILEGTK_TK_HEADERS

#ifdef USE_TK_STUBS
/*
#define USE_TTK_STUBS 1
 */
#endif

#include <tk.h>
#include "ttkTheme.h"
#define TILE_VERSION TTK_VERSION
//#include "ttkDecls.h"

#ifdef __WIN32__
#include <tkWinInt.h>
#endif

#endif /* __TILEGTK_TK_HEADERS */
