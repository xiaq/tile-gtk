/*
 *  tileGtk_Utilities.h
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
#include "tileGtk_Elements.h"
#include <string.h>

#define NO_GTK_STYLE_ENGINE {/*printf("NULL qApp\n");fflush(NULL);*/return;}

#define TILEGTK_ENSURE_GTK_STYLE_ENGINE_ACTIVE \
  if (!TileGtk_GtkInitialised()) return;

#define TILEGTK_ENSURE_WIDGET_OK \
  if (!widget) return;

#define TILEGTK_ATTACH_STYLE_TO_WIDGET \
  style = TileGtk_gtk_style_attach(style, widget->window);

#define TILEGTK_WIDGET_CACHE_DEFINITION \
  TileGtk_WidgetCache *wc = (TileGtk_WidgetCache *) clientData;

#define TILEGTK_ORIENTATION_DEFINITION \
  int orientation = wc->orientation;

#define TILEGTK_GTK_DRAWABLE_DEFINITIONS \
  TileGtk_WidgetCache *wc = (TileGtk_WidgetCache *) clientData; \
  GdkPixmap    *gdkDrawable = NULL; \
  GtkStyle     *style       = NULL; \
  GtkStateType  gtkState    = GTK_STATE_NORMAL; \
  GtkShadowType gtkShadow   = GTK_SHADOW_NONE;

#define TILEGTK_SETUP_GTK_DRAWABLE \
  TILEGTK_SETUP_GTK_DRAWABLE_PIXMAP_SIZE(b.width, b.height)

#define TILEGTK_SETUP_GTK_DRAWABLE_PIXMAP_SIZE(pw, ph) \
  if (!wc) return; \
  style = TileGtk_GetGtkWindowStyle(wc->gtkWindow); \
  if (!style) return; \
  gdkDrawable = TileGtk_gdk_pixmap_new(wc->gtkWindow->window, pw, ph, -1); \
  style = TileGtk_GetGtkWindowStyle(wc->gtkWindow);

#define TILEGTK_STYLE_BACKGROUND_DEFAULT \
  if (wc && wc->gtkWindow) style = TileGtk_GetGtkWindowStyle(wc->gtkWindow); \
  if (!style) return;

#define TILEGTK_STYLE_FROM_WIDGET \
  style = TileGtk_GetGtkWindowStyle(widget); \
  if (!style) style = TileGtk_GetGtkWindowStyle(wc->gtkWindow); \
  if (!style) return;

#define TILEGTK_DRAWABLE_FROM_WIDGET_SIZE(pw, ph) \
  gdkDrawable = TileGtk_gdk_pixmap_new(widget->window, pw, ph, -1);

#define TILEGTK_DRAWABLE_FROM_WIDGET \
  TILEGTK_DRAWABLE_FROM_WIDGET_SIZE(b.width, b.height)

// #define TILEGTK_DRAWABLE_FROM_WIDGET \
//   gdkDrawable = gdk_pixmap_foreign_new(Tk_WindowId(tkwin)); \
//   GdkColormap gdkColormap = gdk_x11_colormap_foreign_new(gdkx_visual_get(Tk_Visual(tkwin)), Tk_Colormap(tkwin)); \
//   gdk_drawable_set_colormap(gdkDrawable, gdkColormap);

#define TILEGTK_DEFAULT_BACKGROUND_SIZE(pw, ph) \
  TileGtk_gtk_style_apply_default_background(style, gdkDrawable, TRUE, \
               gtkState, NULL, 0, 0, pw, ph);

#define TILEGTK_DEFAULT_BACKGROUND \
  TILEGTK_DEFAULT_BACKGROUND_SIZE(b.width, b.height)

#define TILEGTK_CLEANUP_GTK_DRAWABLE \
  if (gdkDrawable) TileGtk_g_object_unref(gdkDrawable);

#define TILEGTK_SETUP_STATE_SHADOW(statemap, shadowmap) \
    gtkState  = (GtkStateType) \
       TileGtk_StateTableLookup(statemap,  state); \
    gtkShadow = (GtkShadowType) \
       TileGtk_StateTableLookup(shadowmap, state);

#define TILEGTK_SETUP_WIDGET_SIZE(width, height) \
  TileGtk_gtk_widget_set_size_request(widget, width, height);

#define TILEGTK_GET_WIDGET_SIZE(widthPtr, heightPtr)  \
  if (widget) { \
    GtkRequisition size; \
    TileGtk_gtk_widget_size_request(widget, &size); \
    widthPtr  = size.width; \
    heightPtr = size.height; \
  }

#define TILEGTK_WIDGET_SETUP_DEFAULT(obj) \
  int defaultState  = TTK_BUTTON_DEFAULT_DISABLED; \
  int has_default = (defaultState == TTK_BUTTON_DEFAULT_ACTIVE); \
  /*Ttk_GetButtonDefaultStateFromObj(NULL, obj, &defaultState);*/

#ifdef TILEGTK_LOAD_GTK_DYNAMICALLY

#define TILEGTK_WIDGET_SET_FOCUS(widget)

#define TILEGTK_WIDGET_SET_DEFAULT(widget, obj) \
  int defaultState  = TTK_BUTTON_DEFAULT_DISABLED; \
  int has_default = (defaultState == TTK_BUTTON_DEFAULT_ACTIVE);

