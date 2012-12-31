/*
 *  tileGtk_Utilities.cpp
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

#include "tileGtk_GtkHeaders.h"
#include "tileGtk_TkHeaders.h"
extern gboolean   TileGtk_GtkInitialisedFlag;
extern GtkWidget *TileGtk_GtkWindow;

int TileGtk_GtkInitialised(void) {
  return TileGtk_GtkInitialisedFlag;
}; /* TileGtk_GtkInitialised */

GtkWidget *TileGtk_GetGtkWindow(void) {
  return TileGtk_GtkWindow;
}; /* TileGtk_GetGtkWindow */

GtkStyle *TileGtk_GetGtkWindowStyle(GtkWidget *gtkWindow) {
  if (gtkWindow) {
    return gtkWindow->style;
  }
  return NULL;
}; /* TileGtk_GetGtkWindowStyle */

GtkStyle *TileGtk_GetGtkStyle(void) {
  return TileGtk_GetGtkWindowStyle(TileGtk_GetGtkWindow());
}; /* TileGtk_GetGtkStyle */

void TileGtk_InitialiseGtkWidget(TileGtk_WidgetCache* wc, GtkWidget* widget) {
  // GtkStyle *style;
  if (!wc || !widget) return;
  if (!wc->protoLayout) {
    wc->protoLayout = TileGtk_gtk_fixed_new();
    TileGtk_gtk_container_add((GtkContainer*)(wc->gtkWindow), wc->protoLayout);
  }
  if (!wc->protoLayout) return;
  TileGtk_gtk_container_add((GtkContainer*)(wc->protoLayout), widget);
  TileGtk_gtk_widget_realize(widget);
  // style = TileGtk_gtk_rc_get_style(widget);
  // if (style) TileGtk_gtk_style_attach(style,
}; /* TileGtk_InitialiseGtkWidget */

#define TILEGTK_CHECK_WIDGET(widget, allocator_function) \
  if (!wc) return 0; \
  if (!wc->widget) { \
    wc->widget = allocator_function; \
    TileGtk_InitialiseGtkWidget(wc, wc->widget); \
    return wc->widget; \
  } \
  return wc->widget;

GtkWidget *TileGtk_GetArrow(TileGtk_WidgetCache* wc) {
  TILEGTK_CHECK_WIDGET(gtkArrow,
                       TileGtk_gtk_arrow_new(GTK_ARROW_DOWN, GTK_SHADOW_NONE));
}; /* TileGtk_GetButton */

GtkWidget *TileGtk_GetButton(TileGtk_WidgetCache* wc) {
  TILEGTK_CHECK_WIDGET(gtkButton, TileGtk_gtk_button_new());
}; /* TileGtk_GetButton */

GtkWidget *TileGtk_GetCheckButton(TileGtk_WidgetCache* wc) {
  TILEGTK_CHECK_WIDGET(gtkCheckButton, TileGtk_gtk_check_button_new());
}; /* TileGtk_GetCheckButton */

GtkWidget *TileGtk_GetRadioButton(TileGtk_WidgetCache* wc) {
  return TileGtk_GetCheckButton(wc);
}; /* TileGtk_GetRadioButton */

GtkWidget *TileGtk_GetToolBar(TileGtk_WidgetCache* wc) {
  TILEGTK_CHECK_WIDGET(gtkToolBar, TileGtk_gtk_toolbar_new());
}; /* TileGtk_GetToolBar */

GtkWidget *TileGtk_GetToolButton(TileGtk_WidgetCache* wc) {
  TILEGTK_CHECK_WIDGET(gtkToolButton, TileGtk_gtk_button_new());
}; /* TileGtk_GetToolButton */

GtkWidget *TileGtk_GetFrame(TileGtk_WidgetCache* wc) {
  TILEGTK_CHECK_WIDGET(gtkFrame, TileGtk_gtk_frame_new(NULL));
}; /* TileGtk_GetFrame */

GtkWidget *TileGtk_GetEntry(TileGtk_WidgetCache* wc) {
  TILEGTK_CHECK_WIDGET(gtkEntry, TileGtk_gtk_entry_new());
}; /* TileGtk_GetEntry */

