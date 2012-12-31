/*
 *  tileGtk_Background.cpp
 * -----------------------
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

#include "tileGtk_Utilities.h"
#include "tileGtk_TkHeaders.h"
#include "tileGtk_WidgetDefaults.h"

/*
 * Map between Tk/Tile & Gtk/GNOME state flags.
 */
#if 0
static Ttk_StateTable background_statemap[] =
{
};
#endif

typedef struct {
} BackgroundElement;

static Ttk_ElementOptionSpec BackgroundElementOptions[] = {
    {NULL}
};

static void BackgroundElementGeometry(
    void *clientData, void *elementRecord, Tk_Window tkwin,
    int *widthPtr, int *heightPtr, Ttk_Padding *paddingPtr)
{
    // *paddingPtr = Ttk_UniformPadding(0);
}

static void BackgroundElementDraw(
    void *clientData, void *elementRecord, Tk_Window tkwin,
    Drawable d, Ttk_Box b, unsigned state)
{
    TILEGTK_GTK_DRAWABLE_DEFINITIONS;
    TILEGTK_ENSURE_GTK_STYLE_ENGINE_ACTIVE;
    TILEGTK_SETUP_GTK_DRAWABLE;
    //TileGtk_StateShadowTableLookup(NULL, state, gtkState, gtkShadow,
    //                               TILEGTK_SECTION_ALL);
    TILEGTK_DEFAULT_BACKGROUND;
    TileGtk_CopyGtkPixmapOnToDrawable(gdkDrawable, d, tkwin,
                   0, 0, b.width, b.height, b.x, b.y);
    TILEGTK_CLEANUP_GTK_DRAWABLE;
}

static Ttk_ElementSpec TileGtk_BackgroundElementSpec = {
    TK_STYLE_VERSION_2,
    sizeof(BackgroundElement),
    BackgroundElementOptions,
    BackgroundElementGeometry,
    BackgroundElementDraw
};

/*------------------------------------------------------------------------
 * +++ Border element.
 */
#if 0
typedef struct {
    Tcl_Obj 	*borderWidthObj;
    Tcl_Obj 	*reliefObj;
} BorderElement;

static Ttk_ElementOptionSpec BorderElementOptions[] = {
    { "-borderwidth", TK_OPTION_PIXELS,
	Tk_Offset(BorderElement,borderWidthObj), BorderBorderWidth },
    { "-relief", TK_OPTION_RELIEF,
	Tk_Offset(BorderElement,reliefObj), "flat" },
    {NULL}
};

static void BorderElementGeometry(
    void *clientData, void *elementRecord, Tk_Window tkwin,
    int *widthPtr, int *heightPtr, Ttk_Padding *paddingPtr)
{
    if (!TileGtk_GtkInitialised()) NO_GTK_STYLE_ENGINE;
    //Tcl_MutexLock(&tilegtkMutex);
    BorderElement *border = (BorderElement*) elementRecord;
    int borderWidth = 0;
    Tcl_GetIntFromObj(NULL, border->borderWidthObj, &borderWidth);
    *paddingPtr = Ttk_UniformPadding((short)borderWidth+2);
    //Tcl_MutexUnlock(&tilegtkMutex);
}

static void BorderElementDraw(
    void *clientData, void *elementRecord, Tk_Window tkwin,
    Drawable d, Ttk_Box b, unsigned state)
{
    if (!TileGtk_GtkInitialised()) NO_GTK_STYLE_ENGINE;
    NULL_PROXY_WIDGET(TileGtk_QWidget_WidgetParent);
    Tcl_MutexLock(&tilegtkMutex);
    BorderElement *border = (BorderElement*) elementRecord;
    int relief = TK_RELIEF_FLAT;
    int borderWidth = 0;

    Tcl_GetIntFromObj(NULL, border->borderWidthObj, &borderWidth);
    Tk_GetReliefFromObj(NULL, border->reliefObj, &relief);
    QStyle::SFlags sflags;

    switch (relief) {
	case TK_RELIEF_GROOVE :
	case TK_RELIEF_RIDGE :
	case TK_RELIEF_RAISED :
            sflags |= QStyle::Style_Raised;
	    break;
	case TK_RELIEF_SUNKEN :
            sflags |= QStyle::Style_Sunken;
	    break;
	case TK_RELIEF_FLAT :
	    break;
	case TK_RELIEF_SOLID :
	    break;
    }

    QPixmap      pixmap(b.width, b.height);
    QPainter     painter(&pixmap);
    if (wc->TileGtk_QPixmap_BackgroundTile &&
        !(wc->TileGtk_QPixmap_BackgroundTile->isNull())) {
        painter.fillRect(0, 0, b.width, b.height,
                         QBrush(QColor(255,255,255),
                         *(wc->TileGtk_QPixmap_BackgroundTile)));
    } else {
        painter.fillRect(0, 0, b.width, b.height,
                         qApp->palette().active().background());
    }
    if (borderWidth) {
      wc->TileGtk_Style->drawPrimitive(QStyle::PE_GroupBoxFrame, &painter,
            QRect(0, 0, b.width, b.height), qApp->palette().active(), sflags,
            QStyleOption(borderWidth, 0));
    }
    TileGtk_CopyGtkPixmapOnToDrawable(gdkDrawable, d, tkwin,
                                    0, 0, b.width, b.height, b.x, b.y);
    Tcl_MutexUnlock(&tilegtkMutex);
}

static Ttk_ElementSpec BorderElementSpec = {
    TK_STYLE_VERSION_2,
    sizeof(BorderElement),
    BorderElementOptions,
    BorderElementGeometry,
    BorderElementDraw
};
#endif

/*------------------------------------------------------------------------
 * +++ Widget layout.
 */

int TileGtk_Init_Background(Tcl_Interp *interp,
                           TileGtk_WidgetCache **wc, Ttk_Theme themePtr)
{
    /*
     * Register elements:
     */
    Ttk_RegisterElement(interp, themePtr, "background",
            &TileGtk_BackgroundElementSpec, (void *) wc[0]);
    //Ttk_RegisterElement(interp, themePtr, "border",
    //        &BorderElementSpec, NULL);
    /*
     * Register layouts:
     */

    return TCL_OK;
}; /* TileGtk_Init_Background */
