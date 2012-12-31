/*
 *  tileGtk_Combobox.cpp
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

typedef struct {
} ComboboxFieldElement;


static Ttk_ElementOptionSpec ComboboxFieldElementOptions[] = {
    {NULL}
};

static void ComboboxFieldElementGeometry(
    void *clientData, void *elementRecord, Tk_Window tkwin,
    int *widthPtr, int *heightPtr, Ttk_Padding *paddingPtr)
{
    TILEGTK_WIDGET_CACHE_DEFINITION;
    TILEGTK_ENSURE_GTK_STYLE_ENGINE_ACTIVE;
    GtkWidget *widget = TileGtk_GetComboboxEntry(wc);
    TILEGTK_ENSURE_WIDGET_OK;
    int xt = widget->style->xthickness;
    int yt = widget->style->ythickness;
    *paddingPtr = Ttk_MakePadding(xt + EntryUniformPadding,
                                  yt + EntryUniformPadding,
                                  xt + EntryUniformPadding,
                                  yt + EntryUniformPadding);
}

static void ComboboxFieldElementDraw(
    void *clientData, void *elementRecord, Tk_Window tkwin,
    Drawable d, Ttk_Box b, unsigned state)
{
    TILEGTK_GTK_DRAWABLE_DEFINITIONS;
    TILEGTK_ENSURE_GTK_STYLE_ENGINE_ACTIVE;
    GtkWidget *widget = NULL;
    if (state & (TTK_STATE_DISABLED|TTK_STATE_READONLY)) {
      widget = TileGtk_GetCombobox(wc);
      TILEGTK_ENSURE_WIDGET_OK;
      TILEGTK_DRAWABLE_FROM_WIDGET;
      TILEGTK_STYLE_BACKGROUND_DEFAULT;
      TILEGTK_DEFAULT_BACKGROUND;
      TILEGTK_STYLE_FROM_WIDGET;
      TileGtk_StateShadowTableLookup(NULL, state, gtkState, gtkShadow,
              TILEGTK_SECTION_BUTTONS|TILEGTK_SECTION_ALL);
      TILEGTK_WIDGET_SET_FOCUS(widget);
      TileGtk_gtk_paint_box(style, gdkDrawable, gtkState, gtkShadow, NULL,
              widget, "button", 0, 0, b.width, b.height);
    } else {
      widget = TileGtk_GetComboboxEntry(wc);
      TILEGTK_ENSURE_WIDGET_OK;
      TILEGTK_DRAWABLE_FROM_WIDGET;
      TILEGTK_STYLE_BACKGROUND_DEFAULT;
      TILEGTK_DEFAULT_BACKGROUND;
      TILEGTK_STYLE_FROM_WIDGET;
      TileGtk_StateShadowTableLookup(NULL, state, gtkState, gtkShadow,
              TILEGTK_SECTION_ENTRY|TILEGTK_SECTION_ALL);
      TILEGTK_WIDGET_SET_FOCUS(widget);
      TileGtk_gtk_paint_shadow(style, gdkDrawable, gtkState, gtkShadow, NULL,
              widget, "combobox", 0, 0, b.width, b.height);
    }
    TileGtk_CopyGtkPixmapOnToDrawable(gdkDrawable, d, tkwin,
                   0, 0, b.width, b.height, b.x, b.y);
    TILEGTK_CLEANUP_GTK_DRAWABLE;
}

static Ttk_ElementSpec ComboboxFieldElementSpec = {
    TK_STYLE_VERSION_2,
    sizeof(ComboboxFieldElement),
    ComboboxFieldElementOptions,
    ComboboxFieldElementGeometry,
    ComboboxFieldElementDraw
};

typedef struct {
} ComboboxArrowElement;


static Ttk_ElementOptionSpec ComboboxArrowElementOptions[] = {
    {NULL}
};

static void ComboboxArrowElementGeometry(
    void *clientData, void *elementRecord, Tk_Window tkwin,
    int *widthPtr, int *heightPtr, Ttk_Padding *paddingPtr)
{
    TILEGTK_WIDGET_CACHE_DEFINITION;
    gint size = 15;
    GtkWidget *widget = TileGtk_GetComboboxEntry(wc);
    TILEGTK_ENSURE_WIDGET_OK;
    TileGtk_gtk_widget_style_get(widget, "arrow-size", &size, NULL);

    *widthPtr = *heightPtr = size;
    *paddingPtr = Ttk_UniformPadding(3);
}

static void ComboboxArrowElementDraw(
    void *clientData, void *elementRecord, Tk_Window tkwin,
    Drawable d, Ttk_Box b, unsigned state)
{
    TILEGTK_GTK_DRAWABLE_DEFINITIONS;
    TILEGTK_ENSURE_GTK_STYLE_ENGINE_ACTIVE;
    /* TILEGTK_SETUP_GTK_DRAWABLE; */
    GtkWidget *widget = NULL;
    GtkWidget *arrow_widget = TileGtk_GetArrow(wc);
    gfloat arrow_scaling;
    gint arrow_x;
    gint arrow_y;
    gint arrow_width;
    gint arrow_height;
    if (state & (TTK_STATE_DISABLED|TTK_STATE_READONLY)) {
      widget = TileGtk_GetCombobox(wc);
      TILEGTK_ENSURE_WIDGET_OK;
      TILEGTK_STYLE_FROM_WIDGET;
      TILEGTK_DRAWABLE_FROM_WIDGET;
      TileGtk_StateShadowTableLookup(NULL, state, gtkState, gtkShadow,
              TILEGTK_SECTION_BUTTONS|TILEGTK_SECTION_ALL);
      TILEGTK_WIDGET_SET_FOCUS(widget);
      TileGtk_gtk_paint_flat_box(style, gdkDrawable, gtkState, gtkShadow, NULL,
              widget, "button", 0, 0, b.width, b.height);
    } else {
      widget = TileGtk_GetComboboxEntry(wc);
      TILEGTK_ENSURE_WIDGET_OK;
      TILEGTK_STYLE_FROM_WIDGET;
      TILEGTK_DRAWABLE_FROM_WIDGET;
      TileGtk_StateShadowTableLookup(NULL, state, gtkState, gtkShadow,
              TILEGTK_SECTION_ENTRY|TILEGTK_SECTION_ALL);
      TILEGTK_WIDGET_SET_FOCUS(widget);
      TileGtk_gtk_paint_flat_box(style, gdkDrawable, gtkState, gtkShadow, NULL,
              widget, "combobox", 0, 0, b.width, b.height);
    }
    /* Draw the arrow, according to the desired scaling! */
    TileGtk_gtk_widget_style_get(arrow_widget, "arrow-scaling",
                                 &arrow_scaling, NULL);
    if (arrow_scaling == 0.0) arrow_scaling = 1.0;
    arrow_width  = b.width  * arrow_scaling;
    arrow_height = b.height * arrow_scaling;
    arrow_x =  (b.width  - arrow_width) / 2;
    arrow_y =  (b.height - arrow_height) / 2;
    TileGtk_gtk_paint_arrow(style, gdkDrawable, gtkState, GTK_SHADOW_NONE, NULL,
        arrow_widget, "combo", GTK_ARROW_DOWN, TRUE,
        arrow_x, arrow_y, arrow_width, arrow_height);
    TileGtk_CopyGtkPixmapOnToDrawable(gdkDrawable, d, tkwin,
                   0, 0, b.width, b.height, b.x, b.y);
    TILEGTK_CLEANUP_GTK_DRAWABLE;
}

static Ttk_ElementSpec ComboboxArrowElementSpec = {
    TK_STYLE_VERSION_2,
    sizeof(ComboboxArrowElement),
    ComboboxArrowElementOptions,
    ComboboxArrowElementGeometry,
    ComboboxArrowElementDraw
};

/*------------------------------------------------------------------------
 * +++ Widget layout.
 */

int TileGtk_Init_Combobox(Tcl_Interp *interp,
                       TileGtk_WidgetCache **wc, Ttk_Theme themePtr)
{
    /*
     * Register elements:
     */
    Ttk_RegisterElement(interp, themePtr, "Combobox.field",
            &ComboboxFieldElementSpec, (void *) wc[0]);
    Ttk_RegisterElement(interp, themePtr, "Combobox.downarrow",
            &ComboboxArrowElementSpec, (void *) wc[0]);
    
    /*
     * Register layouts:
     */

    return TCL_OK;
}; /* TileGtk_Init_Combobox */
