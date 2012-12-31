/*
 *  tileGtk_Progress.cpp
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
static Ttk_StateTable progress_statemap[] =
{
};
#endif

typedef struct {
} ProgressTroughElement;

static Ttk_ElementOptionSpec ProgressTroughElementOptions[] = {
    {NULL}
};

static void ProgressTroughElementGeometry(
    void *clientData, void *elementRecord, Tk_Window tkwin,
    int *widthPtr, int *heightPtr, Ttk_Padding *paddingPtr)
{
    *paddingPtr = Ttk_UniformPadding(0);
}

static void ProgressTroughElementDraw(
    void *clientData, void *elementRecord, Tk_Window tkwin,
    Drawable d, Ttk_Box b, unsigned state)
{
    TILEGTK_GTK_DRAWABLE_DEFINITIONS;
    TILEGTK_ENSURE_GTK_STYLE_ENGINE_ACTIVE;
    /* TILEGTK_SETUP_GTK_DRAWABLE; */
    GtkWidget *widget = TileGtk_GetProgressBar(wc);
    TILEGTK_ENSURE_WIDGET_OK;
    TILEGTK_STYLE_FROM_WIDGET;
    TILEGTK_DRAWABLE_FROM_WIDGET;
    TileGtk_StateShadowTableLookup(NULL, state, gtkState, gtkShadow,
            TILEGTK_SECTION_TROUGH|TILEGTK_SECTION_ALL);
    // TILEGTK_SETUP_WIDGET_SIZE(b.width, b.height);
    TILEGTK_WIDGET_SET_FOCUS(widget);
    // TILEGTK_DEFAULT_BACKGROUND;
    // TileGtk_StateInfo(state, gtkState, gtkShadow, tkwin, widget);
    TileGtk_gtk_paint_box(style, gdkDrawable, gtkState, GTK_SHADOW_IN, NULL, widget,
        "trough", 0, 0, b.width, b.height);
    TileGtk_CopyGtkPixmapOnToDrawable(gdkDrawable, d, tkwin,
                   0, 0, b.width, b.height, b.x, b.y);
    TILEGTK_CLEANUP_GTK_DRAWABLE;
}

static Ttk_ElementSpec ProgressTroughElementSpec = {
    TK_STYLE_VERSION_2,
    sizeof(ProgressTroughElement),
    ProgressTroughElementOptions,
    ProgressTroughElementGeometry,
    ProgressTroughElementDraw
};

typedef struct {
    Tcl_Obj    *orientObj;
    Tcl_Obj    *lengthObj;
    Tcl_Obj    *modeObj;
    Tcl_Obj    *maximumObj;
    Tcl_Obj    *valueObj;
} ProgressBarElement;

#define TO_CHAR_STAR (char *)

static Ttk_ElementOptionSpec ProgressBarElementOptions[] = {
    { TO_CHAR_STAR "-orient", TK_OPTION_ANY,
      Tk_Offset(ProgressBarElement, orientObj), TO_CHAR_STAR "horizontal"},
    { TO_CHAR_STAR "-length", TK_OPTION_PIXELS,
      Tk_Offset(ProgressBarElement, lengthObj), TO_CHAR_STAR "100" },
    { TO_CHAR_STAR "-mode", TK_OPTION_STRING_TABLE,
      Tk_Offset(ProgressBarElement, modeObj), TO_CHAR_STAR "determinate" },
    { TO_CHAR_STAR "-maximum", TK_OPTION_DOUBLE,
      Tk_Offset(ProgressBarElement, maximumObj), TO_CHAR_STAR "100" },
    { TO_CHAR_STAR "-value", TK_OPTION_DOUBLE,
      Tk_Offset(ProgressBarElement, valueObj), TO_CHAR_STAR "0.0" },
    { NULL }
};

static void ProgressBarElementGeometry(
    void *clientData, void *elementRecord, Tk_Window tkwin,
    int *widthPtr, int *heightPtr, Ttk_Padding *paddingPtr)
{
    *widthPtr = *heightPtr = ProgressBarThumbSize;
}

