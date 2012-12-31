/*
 *  tileGtk_Separator.cpp
 * ---------------------
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
static Ttk_StateTable separator_statemap[] =
{
#ifdef TILEGTK_GTK_VERSION_3
    {QStyle::Style_Default,                         TTK_STATE_DISABLED, 0},
    {QStyle::Style_Enabled,                         0, 0}
#endif /* TILEGTK_GTK_VERSION_3 */
#ifdef TILEGTK_GTK_VERSION_4
    {QStyle::State_None,                            TTK_STATE_DISABLED, 0},
    {QStyle::State_Enabled,                         0, 0}
#endif /* TILEGTK_GTK_VERSION_4 */
};

typedef struct {
    Tcl_Obj        *orientObj;
    Tcl_Obj        *borderObj;
} SeparatorElement;

#define TO_CHAR_STAR (char *)
static Ttk_ElementOptionSpec SeparatorElementOptions[] = {
    { TO_CHAR_STAR "-orient", TK_OPTION_ANY,
        Tk_Offset(SeparatorElement, orientObj), TO_CHAR_STAR "horizontal" },
    { TO_CHAR_STAR "-background", TK_OPTION_BORDER,
        Tk_Offset(SeparatorElement,borderObj), DEFAULT_BACKGROUND },
    {NULL}
};

static void SeparatorElementGeometry(
    void *clientData, void *elementRecord, Tk_Window tkwin,
    int *widthPtr, int *heightPtr, Ttk_Padding *paddingPtr)
{
    if (!TileGtk_GtkInitialised()) NO_GTK_STYLE_ENGINE;
    NULL_PROXY_ORIENTED_WIDGET(TileGtk_QWidget_Widget);
    if (orient == TTK_ORIENT_HORIZONTAL) {
      *heightPtr = PMW(PM_DefaultFrameWidth, wc->TileGtk_QWidget_Widget);
    } else {
      *widthPtr  = PMW(PM_DefaultFrameWidth, wc->TileGtk_QWidget_Widget);
    }
    *paddingPtr = Ttk_UniformPadding(0);
}

static void SeparatorElementDraw(
    void *clientData, void *elementRecord, Tk_Window tkwin,
    Drawable d, Ttk_Box b, unsigned state)
{
    if (!TileGtk_GtkInitialised()) NO_GTK_STYLE_ENGINE;
    NULL_PROXY_ORIENTED_WIDGET(TileGtk_QWidget_Widget);
    Tcl_MutexLock(&tilegtkMutex);
    QPixmap      pixmap(b.width, b.height);
    QPainter     painter(&pixmap);
    TILEGTK_PAINT_BACKGROUND(b.width, b.height);
#ifdef TILEGTK_GTK_VERSION_3
    QStyle::SFlags sflags = TileGtk_StateTableLookup(separator_statemap, state);
    if (orient == TTK_ORIENT_HORIZONTAL) {
      sflags |= QStyle::Style_Horizontal;
    }
    wc->TileGtk_Style->drawPrimitive(QStyle::PE_Separator, &painter,
          QRect(0, 0, b.width, b.height), qApp->palette().active(), sflags);
#endif /* TILEGTK_GTK_VERSION_3 */
#ifdef TILEGTK_GTK_VERSION_4
    QStyleOption option;
    option.state |= 
      (QStyle::StateFlag) TileGtk_StateTableLookup(separator_statemap, state);
    wc->TileGtk_Style->drawPrimitive(QStyle::PE_Q3Separator, &option,
                                    &painter);
#endif /* TILEGTK_GTK_VERSION_4 */
    TileGtk_CopyGtkPixmapOnToDrawable(gdkDrawable, d, tkwin,
                                    0, 0, b.width, b.height, b.x, b.y);
    Tcl_MutexUnlock(&tilegtkMutex);
}; /* SeparatorElementDraw */

static void GeneralSeparatorElementGeometry(
    void *clientData, void *elementRecord, Tk_Window tkwin,
    int *widthPtr, int *heightPtr, Ttk_Padding *paddingPtr)
{
    TileGtk_WidgetCache **wc = (TileGtk_WidgetCache **) clientData;
    if (wc == NULL) return;
    SeparatorElement *separator = (SeparatorElement *) elementRecord;
    int orient;
    Ttk_GetOrientFromObj(NULL, separator->orientObj, &orient);
    switch (orient) {
      case TTK_ORIENT_HORIZONTAL:
        SeparatorElementGeometry((void *) wc[0], elementRecord, tkwin,
                                 widthPtr, heightPtr, paddingPtr);
        break;
      case TTK_ORIENT_VERTICAL:
        SeparatorElementGeometry((void *) wc[1], elementRecord, tkwin,
                                 widthPtr, heightPtr, paddingPtr);
        break;
    }
}

static void GeneralSeparatorElementDraw(
    void *clientData, void *elementRecord, Tk_Window tkwin,
    Drawable d, Ttk_Box b, unsigned state)
{
    TileGtk_WidgetCache **wc = (TileGtk_WidgetCache **) clientData;
    if (wc == NULL) return;
    SeparatorElement *separator = (SeparatorElement *) elementRecord;
    int orient;
    Ttk_GetOrientFromObj(NULL, separator->orientObj, &orient);
    switch (orient) {
      case TTK_ORIENT_HORIZONTAL:
        SeparatorElementDraw((void *) wc[0], elementRecord, tkwin,
                                 d, b, state);
        break;
      case TTK_ORIENT_VERTICAL:
        SeparatorElementDraw((void *) wc[1], elementRecord, tkwin,
                                 d, b, state);
        break;
    }
}; /* GeneralSeparatorElementDraw */

static Ttk_ElementSpec SeparatorElementSpec = {
    TK_STYLE_VERSION_2,
    sizeof(SeparatorElement),
    SeparatorElementOptions,
    SeparatorElementGeometry,
    SeparatorElementDraw
};

static Ttk_ElementSpec GeneralSeparatorElementSpec = {
    TK_STYLE_VERSION_2,
    sizeof(SeparatorElement),
    SeparatorElementOptions,
    GeneralSeparatorElementGeometry,
    GeneralSeparatorElementDraw
};

/*------------------------------------------------------------------------
 * +++ Widget layout.
 */

TTK_BEGIN_LAYOUT(SeparatorLayout)
    TTK_NODE("Separator.separator", TTK_FILL_BOTH)
TTK_END_LAYOUT

int TileGtk_Init_Separator(Tcl_Interp *interp,
                       TileGtk_WidgetCache **wc, Ttk_Theme themePtr)
{
    /*
     * Register elements:
     */
    Ttk_RegisterElement(interp, themePtr, "separator",
            &GeneralSeparatorElementSpec, (void *) wc);
    Ttk_RegisterElement(interp, themePtr, "hseparator",
            &SeparatorElementSpec, (void *) wc[0]);
    Ttk_RegisterElement(interp, themePtr, "vseparator",
            &SeparatorElementSpec, (void *) wc[1]);
    
    /*
     * Register layouts:
     */
    Ttk_RegisterLayout(themePtr, "TSeparator", SeparatorLayout);

    return TCL_OK;
}; /* TileGtk_Init_Separator */