GtkWidget *TileGtk_GetCombobox(TileGtk_WidgetCache* wc) {
  TILEGTK_CHECK_WIDGET(gtkCombobox, TileGtk_gtk_combo_box_new());
}; /* TileGtk_GetComboboxEntry */

GtkWidget *TileGtk_GetComboboxEntry(TileGtk_WidgetCache* wc) {
  TILEGTK_CHECK_WIDGET(gtkComboboxEntry, TileGtk_gtk_combo_box_entry_new());
}; /* TileGtk_GetComboboxEntry */

GtkWidget *TileGtk_GetHScrollBar(TileGtk_WidgetCache* wc) {
  GtkAdjustment *adjustment = (GtkAdjustment *)
             TileGtk_gtk_adjustment_new(0.0, 0.0, 1.0, 0, 0, 0);
  TILEGTK_CHECK_WIDGET(gtkHScrollBar, TileGtk_gtk_hscrollbar_new(adjustment));
}; /* TileGtk_GetHScrollBar */

GtkWidget *TileGtk_GetVScrollBar(TileGtk_WidgetCache* wc) {
  GtkAdjustment *adjustment = (GtkAdjustment *)
             TileGtk_gtk_adjustment_new(0.0, 0.0, 1.0, 0, 0, 0);
  TILEGTK_CHECK_WIDGET(gtkVScrollBar, TileGtk_gtk_vscrollbar_new(adjustment));
}; /* TileGtk_GetVScrollBar */

GtkWidget *TileGtk_GetScrollBar(TileGtk_WidgetCache* wc) {
  if (wc->orientation == TTK_ORIENT_HORIZONTAL) {
    return TileGtk_GetHScrollBar(wc);
  }
  return TileGtk_GetVScrollBar(wc);
}; /* TileGtk_GetScrollBar */

GtkWidget *TileGtk_GetHScale(TileGtk_WidgetCache* wc) {
  TILEGTK_CHECK_WIDGET(gtkHScale, TileGtk_gtk_hscale_new_with_range(0,1,0.001));
}; /* TileGtk_GetHScale */

GtkWidget *TileGtk_GetVScale(TileGtk_WidgetCache* wc) {
  TILEGTK_CHECK_WIDGET(gtkVScale, TileGtk_gtk_vscale_new_with_range(0,1,0.001));
}; /* TileGtk_GetVScale */

GtkWidget *TileGtk_GetScale(TileGtk_WidgetCache* wc) {
  if (wc->orientation == TTK_ORIENT_HORIZONTAL) {
    return TileGtk_GetHScale(wc);
  }
  return TileGtk_GetVScale(wc);
}; /* TileGtk_GetScale */

GtkWidget *TileGtk_GetHProgressBar(TileGtk_WidgetCache* wc) {
  TILEGTK_CHECK_WIDGET(gtkHProgressBar, TileGtk_gtk_progress_bar_new());
  TileGtk_gtk_progress_bar_set_orientation(
     (GtkProgressBar*) wc->gtkHProgressBar, GTK_PROGRESS_LEFT_TO_RIGHT);
  TileGtk_gtk_progress_bar_set_fraction(
     (GtkProgressBar *) wc->gtkHProgressBar, 1);
}; /* TileGtk_GetHProgressBar */

GtkWidget *TileGtk_GetVProgressBar(TileGtk_WidgetCache* wc) {
  TILEGTK_CHECK_WIDGET(gtkVProgressBar, TileGtk_gtk_progress_bar_new());
  TileGtk_gtk_progress_bar_set_orientation(
     (GtkProgressBar*) wc->gtkVProgressBar, GTK_PROGRESS_BOTTOM_TO_TOP);
  TileGtk_gtk_progress_bar_set_fraction(
     (GtkProgressBar *) wc->gtkVProgressBar, 1);
}; /* TileGtk_GetVProgressBar */

GtkWidget *TileGtk_GetProgressBar(TileGtk_WidgetCache* wc) {
  if (wc->orientation == TTK_ORIENT_HORIZONTAL) {
    return TileGtk_GetHProgressBar(wc);
  }
  return TileGtk_GetVProgressBar(wc);
}; /* TileGtk_GetProgressBar */

