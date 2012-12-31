/*
 *  tileGtk_RadioButton.cpp
 * ------------------------
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

typedef struct {
} RadioButtonIndicatorElement;


static Ttk_ElementOptionSpec RadioButtonIndicatorElementOptions[] = {
    {NULL}
};

static void RadioButtonIndicatorElementGeometry(
    void *clientData, void *elementRecord, Tk_Window tkwin,
    int *widthPtr, int *heightPtr, Ttk_Padding *paddingPtr)
{
    TILEGTK_WIDGET_CACHE_DEFINITION;
    gint size, spacing = RadioButtonHorizontalPadding;
    gint focus_width, focus_pad;
    TILEGTK_ENSURE_GTK_STYLE_ENGINE_ACTIVE;
    GtkWidget *widget = TileGtk_GetRadioButton(wc);
    TILEGTK_ENSURE_WIDGET_OK;
    TileGtk_gtk_widget_style_get(widget,
           "indicator-size",    &size,
           "indicator-spacing", &spacing,
           "focus-line-width",  &focus_width,
           "focus-padding",     &focus_pad, NULL);
    *widthPtr = *heightPtr = size;
    size = focus_pad + focus_width;
    *paddingPtr = Ttk_MakePadding(spacing+size, size, spacing+size, size);
}

static void RadioButtonIndicatorElementDraw(
    void *clientData, void *elementRecord, Tk_Window tkwin,
    Drawable d, Ttk_Box b, unsigned state)
{
    TILEGTK_GTK_DRAWABLE_DEFINITIONS;
    gint indicator_size, x, y;
    TILEGTK_ENSURE_GTK_STYLE_ENGINE_ACTIVE;
    GtkWidget *widget = TileGtk_GetRadioButton(wc);
    TILEGTK_ENSURE_WIDGET_OK;
    TILEGTK_DRAWABLE_FROM_WIDGET_SIZE(b.width+20, b.height+20);
    TILEGTK_STYLE_BACKGROUND_DEFAULT;
    TILEGTK_DEFAULT_BACKGROUND_SIZE(b.width+20, b.height+20);
    TILEGTK_STYLE_FROM_WIDGET;
    TILEGTK_WIDGET_SET_FOCUS(widget);
    TileGtk_gtk_widget_style_get(widget,
           "indicator-size", &indicator_size, NULL);
    TileGtk_StateShadowTableLookup(NULL, state, gtkState, gtkShadow,
            TILEGTK_SECTION_BUTTONS|TILEGTK_SECTION_ALL);
    if (state & TTK_STATE_FOCUS) {
      TileGtk_gtk_paint_focus(style, gdkDrawable, gtkState, NULL, widget,
              "radiobutton", 0, 0, b.width + 20, b.height + 20);
    }
    // TileGtk_StateInfo(state, gtkState, gtkShadow, tkwin, widget);
    x = 10 + (b.width  - indicator_size) / 2;
    y = 10 + (b.height - indicator_size) / 2;
    TileGtk_gtk_paint_option(style, gdkDrawable, gtkState, gtkShadow, NULL,
            widget, "radiobutton", x, y, indicator_size, indicator_size);
    TileGtk_CopyGtkPixmapOnToDrawable(gdkDrawable, d, tkwin,
            10, 10, b.width, b.height, b.x, b.y);
    TILEGTK_CLEANUP_GTK_DRAWABLE;
}

static Ttk_ElementSpec RadioButtonIndicatorElementSpec = {
    TK_STYLE_VERSION_2,
    sizeof(RadioButtonIndicatorElement),
    RadioButtonIndicatorElementOptions,
    RadioButtonIndicatorElementGeometry,
    RadioButtonIndicatorElementDraw
};

typedef struct {
} RadioButtonBorderElement;


static Ttk_ElementOptionSpec RadioButtonBorderElementOptions[] = {
    {NULL}
};

static void RadioButtonBorderElementGeometry(
    void *clientData, void *elementRecord, Tk_Window tkwin,
    int *widthPtr, int *heightPtr, Ttk_Padding *paddingPtr)
{
    TILEGTK_WIDGET_CACHE_DEFINITION;
    gint focus_width;
    gint focus_pad;
    TILEGTK_ENSURE_GTK_STYLE_ENGINE_ACTIVE;
    GtkWidget *widget = TileGtk_GetRadioButton(wc);
    TILEGTK_ENSURE_WIDGET_OK;
    TileGtk_gtk_widget_style_get(widget,
           "focus-line-width", &focus_width,
           "focus-padding",    &focus_pad, NULL);
    *paddingPtr = Ttk_UniformPadding(focus_width + focus_pad);
}

static void RadioButtonBorderElementDraw(
    void *clientData, void *elementRecord, Tk_Window tkwin,
    Drawable d, Ttk_Box b, unsigned state)
{
    TILEGTK_GTK_DRAWABLE_DEFINITIONS;
    TILEGTK_ENSURE_GTK_STYLE_ENGINE_ACTIVE;
    GtkWidget *widget = TileGtk_GetRadioButton(wc);
    TILEGTK_ENSURE_WIDGET_OK;
    TILEGTK_DRAWABLE_FROM_WIDGET;
    TILEGTK_STYLE_BACKGROUND_DEFAULT;
    TILEGTK_DEFAULT_BACKGROUND;
    if (state & TTK_STATE_FOCUS) {
      gint border_width = ((GtkContainer*) widget)->border_width;
      gint focus_width;
      gint focus_pad;
      TILEGTK_STYLE_FROM_WIDGET;
      TILEGTK_WIDGET_SET_FOCUS(widget);
      TileGtk_StateShadowTableLookup(NULL, state, gtkState, gtkShadow,
            TILEGTK_SECTION_BUTTONS|TILEGTK_SECTION_ALL);
      TileGtk_gtk_widget_style_get(widget,
           "focus-line-width", &focus_width,
           "focus-padding",    &focus_pad, NULL);
      TileGtk_gtk_paint_focus(style, gdkDrawable, gtkState, NULL, widget,
              "radiobutton", b.x + border_width, b.y + border_width,
               b.width - 2 * border_width, b.height - 2 * border_width);
    }
    TileGtk_CopyGtkPixmapOnToDrawable(gdkDrawable, d, tkwin,
                   0, 0, b.width, b.height, b.x, b.y);
    TILEGTK_CLEANUP_GTK_DRAWABLE;
}

static Ttk_ElementSpec RadioButtonBorderElementSpec = {
    TK_STYLE_VERSION_2,
    sizeof(RadioButtonBorderElement),
    RadioButtonBorderElementOptions,
    RadioButtonBorderElementGeometry,
    RadioButtonBorderElementDraw
};


/*------------------------------------------------------------------------
 * +++ Widget layout.
 */