#else /* TILEGTK_LOAD_GTK_DYNAMICALLY */

#define TILEGTK_WIDGET_SET_FOCUS(widget) \
  if (state & TTK_STATE_FOCUS) { \
    GTK_WIDGET_SET_FLAGS(widget,   GTK_HAS_FOCUS); \
  } else { \
    GTK_WIDGET_UNSET_FLAGS(widget, GTK_HAS_FOCUS); \
  }

#define TILEGTK_WIDGET_SET_DEFAULT(widget, obj) \
  int defaultState  = TTK_BUTTON_DEFAULT_DISABLED; \
  int has_default = (defaultState == TTK_BUTTON_DEFAULT_ACTIVE); \
  /*Ttk_GetButtonDefaultStateFromObj(NULL, obj, &defaultState);*/ \
  if (has_default) { \
    GTK_WIDGET_SET_FLAGS(widget,   GTK_HAS_DEFAULT); \
  } else { \
    GTK_WIDGET_UNSET_FLAGS(widget, GTK_HAS_DEFAULT); \
  } 
#endif /* TILEGTK_LOAD_GTK_DYNAMICALLY */

#define TILEGTK_DEBUG_PRINT_BOX \
  printf("x=%d, y=%d, w=%d, h=%d\n", b.x, b.y, b.width, b.height); \
  fflush(0);

#define TILEGTK_DEBUG_PRINT_TK_WIDGET \
  printf("Widget: %s,p=%p\n", Tk_PathName(tkwin), tkwin); \
  fflush(0);

#define TILEGTK_GTKBORDER_TO_PADDING(border) \
  Ttk_MakePadding(border.left, border.top, border.right, border.bottom)

TCL_DECLARE_MUTEX(tilegtkMutex);

/* Global Symbols */

/* Helper Functions */
extern int        TileGtk_GtkInitialised(void);
extern GtkWidget *TileGtk_GetGtkWindow(void);
extern GtkStyle  *TileGtk_GetGtkWindowStyle(GtkWidget *gtkWindow);
extern GtkStyle  *TileGtk_GetGtkStyle(void);

extern unsigned int TileGtk_StateTableLookup(Ttk_StateTable *, unsigned int);
extern void TileGtk_CopyGtkPixmapOnToDrawable(GdkPixmap *, Drawable, Tk_Window,
                                            int, int, int, int, int, int);
extern void TileGtk_StateInfo(int, GtkStateType,
                    GtkShadowType, Tk_Window, GtkWidget *widget = NULL);

extern GtkWidget *TileGtk_GetArrow(TileGtk_WidgetCache* wc);
extern GtkWidget *TileGtk_GetButton(TileGtk_WidgetCache* wc);
extern GtkWidget *TileGtk_GetCheckButton(TileGtk_WidgetCache* wc);
extern GtkWidget *TileGtk_GetRadioButton(TileGtk_WidgetCache* wc);
extern GtkWidget *TileGtk_GetToolBar(TileGtk_WidgetCache* wc);
extern GtkWidget *TileGtk_GetToolButton(TileGtk_WidgetCache* wc);
extern GtkWidget *TileGtk_GetFrame(TileGtk_WidgetCache* wc);
extern GtkWidget *TileGtk_GetEntry(TileGtk_WidgetCache* wc);
extern GtkWidget *TileGtk_GetCombobox(TileGtk_WidgetCache* wc);
extern GtkWidget *TileGtk_GetComboboxEntry(TileGtk_WidgetCache* wc);
extern GtkWidget *TileGtk_GetHScrollBar(TileGtk_WidgetCache* wc);
extern GtkWidget *TileGtk_GetVScrollBar(TileGtk_WidgetCache* wc);
extern GtkWidget *TileGtk_GetScrollBar(TileGtk_WidgetCache* wc);
extern GtkWidget *TileGtk_GetHScale(TileGtk_WidgetCache* wc);
extern GtkWidget *TileGtk_GetVScale(TileGtk_WidgetCache* wc);
extern GtkWidget *TileGtk_GetScale(TileGtk_WidgetCache* wc);
extern GtkWidget *TileGtk_GetHProgressBar(TileGtk_WidgetCache* wc);
extern GtkWidget *TileGtk_GetVProgressBar(TileGtk_WidgetCache* wc);
extern GtkWidget *TileGtk_GetProgressBar(TileGtk_WidgetCache* wc);
extern GtkWidget *TileGtk_GetStatusBar(TileGtk_WidgetCache* wc);
extern GtkWidget *TileGtk_GetPaned(TileGtk_WidgetCache* wc);
extern GtkWidget *TileGtk_GetNotebook(TileGtk_WidgetCache* wc);
#if 0
extern void TileGtk_StoreStyleNameLowers(TileGtk_WidgetCache *wc);
extern bool TileGtk_ThemeIs(TileGtk_WidgetCache *wc, const char* name);
extern void TileGtk_SetFocus(bool focus);
#endif

extern unsigned int TileGtk_StateShadowTableLookup(TileGtk_StateTable*,
       unsigned int, GtkStateType&, GtkShadowType&,
       unsigned int section = TILEGTK_SECTION_ALL);
extern double TileGtk_ValueFromSlider(TileGtk_WidgetCache *wc, Tk_Window tkwin,
                               Ttk_Box b);