static void ProgressBarElementDraw(
    void *clientData, void *elementRecord, Tk_Window tkwin,
    Drawable d, Ttk_Box b, unsigned state)
{
    //ProgressBarElement *pbar = (ProgressBarElement *) elementRecord;
    //int width = Tk_Width(tkwin), height = Tk_Height(tkwin);
    //bool determinate = true;
    //double value, maximum;
    TILEGTK_GTK_DRAWABLE_DEFINITIONS;
    TILEGTK_ENSURE_GTK_STYLE_ENGINE_ACTIVE;
    /* TILEGTK_SETUP_GTK_DRAWABLE; */
    GtkWidget *widget = TileGtk_GetProgressBar(wc);
    TILEGTK_ENSURE_WIDGET_OK;
    TILEGTK_STYLE_FROM_WIDGET;
    TILEGTK_DRAWABLE_FROM_WIDGET;
    // Tcl_GetDoubleFromObj(NULL, pbar->valueObj, &value);
    // Tcl_GetDoubleFromObj(NULL, pbar->maximumObj, &maximum);
    // if (strncmp(Tcl_GetString(pbar->modeObj), "determinate", 10) != 0) {
    //   determinate = false;
    // }

    // if (determinate) {
    //   TileGtk_gtk_progress_bar_set_bar_style((GtkProgressBar *) widget,
    //                                   GTK_PROGRESS_DISCRETE);
    //   TileGtk_gtk_progress_bar_set_fraction((GtkProgressBar *) widget,
    //                      TileGtk_ValueFromSlider(wc, tkwin, b));
    // } else {
    //   TileGtk_gtk_progress_bar_set_bar_style((GtkProgressBar *) widget,
    //                                   GTK_PROGRESS_CONTINUOUS);
    //   TileGtk_gtk_progress_bar_set_fraction((GtkProgressBar *) widget,
    //                      TileGtk_ValueFromSlider(wc, tkwin, b));
    // }

    TILEGTK_WIDGET_SET_FOCUS(widget);
    // TILEGTK_DEFAULT_BACKGROUND;
    TileGtk_StateShadowTableLookup(NULL, state, gtkState, gtkShadow,
            TILEGTK_SECTION_SCROLLBAR|TILEGTK_SECTION_ALL);
    // TileGtk_StateInfo(state, gtkState, gtkShadow, tkwin, widget);
    TileGtk_gtk_paint_box(style, gdkDrawable, gtkState, gtkShadow, NULL, widget,
        "bar", 0, 0, b.width, b.height);
    TileGtk_CopyGtkPixmapOnToDrawable(gdkDrawable, d, tkwin,
                   0, 0, b.width, b.height, b.x, b.y);
    TILEGTK_CLEANUP_GTK_DRAWABLE;
}

static Ttk_ElementSpec ProgressBarElementSpec = {
    TK_STYLE_VERSION_2,
    sizeof(ProgressBarElement),
    ProgressBarElementOptions,
    ProgressBarElementGeometry,
    ProgressBarElementDraw
};

/*------------------------------------------------------------------------
 * +++ Widget layout.
 */

TTK_BEGIN_LAYOUT(VerticalProgressBarLayout)
    TTK_GROUP("Vertical.Progressbar.trough", TTK_FILL_BOTH,
            TTK_NODE("Vertical.Progressbar.pbar", TTK_PACK_BOTTOM|TTK_FILL_X))
TTK_END_LAYOUT

TTK_BEGIN_LAYOUT(HorizontalProgressBarLayout)
    TTK_GROUP("Horizontal.Progressbar.trough", TTK_FILL_BOTH,
        TTK_NODE("Horizontal.Progressbar.pbar", TTK_PACK_LEFT|TTK_FILL_Y))
TTK_END_LAYOUT

int TileGtk_Init_Progress(Tcl_Interp *interp,
                       TileGtk_WidgetCache **wc, Ttk_Theme themePtr)
{
    /*
     * Register elements:
     */
    Ttk_RegisterElement(interp, themePtr, "Horizontal.Progressbar.trough",
            &ProgressTroughElementSpec, (void *) wc[0]);
    Ttk_RegisterElement(interp, themePtr, "Vertical.Progressbar.trough",
            &ProgressTroughElementSpec, (void *) wc[1]);

    Ttk_RegisterElement(interp, themePtr, "Horizontal.Progressbar.pbar",
            &ProgressBarElementSpec, (void *) wc[0]);
    Ttk_RegisterElement(interp, themePtr, "Vertical.Progressbar.pbar",
            &ProgressBarElementSpec, (void *) wc[1]);
    
    /*
     * Register layouts:
     */
    Ttk_RegisterLayout(themePtr,
            "Horizontal.TProgressbar", HorizontalProgressBarLayout);
    Ttk_RegisterLayout(themePtr,
            "Vertical.TProgressbar", VerticalProgressBarLayout);

    return TCL_OK;
}; /* TileGtk_Init_Progress */