/* 
 * TTK_BEGIN_LAYOUT(RadiobuttonLayout)
 *      TTK_GROUP("Radiobutton.border", TTK_FILL_BOTH,
 *          TTK_GROUP("Radiobutton.padding", TTK_FILL_BOTH,
 *              TTK_NODE("Radiobutton.indicator", TTK_PACK_LEFT)
 *              TTK_GROUP("Radiobutton.focus", TTK_PACK_LEFT,
 *                  TTK_NODE("Radiobutton.label", TTK_FILL_BOTH))))
 * TTK_END_LAYOUT
 */
TTK_BEGIN_LAYOUT(RadiobuttonLayout)
  TTK_GROUP("Radiobutton.border", TTK_FILL_BOTH,
    TTK_GROUP("Radiobutton.padding", TTK_FILL_BOTH,
      TTK_NODE("Radiobutton.indicator", TTK_PACK_LEFT)
      TTK_NODE("Radiobutton.label", TTK_PACK_LEFT|TTK_STICK_W|TTK_FILL_BOTH)))
TTK_END_LAYOUT

int TileGtk_Init_RadioButton(Tcl_Interp *interp,
                       TileGtk_WidgetCache **wc, Ttk_Theme themePtr)
{
    /*
     * Register elements:
     */
    Ttk_RegisterElement(interp, themePtr, "Radiobutton.border",
            &RadioButtonBorderElementSpec, (void *) wc[0]);
    Ttk_RegisterElement(interp, themePtr, "Radiobutton.indicator",
            &RadioButtonIndicatorElementSpec, (void *) wc[0]);
    
    /*
     * Register layouts:
     */
    Ttk_RegisterLayout(themePtr, "TRadiobutton", RadiobuttonLayout);
    
    return TCL_OK;
}; /* TileGtk_Init_RadioButton */
