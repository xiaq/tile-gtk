/*
 *  tileGtk_Scale.cpp
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
static Ttk_StateTable scale_statemap[] =
{
};
#endif

typedef struct {
} ScaleTroughElement;

static Ttk_ElementOptionSpec ScaleTroughElementOptions[] = {
    {NULL}
};

static void ScaleTroughElementGeometry(
    void *clientData, void *elementRecord, Tk_Window tkwin,
    int *widthPtr, int *heightPtr, Ttk_Padding *paddingPtr)
{
    TILEGTK_WIDGET_CACHE_DEFINITION;
    gint trough_border = 0;
    int xt = 0, yt = 0;
    GtkWidget *widget = TileGtk_GetScale(wc);
    TILEGTK_ENSURE_GTK_STYLE_ENGINE_ACTIVE;
    TILEGTK_ENSURE_WIDGET_OK;
    TileGtk_gtk_widget_style_get(widget, "trough-border", &trough_border, NULL);
    // xt = widget->style->xthickness;
    // yt = widget->style->ythickness;
    *paddingPtr = Ttk_MakePadding(xt + trough_border,
                                  yt + trough_border,
                                  xt + trough_border,
                                  yt + trough_border);
}

static void ScaleTroughElementDraw(
    void *clientData, void *elementRecord, Tk_Window tkwin,
    Drawable d, Ttk_Box b, unsigned state)
{
    TILEGTK_GTK_DRAWABLE_DEFINITIONS;
    TILEGTK_ENSURE_GTK_STYLE_ENGINE_ACTIVE;
    GtkWidget *widget = TileGtk_GetScale(wc);
    gboolean trough_side_details = FALSE;
    TILEGTK_ENSURE_WIDGET_OK;
    TILEGTK_DRAWABLE_FROM_WIDGET;
    TILEGTK_STYLE_BACKGROUND_DEFAULT;
    TILEGTK_DEFAULT_BACKGROUND;
    TILEGTK_STYLE_FROM_WIDGET;
    TileGtk_StateShadowTableLookup(NULL, state, gtkState, gtkShadow,
            TILEGTK_SECTION_TROUGH|TILEGTK_SECTION_ALL);
    TILEGTK_WIDGET_SET_FOCUS(widget);
    TileGtk_gtk_widget_style_get(widget, "trough-side-details",
                                 &trough_side_details, NULL);
    // TileGtk_StateInfo(state, gtkState, gtkShadow, tkwin, widget);
    if (trough_side_details) {
      int trough_change_pos_x = b.width, trough_change_pos_y = b.height;
      if (wc->gtkOrientation == GTK_ORIENTATION_HORIZONTAL)
        trough_change_pos_x = b.width / 2;
      else
        trough_change_pos_y = b.height / 2;
      TileGtk_gtk_paint_box(style, gdkDrawable, gtkState, GTK_SHADOW_IN, NULL,
              widget, "trough-upper", 0, 0,
              trough_change_pos_x, trough_change_pos_y);
      if (wc->gtkOrientation == GTK_ORIENTATION_HORIZONTAL)
        trough_change_pos_y = 0;
      else
        trough_change_pos_x = 0;
      TileGtk_gtk_paint_box(style, gdkDrawable, gtkState, GTK_SHADOW_IN, NULL,
              widget, "trough-lower", trough_change_pos_x, trough_change_pos_y,
              b.width-trough_change_pos_x, b.height-trough_change_pos_y);
    } else {
      TileGtk_gtk_paint_box(style, gdkDrawable, gtkState, GTK_SHADOW_IN, NULL,
              widget, "trough", 0, 0, b.width, b.height);
    }
    TileGtk_CopyGtkPixmapOnToDrawable(gdkDrawable, d, tkwin,
                   0, 0, b.width, b.height, b.x, b.y);
    TILEGTK_CLEANUP_GTK_DRAWABLE;
}

static Ttk_ElementSpec ScaleTroughElementSpec = {
    TK_STYLE_VERSION_2,
    sizeof(ScaleTroughElement),
    ScaleTroughElementOptions,
    ScaleTroughElementGeometry,
    ScaleTroughElementDraw
};

typedef struct {
    Tcl_Obj *lengthObj;  /* the length of the slider (if a flat style) */
    Tcl_Obj *widthObj;   /* the width of the slider (height if horizontal) */
    Tcl_Obj *orientObj;  /* orientation of overall slider */
} ScaleSliderElement;

static Ttk_ElementOptionSpec ScaleSliderElementOptions[] = {
    {NULL}
};

