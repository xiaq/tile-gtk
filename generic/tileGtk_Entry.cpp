/*
 *  tileGtk_Entry.cpp
 * ------------------
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

#if 0
/*
 * Map between Tk/Tile & Gtk/GNOME state flags.
 */
static Ttk_StateTable entry_statemap[] =
{
};
#endif

typedef struct {
} EntryFieldElement;


static Ttk_ElementOptionSpec EntryFieldElementOptions[] = {
    {NULL}
};

static void EntryFieldElementGeometry(
    void *clientData, void *elementRecord, Tk_Window tkwin,
    int *widthPtr, int *heightPtr, Ttk_Padding *paddingPtr)
{
    TILEGTK_WIDGET_CACHE_DEFINITION;
    // GtkBorder border = {0, 0, 0, 0};
    TILEGTK_ENSURE_GTK_STYLE_ENGINE_ACTIVE;
    GtkWidget *widget = TileGtk_GetEntry(wc);
    TILEGTK_ENSURE_WIDGET_OK;
    int xt = widget->style->xthickness;
    int yt = widget->style->ythickness;
    *paddingPtr = Ttk_MakePadding(xt + EntryUniformPadding,
                                  yt + EntryUniformPadding,
                                  xt + EntryUniformPadding,
                                  yt + EntryUniformPadding);
    // TileGtk_gtk_widget_style_get(widget, "inner-border", &border, NULL);
    // TileGtk_g_object_get(widget, "inner-border", &border, NULL);
    // *paddingPtr = TILEGTK_GTKBORDER_TO_PADDING(border);
}

static void EntryFieldElementDraw(
    void *clientData, void *elementRecord, Tk_Window tkwin,
    Drawable d, Ttk_Box b, unsigned state)
{
    TILEGTK_GTK_DRAWABLE_DEFINITIONS;
    TILEGTK_ENSURE_GTK_STYLE_ENGINE_ACTIVE;
    gboolean hasFrame = TRUE;
    GtkWidget *widget = TileGtk_GetEntry(wc);
    TILEGTK_ENSURE_WIDGET_OK;
    TILEGTK_DRAWABLE_FROM_WIDGET;
    style = TileGtk_GetGtkWindowStyle(wc->gtkWindow);
    TILEGTK_DEFAULT_BACKGROUND;
    if (hasFrame) {
      TILEGTK_STYLE_FROM_WIDGET;
      TileGtk_StateShadowTableLookup(NULL, state, gtkState, gtkShadow,
              TILEGTK_SECTION_ENTRY|TILEGTK_SECTION_ALL);
      TILEGTK_WIDGET_SET_FOCUS(widget);
      TileGtk_gtk_paint_shadow(style, gdkDrawable, gtkState, gtkShadow, NULL,
              widget, "entry", 0, 0, b.width, b.height);
    }
    // TileGtk_StateInfo(state, gtkState, gtkShadow, tkwin, widget);
    TileGtk_CopyGtkPixmapOnToDrawable(gdkDrawable, d, tkwin,
                   0, 0, b.width, b.height, b.x, b.y);
    TILEGTK_CLEANUP_GTK_DRAWABLE;
}

static Ttk_ElementSpec EntryFieldElementSpec = {
    TK_STYLE_VERSION_2,
    sizeof(EntryFieldElement),
    EntryFieldElementOptions,
    EntryFieldElementGeometry,
    EntryFieldElementDraw
};

/*------------------------------------------------------------------------
 * +++ Widget layout.
 */

int TileGtk_Init_Entry(Tcl_Interp *interp,
                       TileGtk_WidgetCache **wc, Ttk_Theme themePtr)
{
    /*
     * Register elements:
     */
    Ttk_RegisterElement(interp, themePtr, "Entry.field",
            &EntryFieldElementSpec, (void *) wc[0]);
    
    /*
     * Register layouts:
     */

    return TCL_OK;
}; /* TileGtk_Init_Entry */