GtkWidget *TileGtk_GetStatusBar(TileGtk_WidgetCache* wc) {
  TILEGTK_CHECK_WIDGET(gtkStatusBar, TileGtk_gtk_statusbar_new());
}; /* TileGtk_GetStatusBar */

GtkWidget *TileGtk_GetHPaned(TileGtk_WidgetCache* wc) {
  TILEGTK_CHECK_WIDGET(gtkHPaned, TileGtk_gtk_hpaned_new());
}; /* TileGtk_GetHPaned */

GtkWidget *TileGtk_GetVPaned(TileGtk_WidgetCache* wc) {
  TILEGTK_CHECK_WIDGET(gtkVPaned, TileGtk_gtk_vpaned_new());
}; /* TileGtk_GetVPaned */

GtkWidget *TileGtk_GetPaned(TileGtk_WidgetCache* wc) {
  if (wc->orientation == TTK_ORIENT_HORIZONTAL) {
    return TileGtk_GetHPaned(wc);
  }
  return TileGtk_GetVPaned(wc);
}; /* TileGtk_GetPaned */

GtkWidget *TileGtk_GetNotebook(TileGtk_WidgetCache* wc) {
  TILEGTK_CHECK_WIDGET(gtkNotebook, TileGtk_gtk_notebook_new());
}; /* TileGtk_GetNotebook */

const char *TileGtk_GtkStateStr(GtkStateType gtkState) {
  switch ((GtkStateType) gtkState) {
    case GTK_STATE_NORMAL:      return "GTK_STATE_NORMAL";
    case GTK_STATE_ACTIVE:      return "GTK_STATE_ACTIVE";
    case GTK_STATE_PRELIGHT:    return "GTK_STATE_PRELIGHT";
    case GTK_STATE_SELECTED:    return "GTK_STATE_SELECTED";
    case GTK_STATE_INSENSITIVE: return "GTK_STATE_INSENSITIVE";
  }
  return "invalid!";
}; /* TileGtk_GtkStateStr */
const char *TileGtk_GtkShadowStr(GtkShadowType gtkShadow) {
  switch ((GtkShadowType) gtkShadow) {
    case GTK_SHADOW_NONE:       return "GTK_SHADOW_NONE";
    case GTK_SHADOW_IN:         return "GTK_SHADOW_IN";
    case GTK_SHADOW_OUT:        return "GTK_SHADOW_OUT";
    case GTK_SHADOW_ETCHED_IN:  return "GTK_SHADOW_ETCHED_IN";
    case GTK_SHADOW_ETCHED_OUT: return "GTK_SHADOW_ETCHED_OUT";
  }
  return "invalid!";
}; /* TileGtk_GtkShadowStr */

