/*
 *  tileGtk_Paned.cpp
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

#if 0
/*
 * Map between Tk/Tile & Gtk/GNOME state flags.
 */
static Ttk_StateTable paned_statemap[] =
{
};
#endif

typedef struct {
} PanedSashGripElement;

static Ttk_ElementOptionSpec PanedSashGripElementOptions[] = {
    {NULL}
};

static void PanedSashGripElementGeometry(
    void *clientData, void *elementRecord, Tk_Window tkwin,
    int *widthPtr, int *heightPtr, Ttk_Padding *paddingPtr)
{
    TILEGTK_WIDGET_CACHE_DEFINITION;
    GtkWidget *widget = TileGtk_GetPaned(wc);
    gint size = 5;
    TILEGTK_ENSURE_GTK_STYLE_ENGINE_ACTIVE;
    TILEGTK_ENSURE_WIDGET_OK;
    TileGtk_gtk_widget_style_get(widget, "handle-size", &size, NULL);
    if (wc->orientation == TTK_ORIENT_HORIZONTAL) {
      *widthPtr  = size;
    } else {
      *heightPtr = size;
    }
    *paddingPtr = Ttk_UniformPadding(PanedUniformPadding);
}

static void PanedSashGripElementDraw(
    void *clientData, void *elementRecord, Tk_Window tkwin,
    Drawable d, Ttk_Box b, unsigned state)
{
    TILEGTK_GTK_DRAWABLE_DEFINITIONS;
    TILEGTK_ENSURE_GTK_STYLE_ENGINE_ACTIVE;
    /* TILEGTK_SETUP_GTK_DRAWABLE; */
    GtkWidget *widget = TileGtk_GetPaned(wc);
    TILEGTK_ENSURE_WIDGET_OK;
    TILEGTK_STYLE_FROM_WIDGET;
    TILEGTK_DRAWABLE_FROM_WIDGET;
    TileGtk_StateShadowTableLookup(NULL, state, gtkState, gtkShadow,
            TILEGTK_SECTION_SASH|TILEGTK_SECTION_ALL);
    TILEGTK_DEFAULT_BACKGROUND;
    TileGtk_gtk_paint_handle(style, gdkDrawable, gtkState, GTK_SHADOW_NONE,
            NULL, widget, "paned", 0, 0, b.width, b.height,
            wc->gtkOrientation);
    TileGtk_CopyGtkPixmapOnToDrawable(gdkDrawable, d, tkwin,
                   0, 0, b.width, b.height, b.x, b.y);
    TILEGTK_CLEANUP_GTK_DRAWABLE;
}; /* PanedSashGripElementDraw */

static Ttk_ElementSpec PanedSashGripElementSpec = {
    TK_STYLE_VERSION_2,
    sizeof(PanedSashGripElement),
    PanedSashGripElementOptions,
    PanedSashGripElementGeometry,
    PanedSashGripElementDraw
};

/*------------------------------------------------------------------------
 * +++ Widget layout.
 */

TTK_BEGIN_LAYOUT(HorizontalSashLayout)
    TTK_GROUP("Sash.hsash", TTK_FILL_BOTH,
        TTK_NODE("Sash.hgrip", TTK_FILL_BOTH))
TTK_END_LAYOUT

TTK_BEGIN_LAYOUT(VerticalSashLayout)
    TTK_GROUP("Sash.vsash", TTK_FILL_BOTH,
        TTK_NODE("Sash.vgrip", TTK_FILL_BOTH))
TTK_END_LAYOUT

int TileGtk_Init_Paned(Tcl_Interp *interp,
                       TileGtk_WidgetCache **wc, Ttk_Theme themePtr)
{
    /*
     * Register elements:
     */
    Ttk_RegisterElement(interp, themePtr, "hgrip",
            &PanedSashGripElementSpec,  (void *) wc[0]);
    Ttk_RegisterElement(interp, themePtr, "vgrip",
            &PanedSashGripElementSpec,  (void *) wc[1]);
    
    /*
     * Register layouts:
     */
    Ttk_RegisterLayout(themePtr, "Horizontal.Sash", HorizontalSashLayout);
    Ttk_RegisterLayout(themePtr, "Vertical.Sash", VerticalSashLayout);

    return TCL_OK;
}; /* TileGtk_Init_Paned */