static void ScaleSliderElementGeometry(
    void *clientData, void *elementRecord, Tk_Window tkwin,
    int *widthPtr, int *heightPtr, Ttk_Padding *paddingPtr)
{
    TILEGTK_WIDGET_CACHE_DEFINITION;
    gint slider_len = 0, slider_width = ScaleThumbMinimumLen;
    GtkWidget *widget = TileGtk_GetScale(wc);
    TILEGTK_ENSURE_GTK_STYLE_ENGINE_ACTIVE;
    TILEGTK_ENSURE_WIDGET_OK;
    TileGtk_gtk_widget_style_get(widget,
           "slider-length", &slider_len,
           "slider-width",  &slider_width, NULL);
    *heightPtr = *widthPtr = slider_width;
    if (wc->orientation == TTK_ORIENT_HORIZONTAL) {
      *heightPtr = slider_width;
      *widthPtr  = slider_len;
    } else {
      *widthPtr  = slider_width;
      *heightPtr = slider_len;
    }
    *paddingPtr = Ttk_UniformPadding(0);
}

static void ScaleSliderElementDraw(
    void *clientData, void *elementRecord, Tk_Window tkwin,
    Drawable d, Ttk_Box b, unsigned state)
{
    TILEGTK_GTK_DRAWABLE_DEFINITIONS;
    TILEGTK_ENSURE_GTK_STYLE_ENGINE_ACTIVE;
    GtkWidget *widget = TileGtk_GetScale(wc);
    GtkAdjustment *adj = NULL;
    TILEGTK_ENSURE_WIDGET_OK;
    TILEGTK_DRAWABLE_FROM_WIDGET;
    TILEGTK_STYLE_BACKGROUND_DEFAULT;
    TILEGTK_DEFAULT_BACKGROUND;
    TILEGTK_STYLE_FROM_WIDGET;
    TILEGTK_WIDGET_SET_FOCUS(widget);
    adj = TileGtk_gtk_range_get_adjustment((GtkRange *) widget);
    TileGtk_gtk_adjustment_set_value(adj,
            TileGtk_ValueFromSlider(wc, tkwin, b));
    TileGtk_StateShadowTableLookup(NULL, state, gtkState, gtkShadow,
            TILEGTK_SECTION_SCROLLBAR|TILEGTK_SECTION_ALL);
    // TileGtk_StateInfo(state, gtkState, gtkShadow, tkwin, widget);
    TileGtk_gtk_paint_slider(style, gdkDrawable, gtkState, gtkShadow, NULL,
            widget,
            (wc->orientation == TTK_ORIENT_HORIZONTAL) ? "hscale" : "vscale",
            0, 0, b.width, b.height, wc->gtkOrientation);
    TileGtk_CopyGtkPixmapOnToDrawable(gdkDrawable, d, tkwin,
                   0, 0, b.width, b.height, b.x, b.y);
    TILEGTK_CLEANUP_GTK_DRAWABLE;
}

static Ttk_ElementSpec ScaleSliderElementSpec = {
    TK_STYLE_VERSION_2,
    sizeof(ScaleSliderElement),
    ScaleSliderElementOptions,
    ScaleSliderElementGeometry,
    ScaleSliderElementDraw
};

/*------------------------------------------------------------------------
 * +++ Widget layout.
 */

TTK_BEGIN_LAYOUT(VerticalScaleLayout)
        TTK_GROUP("Vertical.Scale.trough", TTK_FILL_BOTH,
            TTK_NODE("Vertical.Scale.slider", TTK_PACK_TOP) )
TTK_END_LAYOUT

TTK_BEGIN_LAYOUT(HorizontalScaleLayout)
        TTK_GROUP("Horizontal.Scale.trough", TTK_FILL_BOTH,
            TTK_NODE("Horizontal.Scale.slider", TTK_PACK_LEFT) )
TTK_END_LAYOUT

int TileGtk_Init_Scale(Tcl_Interp *interp,
                       TileGtk_WidgetCache **wc, Ttk_Theme themePtr)
{
    /*
     * Register elements:
     */
    Ttk_RegisterElement(interp, themePtr, "Horizontal.Scale.trough",
            &ScaleTroughElementSpec, (void *) wc[0]);
    Ttk_RegisterElement(interp, themePtr, "Vertical.Scale.trough",
            &ScaleTroughElementSpec, (void *) wc[1]);
    Ttk_RegisterElement(interp, themePtr, "Horizontal.Scale.slider",
            &ScaleSliderElementSpec, (void *) wc[0]);
    Ttk_RegisterElement(interp, themePtr, "Vertical.Scale.slider",
            &ScaleSliderElementSpec, (void *) wc[1]);
    
    /*
     * Register layouts:
     */
    Ttk_RegisterLayout(themePtr,
            "Horizontal.TScale", HorizontalScaleLayout);
    Ttk_RegisterLayout(themePtr,
            "Vertical.TScale", VerticalScaleLayout);

    return TCL_OK;
}; /* TileGtk_Init_Scale */