void TileGtk_StateInfo(int state, GtkStateType gtkState,
             GtkShadowType gtkShadow, Tk_Window tkwin, GtkWidget *widget)
{
    printf("Widget: %s\n  ", Tk_PathName(tkwin));
    if (state & TTK_STATE_ACTIVE) {
      printf("TTK_STATE_ACTIVE %d ", TTK_STATE_ACTIVE);
    } 
    if (state & TTK_STATE_DISABLED) {
      printf("TTK_STATE_DISABLED %d ", TTK_STATE_DISABLED);
    } 
    if (state & TTK_STATE_FOCUS) {
      printf("TTK_STATE_FOCUS %d ", TTK_STATE_FOCUS);
    } 
    if (state & TTK_STATE_PRESSED) {
      printf("TTK_STATE_PRESSED %d ", TTK_STATE_PRESSED);
    } 
    if (state & TTK_STATE_SELECTED) {
      printf("TTK_STATE_SELECTED %d ", TTK_STATE_SELECTED);
    } 
    if (state & TTK_STATE_BACKGROUND) {
      printf("TTK_STATE_BACKGROUND %d ", TTK_STATE_BACKGROUND);
    } 
    if (state & TTK_STATE_ALTERNATE) {
      printf("TTK_STATE_ALTERNATE %d ", TTK_STATE_ALTERNATE);
    } 
    if (state & TTK_STATE_INVALID) {
      printf("TTK_STATE_INVALID %d ", TTK_STATE_INVALID);
    } 
    if (state & TTK_STATE_READONLY) {
      printf("TTK_STATE_READONLY %d ", TTK_STATE_READONLY);
    } 
    if (state & TTK_STATE_USER1) {
      printf("TTK_STATE_USER1 %d ", TTK_STATE_USER1);
    } 
    if (state & TTK_STATE_USER2) {
      printf("TTK_STATE_USER2 %d ", TTK_STATE_USER2);
    } 
    if (state & TTK_STATE_USER3) {
      printf("TTK_STATE_USER3 %d ", TTK_STATE_USER3);
    } 
    if (state & TTK_STATE_USER4) {
      printf("TTK_STATE_USER4 %d ", TTK_STATE_USER4);
    } 
    if (state & TTK_STATE_USER5) {
      printf("TTK_STATE_USER5 %d ", TTK_STATE_USER5);
    } 
    if (state & TTK_STATE_USER6) {
      printf("TTK_STATE_USER6 %d ", TTK_STATE_USER6);
    } 
#ifdef TTK_STATE_USER7
    if (state & TTK_STATE_USER7) {
      printf("TTK_STATE_USER7 %d ", TTK_STATE_USER7);
    }
#endif /* TTK_STATE_USER7 */
    printf(" state=%d\n", state);
    printf("  GTK state: %s, shadow: %s",
      TileGtk_GtkStateStr(gtkState), TileGtk_GtkShadowStr(gtkShadow));
#ifndef TILEGTK_LOAD_GTK_DYNAMICALLY
    if (widget) {
      printf(", focus: %d, default: %d", GTK_WIDGET_HAS_FOCUS(widget),
                                         GTK_WIDGET_HAS_DEFAULT(widget));
    }
#endif /* TILEGTK_LOAD_GTK_DYNAMICALLY */
    printf("\n");
    fflush(0);
}; /* TileGtk_StateInfo */

void TileGtk_CopyGtkPixmapOnToDrawable(GdkPixmap *gdkDrawable, Drawable d,
            Tk_Window tkwin, int x, int y, int w, int h, int x1, int x2)
{
#ifdef __WIN32__
#define gc_usage ((GdkGCValuesMask) \
    (GDK_GC_FOREGROUND | GDK_GC_BACKGROUND | GDK_GC_EXPOSURES))
    XGCValues gcValues;
    gcValues.graphics_exposures = False;
    GC gc = Tk_GetGC(tkwin, GCForeground | GCBackground | GCGraphicsExposures,
                     &gcValues);
    GdkGC *gdkGC = TileGtk_gdk_gc_new(gdkDrawable);
    HDC hdcSrc = TileGtk_gdk_win32_hdc_get(gdkDrawable, gdkGC, gc_usage);
    /* Create a Tk Drawable from the HDC... */
    TkWinDrawable gtkD;
    gtkD.type = TWD_WINDC;
    gtkD.winDC.hdc = hdcSrc;
    XCopyArea(Tk_Display(tkwin), (Drawable) &gtkD, d, gc, x, y, w, h, x1, x2);
    TileGtk_gdk_win32_hdc_release(gdkDrawable, gdkGC, gc_usage);
    if (gdkGC) TileGtk_g_object_unref(gdkGC);
    Tk_FreeGC(Tk_Display(tkwin), gc);
#else

#ifdef TILEGTK_USE_XCOPY
    XGCValues gcValues;
    gcValues.graphics_exposures = False;
    GC gc = Tk_GetGC(tkwin, GCForeground | GCBackground | GCGraphicsExposures,
                     &gcValues);
    XCopyArea(Tk_Display(tkwin), GDK_DRAWABLE_XID(pixmap), d, gc,
              x, y, w, h, x1, x2);
    Tk_FreeGC(Tk_Display(tkwin), gc);
    XGCValues gcValues;
    GC gc;

    gcValues.function = GXcopy;
    gcValues.graphics_exposures = False;
    gc = Tk_GetGC(tkwin, GCFunction|GCGraphicsExposures, &gcValues);

    XCopyArea(Tk_Display(tkwin), d, Tk_WindowId(tkwin), gc,
            0, 0, (unsigned) Tk_Width(tkwin), (unsigned) Tk_Height(tkwin),
            0, 0);

    Tk_FreePixmap(Tk_Display(tkwin), d);
    Tk_FreeGC(Tk_Display(tkwin), gc);
