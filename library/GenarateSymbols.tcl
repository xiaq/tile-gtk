##
## Function to return the containing lib of a symbol
##
proc symbol2lib {s} {
  switch -glob $s {
    gdk_pixbuf_get_from_drawable {return gdk}
    gtk_*             {return gtk}
    g_object_*        {return gobject}
    g_*               {return glib}
    xlib_rgb_init -
    gdk_pixbuf_xlib_* {return gdk_pixbuf_xlib}
    gdk_pixbuf_*      {return gdk_pixbuf}
    gdk_*             {return gdk}
    xlib_*            {return xlib}
    default {
      puts $s
    }
  }
  return unknown
};# symbol2lib

##
## Function name to return type/arguments map.
##
proc get_function_spec {f} {
  set t {}; set a {}
  switch -glob $f {
    g_free {
      set a {gpointer mem}
    }
    g_hash_table_get_keys {
      set t {GList*}
      set a {GHashTable *hash_table}
    }
    g_malloc {
      set t {gpointer}
      set a {gsize n_bytes}
    }
    g_malloc0 {
      set t {gpointer}
      set a {gsize n_bytes}
    }
    g_new0 {
      set t {}
      set a {}
      puts "FIXME: $f"
    }
    g_object_get {
      set a {gpointer object, const gchar *first_property_name, ...}
    }
    g_object_ref {
      set a {gpointer object}
    }
    g_object_unref {
      set a {gpointer object}
    }
    g_option_context_add_main_entries {
      set a {GOptionContext *context, const GOptionEntry *entries, const gchar *translation_domain}
    }
    g_option_context_new {
      set t {GOptionContext*}
      set a {const gchar *parameter_string}
    }
    g_strfreev {
      set a {gchar **str_array}
    }
    gdk_color_to_string {
      set t {gchar*}
      set a {const GdkColor *color}
    }
    gdk_display_get_default {
      set t {GdkDisplay*}
    }
    gdk_display_get_screen {
      set t {GdkScreen*}
      set a {GdkDisplay*, gint}
    }
    gdk_drawable_unref {
      set a {GdkDrawable *drawable}
    }
    gdk_drawable_get_colormap {
      set t {GdkColormap*}
      set a {GdkDrawable *drawable}
    }
    gdk_drawable_set_colormap {
      set a {GdkColormap*}
    }
    gdk_pixmap_foreign_new {
      set t {GdkPixmap*}
      set a {GdkNativeWindow anid}
    }
    gdk_pixmap_foreign_new_for_screen {
      set t {GdkPixmap*}
      set a {GdkScreen *screen, GdkNativeWindow anid, gint width, gint height, gint depth}
    }
    gdk_gc_new {
      set t {GdkGC*}
      set a {GdkDrawable *drawable}
    }
    gdk_pixbuf_get_from_drawable {
      set t {GdkPixbuf*}
      set a {GdkPixbuf *dest, GdkDrawable *src, GdkColormap *cmap, int src_x, int src_y, int dest_x, int dest_y, int width, int height}
    }
    gdk_pixbuf_new {
      set t {GdkPixbuf*}
      set a {GdkColorspace colorspace, gboolean has_alpha, int bits_per_sample, int width, int height}
    }
    gdk_pixbuf_unref {
      set a {GdkPixbuf *pixbuf}
    }
    gdk_pixbuf_xlib_render_to_drawable {
      set a {GdkPixbuf *pixbuf, Drawable drawable, GC gc, int src_x, int src_y, int dest_x, int dest_y, int width, int height, XlibRgbDither dither, int x_dither, int y_dither}
    }
    gdk_pixmap_new {
      set t {GdkPixmap*}
      set a {GdkDrawable *drawable, gint width, gint height, gint depth}
    }
    gdk_pixmap_foreign_new_for_display {
      set t {GdkPixmap*}
      set a {GdkDisplay *display, GdkNativeWindow anid}
    }
    gdk_win32_hdc_get {
      set t {HDC}
      set a {GdkDrawable *drawable, GdkGC *gc, GdkGCValuesMask usage}
    }
    gdk_win32_hdc_release {
      set a {GdkDrawable *drawable, GdkGC *gc, GdkGCValuesMask usage}
    }
    gdk_x11_colormap_foreign_new {
      set t {GdkColormap*}
      set a {GdkVisual *visual, Colormap xcolormap}
    }
    gdk_x11_lookup_xdisplay {
      set t {GdkDisplay*}
      set a {Display *xdisplay}
    }
    gdk_x11_screen_lookup_visual {
      set t {GdkVisual*}
      set a {GdkScreen *screen, VisualID xvisualid}
    }
    xlib_rgb_init {
      set a {Display *display, Screen *screen}
    }
    gtk_adjustment_new {
      set t {GtkObject*}
      set a {double, double, double, double, double, double}
    }
    gtk_adjustment_set_value {
      set a {GtkAdjustment *adjustment, gdouble value}
    }
    gtk_arrow_new {
      set a {GtkArrowType at, GtkShadowType st}
      set t {GtkWidget*}
    }
    gtk_tool_button_new {
      set t {GtkToolItem*}
      set a {GtkWidget *icon_widget, const gchar *label}
    }
    gtk_vpaned_new -
    gtk_toolbar_new -
    gtk_statusbar_new -
    gtk_progress_bar_new -
    gtk_notebook_new -
    gtk_hpaned_new -
    gtk_fixed_new -
    gtk_entry_new -
    gtk_combo_box_new -
    gtk_combo_box_entry_new -
    gtk_check_button_new -
    gtk_button_new {
      set t {GtkWidget*}
    }
    gtk_button_get_relief {
      set t {GtkReliefStyle}
      set a {GtkButton *button}
    }
    gtk_button_set_relief {
      set t {}
      set a {GtkButton *button, GtkReliefStyle newstyle}
    }
    gtk_border_free {
      set a {GtkBorder*}
    }
    gtk_container_add {
      set a {GtkContainer *container, GtkWidget *widget}
    }
    gtk_disable_setlocale {
    }
    gtk_frame_new {
      set t {GtkWidget*}
      set a {const gchar *label}
    }
    gtk_hscale_new_with_range {
      set t {GtkWidget*}
      set a {gdouble min, gdouble max, gdouble step}
    }
    gtk_hscrollbar_new {
      set t {GtkWidget*}
      set a {GtkAdjustment *}
    }
    gtk_init_check {
      set t {gboolean}
      set a {int *argc, char ***argv}
    }
    gtk_object_get {
      set a {GtkObject *object, const gchar *first_property_name, ...}
    }
    gtk_paint_arrow {
      set a {GtkStyle*,GdkWindow*, GtkStateType, GtkShadowType, const GdkRectangle *, GtkWidget *, const gchar *, GtkArrowType, gboolean, gint , gint , gint , gint}
    }
    gtk_paint_box {
      set a {GtkStyle*,GdkWindow*, GtkStateType, GtkShadowType, const GdkRectangle *, GtkWidget *, const gchar *, gint , gint , gint , gint}
    }
    gtk_paint_box_gap {
      set a {GtkStyle*,GdkWindow*, GtkStateType, GtkShadowType, const GdkRectangle *, GtkWidget *, const gchar *, gint, gint, gint , gint, GtkPositionType, gint gap_x, gint gap_width}
    }
    gtk_paint_check {
      set a {GtkStyle*,GdkWindow*, GtkStateType, GtkShadowType, const GdkRectangle *, GtkWidget *, const gchar *, gint , gint , gint , gint}
    }
    gtk_paint_extension {
      set a {GtkStyle *, GdkWindow *, GtkStateType, GtkShadowType, const GdkRectangle *, GtkWidget *, const gchar *, gint, gint, gint, gint, GtkPositionType}
    }
    gtk_paint_flat_box {
      set a {GtkStyle*,GdkWindow*, GtkStateType, GtkShadowType, const GdkRectangle *, GtkWidget *, const gchar *, gint , gint , gint , gint}
    }
    gtk_paint_focus {
      set a {GtkStyle *style, GdkWindow *window, GtkStateType state_type, GdkRectangle *area, GtkWidget *widget, const gchar *detail, gint x, gint y, gint width, gint height}
    }
    gtk_paint_handle {
      set a {GtkStyle*,GdkWindow*, GtkStateType, GtkShadowType, const GdkRectangle *, GtkWidget *, const gchar *, gint , gint , gint , gint, GtkOrientation}
    }
    gtk_paint_option {
      set a {GtkStyle*,GdkWindow*, GtkStateType, GtkShadowType, const GdkRectangle *, GtkWidget *, const gchar *, gint , gint , gint , gint}
    }
    gtk_paint_resize_grip {
      set a {GtkStyle*,GdkWindow*, GtkStateType, const GdkRectangle *, GtkWidget *, const gchar *, GdkWindowEdge, gint , gint , gint , gint}
    }
    gtk_paint_shadow {
      set a {GtkStyle*,GdkWindow*, GtkStateType, GtkShadowType, const GdkRectangle *, GtkWidget *, const gchar *, gint , gint , gint , gint}
    }
    gtk_paint_slider {
      set a {GtkStyle*,GdkWindow*, GtkStateType, GtkShadowType, const GdkRectangle *, GtkWidget *, const gchar *, gint , gint , gint , gint, GtkOrientation}
    }
    gtk_progress_bar_set_bar_style {
      set a {GtkProgressBar *pbar, GtkProgressBarStyle style}
    }
    gtk_progress_bar_set_fraction {
      set a {GtkProgressBar *pbar, gdouble fraction}
    }
    gtk_progress_bar_set_orientation {
      set a {GtkProgressBar *pbar, GtkProgressBarOrientation orientation}
    }
    gtk_range_get_adjustment {
      set t {GtkAdjustment *}
      set a {GtkRange *range}
    }
    gtk_rc_get_default_files {
      set t {gchar**}
    }
    gtk_rc_get_style {
      set t {GtkStyle *}
      set a {GtkWidget*}
    }
    gtk_rc_get_theme_dir {
      set t {gchar*}
    }
    gtk_rc_reparse_all_for_settings {
      set t {gboolean}
      set a {GtkSettings *settings, gboolean force_load}
    }
    gtk_rc_reset_styles {
      set a {GtkSettings *settings}
    }
    gtk_rc_set_default_files {
      set a {gchar **filenames}
    }
    gtk_set_locale {
      set t {gchar*}
    }
    gtk_settings_get_default {
      set t {GtkSettings *}
    }
    gtk_style_apply_default_background {
      set a {GtkStyle *style, GdkWindow *window, gboolean set_bg, GtkStateType state_type, GdkRectangle *area, gint x, gint y, gint width, gint height}
    }
    gtk_style_attach {
      set t {GtkStyle*}
      set a {GtkStyle *, GdkWindow *}
    }
    gtk_style_lookup_color {
      set t {gboolean}
      set a {GtkStyle *style, const gchar *color_name, GdkColor *color}
    }
    gtk_vscrollbar_new {
      set t {GtkWidget*}
      set a {GtkAdjustment *}
    }
    gtk_vscale_new_with_range {
      set t {GtkWidget*}
      set a {gdouble min, gdouble max, gdouble step}
    }
    gtk_widget_realize -
    gtk_widget_destroy {
      set a {GtkWidget *}
    }
    gtk_widget_set_size_request {
      set a {GtkWidget *widget, gint width, gint height}
    }
    gtk_widget_size_request {
      set a {GtkWidget *widget, GtkRequisition *requisition}
    }
    gtk_widget_style_get {
      set a {GtkWidget *, const gchar *first_property_name, ...}
    }
    gdk_window_destroy {
      set a {(GdkWindow *}
    }
    gdk_window_foreign_new {
      set t {GdkWindow*}
      set a {GdkNativeWindow}
    }
    gtk_window_new {
      set t {GtkWidget*}
      set a {GtkWindowType}
    }
    default {
      puts "    $f \{\n      set t {}\n      set a {}\n    \}"
    }
  }
  if {![string length $t]} {set t void}
  if {![string length $a]} {set a void}
  return [list $t $a]
};# get_function_spec

set HomeDir [file dirname [file normalize [info script]]]

set sources [file dirname $HomeDir]/generic

##
## Force the following symbols to exist!
##
foreach s {g_malloc0 g_malloc} {
  set Symbols(TileGtk_$s) 1
}

proc process {data} {
  global Symbols
  set skip {[^)(\",=]+}
  foreach lib {gtk gdk xlib g} {
    foreach s [regexp -inline -all "TileGtk_${lib}_${skip}\\\s*\\\(" $data] {
      set Symbols([string trim $s { ()}]) 1
    }
  }
}

foreach file [lsort [glob -directory $sources -type f *]] {
  if {[string match *tileGtk_Symbols* $file ]} continue
  set fd [open $file]
  fconfigure $fd -encoding utf-8
  set data [read $fd]
  close $fd
  process $data
}

## Remove definitions...
foreach s {g_new0} {
  unset -nocomplain Symbols(TileGtk_$s)
}

switch $::tcl_platform(platform) {
  windows {
    set not_native {gdk_pixbuf_xlib_render_to_drawable xlib_rgb_init
                    gdk_x11_lookup_xdisplay}
  }
  unix {
    set not_native {gdk_win32_hdc_get gdk_win32_hdc_release}
  }
  default {
  }
}
foreach s $not_native {
  unset -nocomplain Symbols(TileGtk_$s)
}

##
## Generate file tileGtk_Symbols.cpp
##
set fc [open $sources/tileGtk_Symbols.cpp w]
fconfigure $fc -encoding iso8859-1
puts $fc \
{/*
 *  tileGtk_Symbols.cpp
 * ------------------------
 *
 * This file is part of the Tile-Gtk package, a Tk/Tile based theme that uses
 * Gtk/GNOME for drawing.
 *
 * Copyright (C) 2004-2012 by:
 * Georgios Petasis, petasis@iit.demokritos.gr,
 * Software and Knowledge Engineering Laboratory,
 * Institute of Informatics and Telecommunications,
 * National Centre for Scientific Research (NCSR) "Demokritos",
 * Aghia Paraskevi, 153 10, Athens, Greece.
 */
/* THIS FILE IS AUTOMATICALLY GENERATED - DO NOT EDIT! */
#include "tileGtk_Symbols.h"
#ifdef    TILEGTK_LOAD_GTK_DYNAMICALLY
#if (TCL_MAJOR_VERSION >= 8) && (TCL_MINOR_VERSION >= 6)
#define LOAD_SYMBOLS_WITH_TCL
#endif

#ifndef LOAD_SYMBOLS_WITH_TCL
#include <gmodule.h>
#endif
}

##
## Generate file tileGtk_Symbols.h
##

set fd [open $sources/tileGtk_Symbols.h w]
fconfigure $fd -encoding iso8859-1
puts $fd \
{/*
 *  tileGtk_Symbols.h
 * --------------------
 *
 * This file is part of the Tile-Gtk package, a Tk/Tile based theme that uses
 * Gtk/GNOME for drawing.
 *
 * Copyright (C) 2004-2012 by:
 * Georgios Petasis, petasis@iit.demokritos.gr,
 * Software and Knowledge Engineering Laboratory,
 * Institute of Informatics and Telecommunications,
 * National Centre for Scientific Research (NCSR) "Demokritos",
 * Aghia Paraskevi, 153 10, Athens, Greece.
 */
/* THIS FILE IS AUTOMATICALLY GENERATED - DO NOT EDIT! */
#ifndef _TILEGTK_SYMBOLS
#define _TILEGTK_SYMBOLS
#include "tcl.h"
#ifdef LOAD_SYMBOLS_WITH_TCL
#else
#include <glib.h>
#endif
#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#ifndef __WIN32__
#include <gdk-pixbuf-xlib/gdk-pixbuf-xlib.h>
#include <gdk/gdkx.h>
#else
#include <gdk/gdkwin32.h>
#endif

#ifdef    TILEGTK_LOAD_GTK_DYNAMICALLY}
puts $fd "extern \"C\" \{"

if {1} {
  foreach symbol [lsort -dictionary [array names Symbols]] {
    set gtk_s [string range $symbol 8 end]
    set lib [symbol2lib $gtk_s]
    foreach {t a} [get_function_spec $gtk_s] {break}
    puts $fd "  typedef $t (*Ptr_$gtk_s)($a);"
    puts $fd "  extern Ptr_$gtk_s $symbol;"
    puts $fc "Ptr_$gtk_s $symbol = 0;"
    lappend GTKSymbols($lib) $gtk_s;
    #puts $fd "/*\n * $symbol: Stub for [string range $symbol 8 end]\n */"
    #puts $fd "${symbol}() {\n  TILEGTK_ENSURE\n}; /* $symbol */\n"
  }
}

puts $fd "\}\n"

proc initialise_lib {lib} {
  global fc fd GTKSymbols
  puts $fd "extern int\
            TileGtk_InitialiseSymbols_${lib}(Tcl_Interp *interp, Tcl_Obj *file_name);"
  puts $fc "\nint\
            TileGtk_InitialiseSymbols_${lib}(Tcl_Interp *interp, Tcl_Obj *file_name) \{"
  puts $fc "#ifdef LOAD_SYMBOLS_WITH_TCL"
  puts $fc "  Tcl_LoadHandle loadHandle;" 
  puts $fc "  int status;" 
  puts $fc "  status = Tcl_LoadFile(interp, file_name, NULL, 0, NULL, &loadHandle);"
  puts $fc "  if (status != TCL_OK) return status;"
  foreach s $GTKSymbols($lib) {
    puts $fc "  TileGtk_$s = (Ptr_$s) Tcl_FindSymbol(interp, loadHandle, \"$s\");"
    puts $fc "  if (TileGtk_$s == NULL) return TCL_ERROR;"
  }
  puts $fc "#else"
  puts $fc "  const char *fname = Tcl_GetString(file_name);"
  puts $fc "  GModule *m = g_module_open(fname, G_MODULE_BIND_LAZY);"
  puts $fc "  if (!m) {"
  puts $fc "    Tcl_SetResult(interp, (char *) \"cannot open module: \", TCL_STATIC);"
  puts $fc "    Tcl_AppendResult(interp, fname, (char *) NULL);"
  puts $fc "    return TCL_ERROR;"
  puts $fc "  }"
  foreach s $GTKSymbols($lib) {
    puts $fc "  if (!g_module_symbol(m, \"$s\", (gpointer *) &TileGtk_$s)) {"
    puts $fc "    Tcl_SetResult(interp, (char *) \"cannot find symbol: \", TCL_STATIC);"
    puts $fc "    Tcl_AppendResult(interp, (char *) \"$s\", (char *) NULL);"
    puts $fc "    return TCL_ERROR;"
    puts $fc "  }"
  }
  puts $fc "#endif"
  puts $fc "  return TCL_OK;\n\}; /* TileGtk_InitialiseSymbols_${lib} */\n"
  puts $fd "#define TILEGTK_LAST_SYMBOL_$lib TileGtk_$s"
};# initialise_lib
foreach lib [lsort [array names GTKSymbols]] {
  initialise_lib $lib
}


puts $fd {
#else  /* TILEGTK_LOAD_GTK_DYNAMICALLY */
}

##
## Definitions for linking with GTK...
##
if {1} {
  foreach symbol [lsort -dictionary [array names Symbols]] {
    puts $fd "#define $symbol \\\n               \
              [string range $symbol 8 end]"
  }
}

puts $fd \
{#endif /* TILEGTK_LOAD_GTK_DYNAMICALLY */

#endif /* _TILEGTK_SYMBOLS */}
close $fd

puts $fc \
{#endif /* TILEGTK_LOAD_GTK_DYNAMICALLY */}
close $fc