#else
    GdkPixbuf *imgb;
    XGCValues gcValues;
    gcValues.graphics_exposures = False;
    GC gc = Tk_GetGC(tkwin, GCForeground | GCBackground | GCGraphicsExposures,
                     &gcValues);
    imgb = TileGtk_gdk_pixbuf_new(GDK_COLORSPACE_RGB, true, 8, w, h);
    if (!imgb) {
      Tk_FreeGC(Tk_Display(tkwin), gc);
      return;
    }
    imgb = TileGtk_gdk_pixbuf_get_from_drawable(imgb, gdkDrawable,
                                                NULL, x, y, 0, 0, w, h);
    TileGtk_gdk_pixbuf_xlib_render_to_drawable(imgb, d, gc,
         0, 0, x1, x2, w, h, XLIB_RGB_DITHER_MAX, 0, 0);
    TileGtk_gdk_pixbuf_unref(imgb);
    Tk_FreeGC(Tk_Display(tkwin), gc);
#endif

/*
 * How to convert an X drawable to a GtkDrawable!
 *
  GdkPixmap    *gdkDrawable = NULL; \
  GtkStyle     *style       = NULL; \
  GdkScreen    *screen      = NULL; \
  GdkColormap  *cmap        = NULL; \
  screen = TileGtk_gdk_display_get_screen(wc->gdkDisplay, \
                                          Tk_ScreenNumber(tkwin)); \
  printf("Widget: %s, d=%d, p=%p\n", Tk_PathName(tkwin), d, \
          gdk_xid_table_lookup((XID) d));fflush(0); \
  gdkDrawable = TileGtk_gdk_pixmap_foreign_new_for_screen(screen, d, \
          Tk_Width(tkwin), Tk_Height(tkwin), \
          DefaultDepthOfScreen(Tk_Screen(tkwin))); \
  if (!gdkDrawable) return; \
  GdkVisual *visual = TileGtk_gdk_x11_screen_lookup_visual(screen,
                              XVisualIDFromVisual(Tk_Visual(tkwin))); \
  cmap = TileGtk_gdk_x11_colormap_foreign_new(visual, Tk_Colormap(tkwin)); \
  TileGtk_gdk_drawable_set_colormap(gdkDrawable, cmap);\
  printf("d=%d, gdkDrawable=%p, XID=%d, p=%p\n", d, gdkDrawable, \
          gdk_x11_drawable_get_xid(gdkDrawable), \
          gdk_xid_table_lookup((XID) d));fflush(0);
 */
#endif
}; /* TileGtk_CopyGtkPixmapOnToDrawable */

void TileGtk_StoreStyleNameLowers(TileGtk_WidgetCache *wc) {
}; /* TileGtk_StoreStyleName */

bool TileGtk_ThemeIs(TileGtk_WidgetCache *wc, const char* name) {
  return false;
}; /* TileGtk_ThemeIs */

/*
 * TileGtk_StateTableLookup --
 * Look up an index from a statically allocated state table.
 */
unsigned int TileGtk_StateTableLookup(Ttk_StateTable *map, unsigned int state) {
    return Ttk_StateTableLookup(map, state);
    int value = 0;
    while (map->onBits || map->offBits) {
      if (state & map->onBits) value |= map->index;
      ++map;
    }
    return value;
}; /* TileGtk_StateTableLookup */

/*
 * GTK_STATE_NORMAL:      State during normal operation.
 * GTK_STATE_ACTIVE:      State of a currently active widget, such as a
 *                        depressed button.
 * GTK_STATE_PRELIGHT:    State indicating that the mouse pointer is over the
 *                        widget and the widget will respond to mouse clicks.
 * GTK_STATE_SELECTED:    State of a selected item, such the selected row in
 *                        a list.
 * GTK_STATE_INSENSITIVE: State indicating that the widget is unresponsive
 *                        to user actions.
 *
 * GTK_SHADOW_NONE:       No outline.
 * GTK_SHADOW_IN:         The outline is bevelled inwards.
 * GTK_SHADOW_OUT:        The outline is bevelled outwards like a button.
 * GTK_SHADOW_ETCHED_IN:  The outline has a sunken 3d appearance.
 * GTK_SHADOW_ETCHED_OUT: The outline has a raised 3d appearance
 *
 * TTK_STATE_ACTIVE
 * TTK_STATE_DISABLED
 * TTK_STATE_FOCUS
 * TTK_STATE_PRESSED
 * TTK_STATE_SELECTED
 * TTK_STATE_BACKGROUND
 * TTK_STATE_ALTERNATE
 * TTK_STATE_INVALID
 * TTK_STATE_READONLY
 */
unsigned int TileGtk_StateShadowTableLookup(TileGtk_StateTable *map,
             unsigned int state, GtkStateType& gtkState,
             GtkShadowType& gtkShadow, unsigned int section) {
  static TileGtk_StateTable default_map[] = {
    /* Section for: Buttons */
    /* Section common to all widgets */
    {GTK_STATE_ACTIVE,      GTK_SHADOW_NONE, 0,
     TTK_STATE_ACTIVE,      0,               TILEGTK_SECTION_ALL},
    {GTK_STATE_INSENSITIVE, GTK_SHADOW_NONE, 0,
     TTK_STATE_DISABLED,    0,               TILEGTK_SECTION_ALL},
    {GTK_STATE_PRELIGHT,    GTK_SHADOW_NONE, 0,
     TTK_STATE_FOCUS,       0,               TILEGTK_SECTION_ALL},
    {GTK_STATE_SELECTED,    GTK_SHADOW_NONE, 0,
     TTK_STATE_PRESSED,     0,               TILEGTK_SECTION_ALL},
    {GTK_STATE_SELECTED,    GTK_SHADOW_NONE, 0,
     TTK_STATE_SELECTED,    0,               TILEGTK_SECTION_ALL},
    {GTK_STATE_NORMAL,      GTK_SHADOW_NONE, 0,
     TTK_STATE_BACKGROUND,  0,               TILEGTK_SECTION_ALL},
    {GTK_STATE_NORMAL,      GTK_SHADOW_NONE, 0,
     TTK_STATE_ALTERNATE,   0,               TILEGTK_SECTION_ALL},
    {GTK_STATE_INSENSITIVE, GTK_SHADOW_NONE, 0,
     TTK_STATE_INVALID,     0,               TILEGTK_SECTION_ALL},
    {GTK_STATE_INSENSITIVE, GTK_SHADOW_NONE, 0,
     TTK_STATE_READONLY,    0,               TILEGTK_SECTION_ALL},
    {GTK_STATE_NORMAL,      GTK_SHADOW_NONE, 0,
     0,                     0,               TILEGTK_SECTION_ALL}
  };
  unsigned int value = 0;
  gtkState  = GTK_STATE_NORMAL; 
  gtkShadow = GTK_SHADOW_NONE;
  if (!map) {
    map = default_map;
    /* Instead of writting huge tables, do some checks here... */
    if (section & TILEGTK_SECTION_STEPPERS) {
      gtkShadow = GTK_SHADOW_OUT;
      if (state & TTK_STATE_DISABLED) {
        gtkState = GTK_STATE_INSENSITIVE;
      } else if (state & TTK_STATE_PRESSED) {
        gtkState = GTK_STATE_ACTIVE;
        gtkShadow = GTK_SHADOW_IN;
      } else if (state & TTK_STATE_ACTIVE) {
        gtkState = GTK_STATE_PRELIGHT;
      }
      map = NULL; /* Do not search the table */
    } else if (section & TILEGTK_SECTION_BUTTONS ||
               section & TILEGTK_SECTION_ENTRY ||
               section & TILEGTK_SECTION_SCROLLBAR) {
      /* Whether the button is drawn pressed or not, is defined by shadow. */
      if (state & TTK_STATE_PRESSED || state & TTK_STATE_SELECTED) {
        gtkShadow = GTK_SHADOW_IN;
      } else {
        gtkShadow = GTK_SHADOW_OUT;
        // if (state & TTK_STATE_DISABLED || state & TTK_STATE_READONLY)
        //                                    gtkState  = GTK_STATE_INSENSITIVE;
        // else if (state & TTK_STATE_ACTIVE) gtkState  = GTK_STATE_PRELIGHT;
        // else if (state & TTK_STATE_FOCUS)  gtkState  = GTK_STATE_ACTIVE;
      }
      if ((state & TTK_STATE_ACTIVE) &&
             (!(state & TTK_STATE_PRESSED) || !(state & TTK_STATE_SELECTED))) {
        gtkState = GTK_STATE_PRELIGHT;
      } else {
        if (state & TTK_STATE_DISABLED || state & TTK_STATE_READONLY)
          gtkState  = GTK_STATE_INSENSITIVE;
        else if (state & TTK_STATE_PRESSED) gtkState  = GTK_STATE_ACTIVE;
      }
      map = NULL; /* Do not search the table */
    } else if (section & TILEGTK_SECTION_TROUGH) {
      if (state & TTK_STATE_PRESSED) {
        gtkState = GTK_STATE_ACTIVE;
      } else {
        gtkState = GTK_STATE_INSENSITIVE;
      }
      gtkShadow = GTK_SHADOW_IN;
      map = NULL; /* Do not search the table */
    } else if (section & TILEGTK_SECTION_SASH) {
      if (state & TTK_STATE_DISABLED || state & TTK_STATE_READONLY)
                                         gtkState  = GTK_STATE_INSENSITIVE;
      else if (state & TTK_STATE_FOCUS)  gtkState  = GTK_STATE_SELECTED;
      else if (state & TTK_STATE_ACTIVE) gtkState  = GTK_STATE_PRELIGHT;
      map = NULL; /* Do not search the table */
    } else if (section & TILEGTK_SECTION_TABS) {
      gtkState  = GTK_STATE_ACTIVE;
      gtkShadow = GTK_SHADOW_OUT;
      if (state & TTK_STATE_DISABLED || state & TTK_STATE_READONLY)
                                            gtkState  = GTK_STATE_INSENSITIVE;
      else if (state & TTK_STATE_SELECTED)  gtkState  = GTK_STATE_NORMAL;
      else if (state & TTK_STATE_ACTIVE)    gtkState  = GTK_STATE_PRELIGHT;
      else if (state & TTK_STATE_FOCUS)     gtkState  = GTK_STATE_ACTIVE;
      map = NULL; /* Do not search the table */
    }
  }
  if (map) {
    while ( (state & map->onBits)  != map->onBits  ||
           (~state & map->offBits) != map->offBits ||
           !(section & map->section)) {
      ++map; ++value;
    }
    gtkState  = map->state;
    gtkShadow = map->shadow;
  }
  return (map)? map->value : value;
}; /* TileGtk_StateShadowTableLookup */

double TileGtk_ValueFromSlider(TileGtk_WidgetCache *wc, Tk_Window tkwin,
                               Ttk_Box b) {
  double value = 0.0;
  double ww = Tk_Width(tkwin), wh = Tk_Height(tkwin), dx = 0.0, x;
  if (wc->orientation == TTK_ORIENT_HORIZONTAL) {
    dx = b.width / 2.0;
    x = b.x + dx;
    value = x / ww;
  } else {
    dx = b.height / 2.0;
    x = b.y + dx;
    value = x / wh;
  }
  if (value < 0.0) value = 0.0;
  if (value > 1.0) value = 1.0;
  // printf("ww:%f, b.x:%d, x:%f, dx:%f, v:%f\n", ww, b.x, x, dx, value);
  return value;
}; /* TileGtk_ValueFromSlider */
