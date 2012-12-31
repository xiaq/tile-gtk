/*
 * $Id: tileGtk_Init.cpp,v 1.15 2012/07/22 19:26:14 petasis Exp $
 *
 * Copyright (C) 2004-2008 Georgios Petasis
 *
 * The Tile-Gtk theme is a Tk/Tile theme that uses Gtk/GNOME for drawing.
 */

#include "tileGtk_Utilities.h"
#include "tileGtk_TkHeaders.h"
#include <string.h>

static int TileGtk_GtkAppCreated = 0;

extern TileGtk_WidgetCache **TileGtk_CreateGtkApp(Tcl_Interp *interp);
extern void TileGtk_DestroyGtkApp(void);

static char initScript[] =
  "namespace eval tilegtk { };"
  "namespace eval ttk::theme::tilegtk { variable version "
                                                 PACKAGE_VERSION " };"
  "tcl_findLibrary tilegtk $ttk::theme::tilegtk::version "
  "$ttk::theme::tilegtk::version tilegtk.tcl TILEGTK_LIBRARY tilegtk::library;";
#ifdef TILEGTK_LOAD_GTK_DYNAMICALLY
static char libsInitScript[] =
  "ttk::theme::tilegtk::loadLibraries";
#endif /* TILEGTK_LOAD_GTK_DYNAMICALLY */

/*
 * Exit Handler.
 */
static void TileGtk_ExitProc(ClientData data) {
  //Tcl_MutexLock(&tilegtkMutex);
  //TileGtk_DestroyGtkApp();
  //Tcl_MutexUnlock(&tilegtkMutex);
  Tcl_MutexLock(&tilegtkMutex);
  // printf("TileGtk_ExitProc: %d\n", TileGtk_GtkAppCreated); fflush(NULL);
  if (TileGtk_GtkAppCreated < 0) {
    Tcl_MutexUnlock(&tilegtkMutex);
    return;
  }
  --TileGtk_GtkAppCreated;
  if (TileGtk_GtkAppCreated == 0) {
    // printf("TileGtk_ExitProc: %d <- TileGtk_DestroyGtkApp();\n",
    //       TileGtk_GtkAppCreated); fflush(NULL);
    TileGtk_DestroyGtkApp();
  }
  Tcl_MutexUnlock(&tilegtkMutex);
  return;
}; /* TileGtk_ExitProc */

/*
 * Helper Functions
 */
int Tilegtk_ThemeName(ClientData clientData, Tcl_Interp *interp,
                                 int objc, Tcl_Obj *const objv[]) {
  // TileGtk_WidgetCache **wc_array = (TileGtk_WidgetCache **) clientData;
  // TileGtk_WidgetCache *wc;
  // if (!wc_array || !wc_array[0]) {
  //   Tcl_SetResult(interp, (char *) "empty wc_array!", TCL_STATIC);
  //   return TCL_ERROR;
  // }
  // wc = wc_array[0];
  // if (!wc->gtkStyle) {
  //   Tcl_SetResult(interp, (char *) "empty wc->gtkStyle!", TCL_STATIC);
  //   return TCL_ERROR;
  // }
  GtkSettings *settings = NULL;
  gchar       *strval = NULL;
  if (objc != 1) {Tcl_WrongNumArgs(interp, 1, objv, ""); return TCL_ERROR;}

  Tcl_MutexLock(&tilegtkMutex);
  settings = TileGtk_gtk_settings_get_default();
  if (settings) {
    TileGtk_g_object_get(settings, "gtk-theme-name", &strval, NULL);
    if (strval) {
      Tcl_SetResult(interp, (char *) strval, TCL_VOLATILE);
      TileGtk_g_free(strval);
    }
  }
  Tcl_MutexUnlock(&tilegtkMutex);
  return TCL_OK;
}; /* Tilegtk_ThemeName */

int Tilegtk_SettingsProperty(ClientData clientData, Tcl_Interp *interp,
                                 int objc, Tcl_Obj *const objv[]) {
  static const char *Methods[] = {
    "integer", "boolean", "string", (char *) NULL
  };
  enum methods {
    INTEGER, BOOLEAN, STRING
  };
  int type = STRING;
  GtkSettings *settings = NULL;
  gchar       *s_val = NULL;
  gboolean     b_val = FALSE;
  gint         i_val = 0;
  if (objc != 2 && objc != 3) {
    Tcl_WrongNumArgs(interp, 1, objv, "property ?integer|boolean|string?");
    return TCL_ERROR;
  }
  if (objc == 3) {
    if (Tcl_GetIndexFromObj(interp, objv[2], (const char **) Methods,
                            "method", 0, &type) != TCL_OK) {
      return TCL_ERROR;
    }
  }

  Tcl_MutexLock(&tilegtkMutex);
  settings = TileGtk_gtk_settings_get_default();
  if (settings) {
    switch ((enum methods) type) {
      case INTEGER:
        TileGtk_g_object_get(settings, Tcl_GetString(objv[1]), &i_val, NULL);
        Tcl_SetObjResult(interp, Tcl_NewIntObj(i_val));
        break;
      case BOOLEAN:
        TileGtk_g_object_get(settings, Tcl_GetString(objv[1]), &b_val, NULL);
        if (b_val) Tcl_SetObjResult(interp, Tcl_NewBooleanObj(1));
        else Tcl_SetObjResult(interp, Tcl_NewBooleanObj(0));
        break;
      case STRING:
        TileGtk_g_object_get(settings, Tcl_GetString(objv[1]), &s_val, NULL);
        if (s_val) {
          Tcl_SetResult(interp, (char *) s_val, TCL_VOLATILE);
          TileGtk_g_free (s_val);
        }
        break;
    }
  }
  Tcl_MutexUnlock(&tilegtkMutex);
  return TCL_OK;
}; /* Tilegtk_SettingsProperty */

#define GETPROPERTY_GTK_WIDGET_GET       0
#define GETPROPERTY_GTK_WIDGET_STYLE_GET 1
int Tilegtk_GetProperty(ClientData clientData, Tcl_Interp *interp,
                                 int objc, Tcl_Obj *const objv[], int gtkMethod) {
  static const char *Methods[] = {
    "integer", "boolean", "string", (char *) NULL
  };
  enum methods {
    INTEGER, BOOLEAN, STRING
  };
  static const char *Widgets[] = {
    "GtkHScrollbar" , "GtkVScrollbar", "GtkNotebook", "GtkButton",
    (char *) NULL
  };
  enum widgets {
    W_HSCROLLBAR, W_VSCROLLBAR, W_NOTEBOOK, W_BUTTON
  };
  int          type  = STRING;
  gchar       *s_val = NULL;
  gboolean     b_val = FALSE;
  gint         i_val = 0;
  GtkWidget   *widget = NULL;
  TileGtk_WidgetCache **wc_array = (TileGtk_WidgetCache **) clientData;
  TileGtk_WidgetCache *wc;
  if (!wc_array || !wc_array[0]) {
    Tcl_SetResult(interp, (char *) "empty wc_array!", TCL_STATIC);
    return TCL_ERROR;
  }
  wc = wc_array[0];
  if (objc != 3 && objc != 4) {
    Tcl_WrongNumArgs(interp, 1, objv,
       "widget property ?integer|boolean|string?");
    return TCL_ERROR;
  }
  /* Get widget... */
  if (Tcl_GetIndexFromObj(interp, objv[1], (const char **) Widgets,
                                 "widget", 0, &type) != TCL_OK) {
    return TCL_ERROR;
  }
  switch ((enum widgets) type) {
    case W_HSCROLLBAR: {widget = TileGtk_GetHScrollBar(wc); break;}
    case W_VSCROLLBAR: {widget = TileGtk_GetVScrollBar(wc); break;}
    case W_NOTEBOOK:   {widget = TileGtk_GetNotebook(wc);   break;}
    case W_BUTTON:     {widget = TileGtk_GetButton(wc);     break;}
  }
  /* Get property type, which defaults to "string"... */
  if (objc == 4) {
    if (Tcl_GetIndexFromObj(interp, objv[3], (const char **) Methods,
                            "method", 0, &type) != TCL_OK) {
      return TCL_ERROR;
    }
  }

  Tcl_MutexLock(&tilegtkMutex);
  if (widget) {
    switch ((enum methods) type) {
      case INTEGER:
        switch (gtkMethod) {
          case GETPROPERTY_GTK_WIDGET_GET:
            TileGtk_gtk_object_get((GtkObject *) widget,
                Tcl_GetString(objv[2]), &i_val, NULL);
          break;
          case GETPROPERTY_GTK_WIDGET_STYLE_GET:
            TileGtk_gtk_widget_style_get(widget, Tcl_GetString(objv[2]),
                                         &i_val, NULL);
            break;
        }
        Tcl_SetObjResult(interp, Tcl_NewIntObj(i_val));
        break;
      case BOOLEAN:
        switch (gtkMethod) {
          case GETPROPERTY_GTK_WIDGET_GET:
            TileGtk_gtk_object_get((GtkObject *) widget,
                Tcl_GetString(objv[2]), &b_val, NULL);
            break;
          case GETPROPERTY_GTK_WIDGET_STYLE_GET:
            TileGtk_gtk_widget_style_get(widget, Tcl_GetString(objv[2]),
                                         &b_val, NULL);
            break;
        }
        if (b_val) Tcl_SetObjResult(interp, Tcl_NewBooleanObj(1));
        else Tcl_SetObjResult(interp, Tcl_NewBooleanObj(0));
        break;
      case STRING:
        switch (gtkMethod) {
          case GETPROPERTY_GTK_WIDGET_GET:
            TileGtk_gtk_object_get((GtkObject *) widget,
                Tcl_GetString(objv[2]), &s_val, NULL);
            break;
          case GETPROPERTY_GTK_WIDGET_STYLE_GET:
            TileGtk_gtk_widget_style_get(widget, Tcl_GetString(objv[2]),
                                         &s_val, NULL);
            break;
        }
        if (s_val) {
          Tcl_SetResult(interp, (char *) s_val, TCL_VOLATILE);
          TileGtk_g_free (s_val);
        }
        break;
    }
  }
  Tcl_MutexUnlock(&tilegtkMutex);
  return TCL_OK;
}; /* Tilegtk_GetProperty */

int Tilegtk_WidgetProperty(ClientData clientData, Tcl_Interp *interp,
                          int objc, Tcl_Obj *const objv[]) {
  return Tilegtk_GetProperty(clientData, interp, objc, objv,
                            GETPROPERTY_GTK_WIDGET_GET);
}; /* Tilegtk_WidgetProperty */

int Tilegtk_WidgetStyleProperty(ClientData clientData, Tcl_Interp *interp,
                                 int objc, Tcl_Obj *const objv[]) {
  return Tilegtk_GetProperty(clientData, interp, objc, objv,
                            GETPROPERTY_GTK_WIDGET_STYLE_GET);
}; /* Tilegtk_WidgetStyleProperty */

int Tilegtk_GtkEnum(ClientData clientData, Tcl_Interp *interp,
                                 int objc, Tcl_Obj *const objv[]) {
  static const char *Methods[] = {
    "GtkPositionType", (char *) NULL
  };
  enum methods {
    GTKPOSITIONTYPE,
  };
  int index, v;
  const char *n = NULL;
  if (objc != 3) {
    Tcl_WrongNumArgs(interp, 1, objv, "gtk_enum_type value");
    return TCL_ERROR;
  }
  /* Get widget... */
  if (Tcl_GetIndexFromObj(interp, objv[1], (const char **) Methods,
                                 "gtk_enum_type", 0, &index) != TCL_OK) {
    return TCL_ERROR;
  }
  if (Tcl_GetIntFromObj(interp, objv[2], &v) != TCL_OK) {
    return TCL_ERROR;
  }
  switch ((enum methods) index) {
    case GTKPOSITIONTYPE: {
      if      (v == GTK_POS_LEFT)   n = "GTK_POS_LEFT";
      else if (v == GTK_POS_RIGHT)  n = "GTK_POS_RIGHT";
      else if (v == GTK_POS_TOP)    n = "GTK_POS_TOP";
      else if (v == GTK_POS_BOTTOM) n = "GTK_POS_BOTTOM";
      break;
    }
  }
  if (!n) {
    Tcl_SetResult(interp, (char *) "value out of range", TCL_STATIC);
    return TCL_ERROR;
  }
  Tcl_SetResult(interp, (char *) n, TCL_STATIC);
  return TCL_OK;
}; /* Tilegtk_GtkEnum */

int Tilegtk_GtkDirectory(ClientData clientData, Tcl_Interp *interp,
                                 int objc, Tcl_Obj *const objv[]) {
  static const char *Methods[] = {
    "theme", "default_files", (char *) NULL
  };
  enum methods {
    THEME, DEFAULT_FILES
  };
  int type;
  gchar *dir = NULL, **dirs = NULL;
  
  if (objc != 2 && objc != 3) {
    Tcl_WrongNumArgs(interp, 1, objv, "category ?value?");
    return TCL_ERROR;
  }
  if (Tcl_GetIndexFromObj(interp, objv[1], (const char **) Methods,
                          "method", 0, &type) != TCL_OK) {
    return TCL_ERROR;
  }

  Tcl_MutexLock(&tilegtkMutex);
  switch ((enum methods) type) {
    case THEME:
      dir = TileGtk_gtk_rc_get_theme_dir();
      break;
    case DEFAULT_FILES:
      if (objc == 3) {
        int mobjc; Tcl_Obj **mobjv;
        if (Tcl_ListObjGetElements(interp, objv[2], &mobjc, &mobjv) != TCL_OK) {
          return TCL_ERROR;
        }
        dirs = TileGtk_g_new0(gchar *, mobjc+1);
        for (int i = 0; i < mobjc; ++i) {
          Tcl_IncrRefCount(mobjv[i]);
          dirs[i] = Tcl_GetString(mobjv[i]);
        }
        TileGtk_gtk_rc_set_default_files(dirs);
        for (int i = 0; i < mobjc; ++i) {
          Tcl_DecrRefCount(mobjv[i]);
        }
        TileGtk_g_free(dirs); dirs = NULL;
      } else {
        dirs = TileGtk_gtk_rc_get_default_files();
      }
      break;
  }
  if (dir) {
    Tcl_SetResult(interp, (char *) dir, TCL_VOLATILE);
    TileGtk_g_free(dir);
  }
  if (dirs) {
    Tcl_Obj *list = Tcl_NewListObj(0, NULL);
    while (*dirs) {
      Tcl_ListObjAppendElement(NULL, list, Tcl_NewStringObj(*dirs, -1));
      ++dirs;
    }
    Tcl_SetObjResult(interp, list);
  }
  Tcl_MutexUnlock(&tilegtkMutex);
  return TCL_OK;
}; /* Tilegtk_GtkDirectory */

int Tilegtk_gtk_method(ClientData clientData, Tcl_Interp *interp,
                                 int objc, Tcl_Obj *const objv[]) {
  static const char *Methods[] = {
    "gtk_rc_reparse_all_for_settings", "gtk_rc_reset_styles", (char *) NULL
  };
  enum methods {
    GTK_RC_REPARSE_ALL_FOR_SETTINGS, GTK_RC_RESET_STYLES
  };
  int type;
  
  if (objc != 2) {
    Tcl_WrongNumArgs(interp, 1, objv, "method"); return TCL_ERROR;
  }
  if (Tcl_GetIndexFromObj(interp, objv[1], (const char **) Methods,
                          "method", 0, &type) != TCL_OK) {
    return TCL_ERROR;
  }

  Tcl_MutexLock(&tilegtkMutex);
  switch ((enum methods) type) {
    case GTK_RC_REPARSE_ALL_FOR_SETTINGS:
      TileGtk_gtk_rc_reparse_all_for_settings(
              TileGtk_gtk_settings_get_default(), TRUE);
      break;
    case GTK_RC_RESET_STYLES:
      TileGtk_gtk_rc_reset_styles(TileGtk_gtk_settings_get_default());
      break;
  }
  Tcl_MutexUnlock(&tilegtkMutex);
  return TCL_OK;
}; /* Tilegtk_gtk_method */

int Tilegtk_ThemeColour(ClientData clientData, Tcl_Interp *interp,
                                 int objc, Tcl_Obj *const objv[]) {
  static const char *Methods[] = {
    "fg(NORMAL)",           "fg(PRELIGHT)",           "fg(ACTIVE)",
    "fg(SELECTED)",         "fg(INSENSITIVE)",
    "bg(NORMAL)",           "bg(PRELIGHT)",           "bg(ACTIVE)",
    "bg(SELECTED)",         "bg(INSENSITIVE)",
    "base(NORMAL)",         "base(PRELIGHT)",         "base(ACTIVE)",
    "base(SELECTED)",       "base(INSENSITIVE)",
    "text(NORMAL)",         "text(PRELIGHT)",         "text(ACTIVE)",
    "text(SELECTED)",       "text(INSENSITIVE)",
    "light(NORMAL)",        "light(PRELIGHT)",        "light(ACTIVE)",
    "light(SELECTED)",      "light(INSENSITIVE)",
    "mid(NORMAL)",          "mid(PRELIGHT)",          "mid(ACTIVE)",
    "mid(SELECTED)",        "mid(INSENSITIVE)",
    "dark(NORMAL)",         "dark(PRELIGHT)",         "dark(ACTIVE)",
    "dark(SELECTED)",       "dark(INSENSITIVE)",
    "text_aa(NORMAL)",      "text_aa(PRELIGHT)",      "text_aa(ACTIVE)",
    "text_aa(SELECTED)",    "text_aa(INSENSITIVE)",
    (char *) NULL
  };
  enum methods {
    FG_NORMAL,        FG_PRELIGHT,        FG_ACTIVE,
    FG_SELECTED,      FG_INSENSITIVE,
    BG_NORMAL ,       BG_PRELIGHT,        BG_ACTIVE,
    BG_SELECTED,      BG_INSENSITIVE,
    BASE_NORMAL,      BASE_PRELIGHT,      BASE_ACTIVE,
    BASE_SELECTED,    BASE_INSENSITIVE,
    TEXT_NORMAL,      TEXT_PRELIGHT,      TEXT_ACTIVE,
    TEXT_SELECTED,    TEXT_INSENSITIVE,
    LIGHT_NORMAL,     LIGHT_PRELIGHT,     LIGHT_ACTIVE,
    LIGHT_SELECTED,   LIGHT_INSENSITIVE,
    MID_NORMAL,       MID_PRELIGHT,       MID_ACTIVE,
    MID_SELECTED,     MID_INSENSITIVE,
    DARK_NORMAL,      DARK_PRELIGHT,      DARK_ACTIVE,
    DARK_SELECTED,    DARK_INSENSITIVE,
    TAA_NORMAL,       TAA_PRELIGHT,       TAA_ACTIVE,
    TAA_SELECTED,     TAA_INSENSITIVE
  };
  TileGtk_WidgetCache **wc = (TileGtk_WidgetCache **) clientData;
  GdkColor colour;
  gchar* colour_str = NULL;
  GtkStyle *style;
  int index;
  if (!wc) {
    Tcl_SetResult(interp, (char *) "empty wc!", TCL_STATIC);
    return TCL_ERROR;
  }
  style = TileGtk_GetGtkWindowStyle(wc[0]->gtkWindow);
  if (!style) {
    Tcl_SetResult(interp, (char *) "empty style!", TCL_STATIC);
    return TCL_ERROR;
  }
  if (objc != 2) {
    Tcl_WrongNumArgs(interp, 1, objv, "colour");
    return TCL_ERROR;
  }

  if (Tcl_GetIndexFromObj(interp, objv[1], (const char **) Methods,
                          "method", 0, &index) == TCL_OK) {
  switch ((enum methods) index) {
    case FG_NORMAL:         colour = style->fg[GTK_STATE_NORMAL];         break;
    case FG_PRELIGHT:       colour = style->fg[GTK_STATE_PRELIGHT];       break;
    case FG_ACTIVE:         colour = style->fg[GTK_STATE_ACTIVE];         break;
    case FG_SELECTED:       colour = style->fg[GTK_STATE_SELECTED];       break;
    case FG_INSENSITIVE:    colour = style->fg[GTK_STATE_INSENSITIVE];    break;
    case BG_NORMAL:         colour = style->bg[GTK_STATE_NORMAL];         break;
    case BG_PRELIGHT:       colour = style->bg[GTK_STATE_PRELIGHT];       break;
    case BG_ACTIVE:         colour = style->bg[GTK_STATE_ACTIVE];         break;
    case BG_SELECTED:       colour = style->bg[GTK_STATE_SELECTED];       break;
    case BG_INSENSITIVE:    colour = style->bg[GTK_STATE_INSENSITIVE];    break;
    case BASE_NORMAL:       colour = style->base[GTK_STATE_NORMAL];       break;
    case BASE_PRELIGHT:     colour = style->base[GTK_STATE_PRELIGHT];     break;
    case BASE_ACTIVE:       colour = style->base[GTK_STATE_ACTIVE];       break;
    case BASE_SELECTED:     colour = style->base[GTK_STATE_SELECTED];     break;
    case BASE_INSENSITIVE:  colour = style->base[GTK_STATE_INSENSITIVE];  break;
    case TEXT_NORMAL:       colour = style->text[GTK_STATE_NORMAL];       break;
    case TEXT_PRELIGHT:     colour = style->text[GTK_STATE_PRELIGHT];     break;
    case TEXT_ACTIVE:       colour = style->text[GTK_STATE_ACTIVE];       break;
    case TEXT_SELECTED:     colour = style->text[GTK_STATE_SELECTED];     break;
    case TEXT_INSENSITIVE:  colour = style->text[GTK_STATE_INSENSITIVE];  break;
    case LIGHT_NORMAL:      colour = style->light[GTK_STATE_NORMAL];      break;
    case LIGHT_PRELIGHT:    colour = style->light[GTK_STATE_PRELIGHT];    break;
    case LIGHT_ACTIVE:      colour = style->light[GTK_STATE_ACTIVE];      break;
    case LIGHT_SELECTED:    colour = style->light[GTK_STATE_SELECTED];    break;
    case LIGHT_INSENSITIVE: colour = style->light[GTK_STATE_INSENSITIVE]; break;
    case DARK_NORMAL:       colour = style->dark[GTK_STATE_NORMAL];       break;
    case DARK_PRELIGHT:     colour = style->dark[GTK_STATE_PRELIGHT];     break;
    case DARK_ACTIVE:       colour = style->dark[GTK_STATE_ACTIVE];       break;
    case DARK_SELECTED:     colour = style->dark[GTK_STATE_SELECTED];     break;
    case DARK_INSENSITIVE:  colour = style->dark[GTK_STATE_INSENSITIVE];  break;
    case MID_NORMAL:        colour = style->mid[GTK_STATE_NORMAL];        break;
    case MID_PRELIGHT:      colour = style->mid[GTK_STATE_PRELIGHT];      break;
    case MID_ACTIVE:        colour = style->mid[GTK_STATE_ACTIVE];        break;
    case MID_SELECTED:      colour = style->mid[GTK_STATE_SELECTED];      break;
    case MID_INSENSITIVE:   colour = style->mid[GTK_STATE_INSENSITIVE];   break;
    case TAA_NORMAL:        colour = style->text_aa[GTK_STATE_NORMAL];    break;
    case TAA_PRELIGHT:      colour = style->text_aa[GTK_STATE_PRELIGHT];  break;
    case TAA_ACTIVE:        colour = style->text_aa[GTK_STATE_ACTIVE];    break;
    case TAA_SELECTED:      colour = style->text_aa[GTK_STATE_SELECTED];  break;
    case TAA_INSENSITIVE:   colour = style->text_aa[GTK_STATE_INSENSITIVE];
  }
    colour_str = TileGtk_gdk_color_to_string(&colour);
  } else {
    if (TileGtk_gtk_style_lookup_color(style, Tcl_GetString(objv[1]), &colour)){
      colour_str = TileGtk_gdk_color_to_string(&colour);
    }
  }

  if (colour_str) {
    Tcl_SetResult(interp, (char *) colour_str, TCL_VOLATILE);
    TileGtk_g_free(colour_str);
    return TCL_OK;
  }
  Tcl_SetResult(interp, (char *) "colour not found: ", TCL_STATIC);
  Tcl_AppendResult(interp, (char *) Tcl_GetString(objv[1]), NULL);
  return TCL_ERROR;
}; /* Tilegtk_ThemeColour */

#ifndef TILEGTK_LOAD_GTK_DYNAMICALLY
#ifndef GTK_STYLE_GET_PRIVATE
struct _GtkStylePrivate {
  GSList *color_hashes;
};
typedef struct _GtkStylePrivate GtkStylePrivate;
#define GTK_STYLE_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE ((obj), GTK_TYPE_STYLE, GtkStylePrivate))
#endif
#endif /* TILEGTK_LOAD_GTK_DYNAMICALLY */

int Tilegtk_ColourKeys(ClientData clientData, Tcl_Interp *interp,
                                 int objc, Tcl_Obj *const objv[]) {
  TileGtk_WidgetCache **wc = (TileGtk_WidgetCache **) clientData;
  GtkStyle *style;
  if (!wc) {
    Tcl_SetResult(interp, (char *) "empty wc!", TCL_STATIC);
    return TCL_ERROR;
  }
  style = TileGtk_GetGtkWindowStyle(wc[0]->gtkWindow);
  if (!style) {
    Tcl_SetResult(interp, (char *) "empty style!", TCL_STATIC);
    return TCL_ERROR;
  }
#ifndef TILEGTK_LOAD_GTK_DYNAMICALLY
  GtkStylePrivate *priv = GTK_STYLE_GET_PRIVATE(style);
  GSList *iter;
  Tcl_Obj *list = Tcl_NewListObj(0, NULL);
  for (iter = priv->color_hashes; iter != NULL; iter = iter->next) {
    GHashTable *hash    = (GHashTable *) iter->data;
    GList *keys = TileGtk_g_hash_table_get_keys(hash);
    for (; keys != NULL; keys = keys->next) {
      Tcl_ListObjAppendElement(NULL, list,
                               Tcl_NewStringObj((char *) keys->data, -1));
    }
  }
  Tcl_SetObjResult(interp, list);
#endif /* TILEGTK_LOAD_GTK_DYNAMICALLY */
  return TCL_OK;
}; /* Tilegtk_ColourKeys */

int Tilegtk_InitialiseLibrary(ClientData clientData, Tcl_Interp *interp,
                                 int objc, Tcl_Obj *const objv[]) {
  static const char *Methods[] = {
    "required",
    "gdk",  "gdk_pixbuf", "gdk_pixbuf_xlib",
    "glib", "gobject",
    "gtk",
    (char *) NULL
  };
  enum methods {
    L_REQUIRED,
    L_GDK,  L_GDK_PIXBUF, L_GDK_PIXBUF_XLIB,
    L_GLIB, L_GOBJECT,
    L_GTK
  };
  int index, status;
  if (objc != 2 && objc != 3) {
    Tcl_WrongNumArgs(interp, 1, objv, "required|library ?filename?");
    return TCL_ERROR;
  }
  if (Tcl_GetIndexFromObj(interp, objv[1], (const char **) Methods,
                          "method", 0, &index) != TCL_OK)
    return TCL_ERROR;
  Tcl_MutexLock(&tilegtkMutex);
  switch ((enum methods) index) {
    case L_REQUIRED:
#ifdef TILEGTK_LOAD_GTK_DYNAMICALLY
      Tcl_SetObjResult(interp, Tcl_NewBooleanObj(1));
#else
      Tcl_SetObjResult(interp, Tcl_NewBooleanObj(0));
#endif /* TILEGTK_LOAD_GTK_DYNAMICALLY */
      Tcl_MutexUnlock(&tilegtkMutex);
      return TCL_OK;
#ifdef TILEGTK_LOAD_GTK_DYNAMICALLY
    case L_GDK:
      if (!TILEGTK_LAST_SYMBOL_gdk && objc > 2) {
        status = TileGtk_InitialiseSymbols_gdk(interp, objv[2]);
      }
      break;
    case L_GDK_PIXBUF:
      if (!TILEGTK_LAST_SYMBOL_gdk_pixbuf && objc > 2) {
        status = TileGtk_InitialiseSymbols_gdk_pixbuf(interp, objv[2]);
      }
      break;
    case L_GDK_PIXBUF_XLIB:
#ifndef __WIN32__
      if (!TILEGTK_LAST_SYMBOL_gdk_pixbuf_xlib && objc > 2) {
        status = TileGtk_InitialiseSymbols_gdk_pixbuf_xlib(interp, objv[2]);
      }
#endif
      break;
    case L_GLIB:
      if (!TILEGTK_LAST_SYMBOL_glib && objc > 2) {
        status = TileGtk_InitialiseSymbols_glib(interp, objv[2]);
      }
      break;
    case L_GOBJECT:
      if (!TILEGTK_LAST_SYMBOL_gobject && objc > 2) {
        status = TileGtk_InitialiseSymbols_gobject(interp, objv[2]);
      }
      break;
    case L_GTK:
      if (!TILEGTK_LAST_SYMBOL_gtk && objc > 2) {
        status = TileGtk_InitialiseSymbols_gtk(interp, objv[2]);
      }
      break;
#endif /* TILEGTK_LOAD_GTK_DYNAMICALLY */
  }
  Tcl_MutexUnlock(&tilegtkMutex);
  return status;
}; /* Tilegtk_InitialiseLibrary */

int Tilegtk_SetPalette(ClientData clientData, Tcl_Interp *interp,
                                 int objc, Tcl_Obj *const objv[]) {
#if 0
  static const char *Methods[] = {
    "-background",       "-foreground",
    "-buttonBackground", "-buttonForeground",
    "-selectBackground", "-selectForeground",
    "-windowBackground", "-windowForeground",
    "-linkColor",        "-visitedLinkColor",
    "-contrast",
    (char *) NULL
  };
  enum methods {
    CLR_background,       CLR_foreground,
    CLR_buttonBackground, CLR_buttonForeground,
    CLR_selectBackground, CLR_selectForeground,
    CLR_windowBackground, CLR_windowForeground,
    CLR_linkColor,        CLR_visitedLinkColor,
    CLR_contrast
  };
  int index, contrast_;
  char *value;
  if ((objc-1)%2) {
    Tcl_WrongNumArgs(interp, 1, objv, "?-key value?");
    return TCL_ERROR;
  }
  Tcl_MutexLock(&tilegtkMutex);
  Tcl_MutexUnlock(&tilegtkMutex);
#endif
  return TCL_OK;
}; /* Tilegtk_SetPalette */

int Tilegtk_SetStyle(ClientData clientData, Tcl_Interp *interp,
                                 int objc, Tcl_Obj *const objv[]) {
  if (objc != 2) {Tcl_WrongNumArgs(interp, 1, objv, "style"); return TCL_ERROR;}
  Tcl_MutexLock(&tilegtkMutex);
#if 0
  TileGtk_WidgetCache **wc_array = (TileGtk_WidgetCache **) clientData;
  TileGtk_WidgetCache *wc = wc_array[0];
  if (qApp) {
    int len;
    const char* str = Tcl_GetStringFromObj(objv[1], &len);
    QString style = QString::fromUtf8(str, len);
    QStyle *new_style = QStyleFactory::create(style);
    QStyle *todelete = NULL;
    if (new_style == NULL) {
      Tcl_SetResult(interp, (char *) "unknwon style: \"", TCL_STATIC);
      Tcl_AppendResult(interp, str, "\"", NULL);
      Tcl_MutexUnlock(&tilegtkMutex);
      return TCL_ERROR;
    }
    //qApp->setStyle(style);
    /* Is this style the qApp style? */
    if (wc->TileGtk_Style_Owner) todelete = wc->TileGtk_Style;

#ifdef TILEGTK_GTK_VERSION_3                    
    if (strcmp(qApp->style().name(), str) == 0) {
      wc->TileGtk_Style = &(qApp->style());
#endif /* TILEGTK_GTK_VERSION_3 */
#ifdef TILEGTK_GTK_VERSION_4                    
    if (qApp->style()->objectName() == style) {
      wc->TileGtk_Style = qApp->style();
#endif /* TILEGTK_GTK_VERSION_4 */
      wc->TileGtk_Style_Owner = false;
    } else {
      wc->TileGtk_Style = QStyleFactory::create(style);
      wc->TileGtk_Style_Owner = true;
    }
    TileGtk_StoreStyleNameLowers(wc);
    wc->TileGtk_QScrollBar_Widget->setStyle(wc->TileGtk_Style);
    wc->TileGtk_QComboBox_RW_Widget->setStyle(wc->TileGtk_Style);
    wc->TileGtk_QComboBox_RO_Widget->setStyle(wc->TileGtk_Style);
    wc->TileGtk_QWidget_WidgetParent->setStyle(wc->TileGtk_Style);
    wc->TileGtk_QWidget_Widget->setStyle(wc->TileGtk_Style);
#ifdef TILEGTK_GTK_VERSION_3
    wc->TileGtk_QWidget_Widget->polish();
#endif /* TILEGTK_GTK_VERSION_3 */
    wc->TileGtk_QSlider_Hor_Widget->setStyle(wc->TileGtk_Style);
    wc->TileGtk_QSlider_Ver_Widget->setStyle(wc->TileGtk_Style);
    wc->TileGtk_QProgressBar_Hor_Widget->setStyle(wc->TileGtk_Style);
    wc->TileGtk_GTKabWidget_Widget->setStyle(wc->TileGtk_Style);
    wc->TileGtk_QPixmap_BackgroundTile = 
#ifdef TILEGTK_GTK_VERSION_3
                     (wc->TileGtk_QWidget_Widget)->paletteBackgroundPixmap();
#endif /* TILEGTK_GTK_VERSION_3 */
#ifdef TILEGTK_GTK_VERSION_4
                     (wc->TileGtk_QWidget_Widget)->palette().window().texture();
#endif /* TILEGTK_GTK_VERSION_4 */
    wc->TileGtk_Style->polish(wc->TileGtk_QWidget_Widget);
    if (todelete) delete todelete;
#if 0
    // Print Scrollbar statistics...
#define SC_PRINT_INFO(subcontrol) \
    wc->TileGtk_Style->querySubControlMetrics(QStyle::CC_ScrollBar,\
        wc->TileGtk_QScrollBar_Widget, subcontrol).x(),\
    wc->TileGtk_Style->querySubControlMetrics(QStyle::CC_ScrollBar,\
        wc->TileGtk_QScrollBar_Widget, subcontrol).y(),\
    wc->TileGtk_Style->querySubControlMetrics(QStyle::CC_ScrollBar,\
        wc->TileGtk_QScrollBar_Widget, subcontrol).width(),\
    wc->TileGtk_Style->querySubControlMetrics(QStyle::CC_ScrollBar,\
        wc->TileGtk_QScrollBar_Widget, subcontrol).height()
    printf("SC_ScrollBarAddLine: x=%d, y=%d, w=%d, h=%d\n", SC_PRINT_INFO(QStyle::SC_ScrollBarAddLine));
    printf("SC_ScrollBarSubLine: x=%d, y=%d, w=%d, h=%d\n", SC_PRINT_INFO(QStyle::SC_ScrollBarSubLine));
    printf("SC_ScrollBarAddPage: x=%d, y=%d, w=%d, h=%d\n", SC_PRINT_INFO(QStyle::SC_ScrollBarAddPage));
    printf("SC_ScrollBarSubPage: x=%d, y=%d, w=%d, h=%d\n", SC_PRINT_INFO(QStyle::SC_ScrollBarSubPage));
    printf("SC_ScrollBarFirst: x=%d, y=%d, w=%d, h=%d\n", SC_PRINT_INFO(QStyle::SC_ScrollBarFirst));
    printf("SC_ScrollBarLast: x=%d, y=%d, w=%d, h=%d\n", SC_PRINT_INFO(QStyle::SC_ScrollBarLast));
#endif
  }
  memcpy(wc_array[1], wc_array[0], sizeof(TileGtk_WidgetCache));
  wc_array[0]->orientation = TTK_ORIENT_HORIZONTAL;
  wc_array[1]->orientation = TTK_ORIENT_VERTICAL;
  /* Save the name of the current theme... */
  Tcl_SetVar(interp, "ttk::theme::tilegtk::theme",
#ifdef TILEGTK_GTK_VERSION_3                    
             wc->TileGtk_Style->name(), TCL_GLOBAL_ONLY);
#endif /* TILEGTK_GTK_VERSION_3 */
#ifdef TILEGTK_GTK_VERSION_4                    
             wc->TileGtk_Style->objectName().toUtf8().data(), TCL_GLOBAL_ONLY);
#endif /* TILEGTK_GTK_VERSION_4 */
#endif
  Tcl_MutexUnlock(&tilegtkMutex);
  return TCL_OK;
}; /* Tilegtk_SetStyle */

extern "C" int DLLEXPORT
Tilegtk_Init(Tcl_Interp *interp)
{
    Ttk_Theme themePtr;
    Tk_Window tkwin;
    char tmpScript[1024];
    TileGtk_WidgetCache **wc = NULL;
    GtkSettings *settings = NULL;
    gchar       *strval = NULL;

    if (Tcl_InitStubs(interp, TCL_VERSION, 0) == NULL)
        return TCL_ERROR;
    if (Tk_InitStubs(interp,  TK_VERSION,  0) == NULL)
        return TCL_ERROR;
    if (Ttk_InitStubs(interp) == NULL)
        return TCL_ERROR;

    tkwin = Tk_MainWindow(interp);
    if (tkwin == NULL) return TCL_ERROR;

    themePtr  = Ttk_CreateTheme(interp, "tilegtk", NULL);
    if (!themePtr) return TCL_ERROR;

    Tcl_CreateObjCommand(interp,
                         "ttk::theme::tilegtk::initialiseLibrary",
                         Tilegtk_InitialiseLibrary, (ClientData) wc, NULL);
    if (Tcl_Eval(interp, initScript) != TCL_OK) {
      return TCL_ERROR;
    }
#ifdef TILEGTK_LOAD_GTK_DYNAMICALLY
    if (!TileGtk_GtkAppCreated) {
      if (Tcl_Eval(interp, libsInitScript) != TCL_OK) {
        return TCL_ERROR;
      }
    }
#endif /* TILEGTK_LOAD_GTK_DYNAMICALLY */

    /*
     * Initialise Gtk:
     */
    Tcl_MutexLock(&tilegtkMutex);
    wc = TileGtk_CreateGtkApp(interp);
    ++TileGtk_GtkAppCreated;

    /*
     * Register the various widgets...
     */
    TileGtk_Init_Background(interp, wc, themePtr);
    TileGtk_Init_Button(interp, wc, themePtr);
    TileGtk_Init_CheckButton(interp, wc, themePtr);
    TileGtk_Init_RadioButton(interp, wc, themePtr);
    TileGtk_Init_ToolButton(interp, wc, themePtr);
    TileGtk_Init_Labelframe(interp, wc, themePtr);
    TileGtk_Init_Entry(interp, wc, themePtr);
    TileGtk_Init_Menubutton(interp, wc, themePtr);
    TileGtk_Init_Scrollbar(interp, wc, themePtr);
    TileGtk_Init_Scale(interp, wc, themePtr);
    TileGtk_Init_Progress(interp, wc, themePtr);
    TileGtk_Init_SizeGrip(interp, wc, themePtr);
    TileGtk_Init_Paned(interp, wc, themePtr);
    TileGtk_Init_Notebook(interp, wc, themePtr);
    TileGtk_Init_Combobox(interp, wc, themePtr);
#if 0
    TileGtk_Init_TreeView(interp, wc, themePtr);
    //TileGtk_Init_Separator(interp, wc, themePtr);
    //TileGtk_Init_Arrows(interp, wc, themePtr);
#endif
    Tcl_CreateExitHandler(&TileGtk_ExitProc, 0);
    //Tcl_CreateThreadExitHandler(&TileGtk_ExitProc, 0);
    
    /*
     * Register the TileGtk package...
     */
    Tcl_CreateObjCommand(interp, "ttk::theme::tilegtk::gtkEnum",
                         Tilegtk_GtkEnum, (ClientData) wc, NULL);
    Tcl_CreateObjCommand(interp, "ttk::theme::tilegtk::settingsProperty",
                         Tilegtk_SettingsProperty, (ClientData) wc, NULL);
    Tcl_CreateObjCommand(interp, "ttk::theme::tilegtk::widgetStyleProperty",
                         Tilegtk_WidgetStyleProperty, (ClientData) wc, NULL);
    Tcl_CreateObjCommand(interp, "ttk::theme::tilegtk::widgetProperty",
                         Tilegtk_WidgetProperty, (ClientData) wc, NULL);
    Tcl_CreateObjCommand(interp, "ttk::theme::tilegtk::currentThemeName",
                         Tilegtk_ThemeName, (ClientData) wc, NULL);
    Tcl_CreateObjCommand(interp, "ttk::theme::tilegtk::gtkDirectory",
                         Tilegtk_GtkDirectory, (ClientData) wc, NULL);
    Tcl_CreateObjCommand(interp, "ttk::theme::tilegtk::setStyle",
                         Tilegtk_SetStyle, (ClientData) wc, NULL);
    Tcl_CreateObjCommand(interp, "ttk::theme::tilegtk::gtk_method",
                         Tilegtk_gtk_method, (ClientData) wc, NULL);
    Tcl_CreateObjCommand(interp,
                         "ttk::theme::tilegtk::currentThemeColour",
                         Tilegtk_ThemeColour, (ClientData) wc, NULL);
    Tcl_CreateObjCommand(interp,
                         "ttk::theme::tilegtk::currentThemeColourKeys",
                         Tilegtk_ColourKeys, (ClientData) wc, NULL);
#if 0
    Tcl_CreateObjCommand(interp,
                         "ttk::theme::tilegtk::setPalette",
                         Tilegtk_SetPalette, (ClientData) wc, NULL);
    Tcl_CreateObjCommand(interp,
                         "ttk::theme::tilegtk::getPixelMetric",
                         Tilegtk_GetPixelMetric, (ClientData) wc, NULL);
    Tcl_CreateObjCommand(interp,
                         "ttk::theme::tilegtk::getStyleHint",
                         Tilegtk_GetStyleHint, (ClientData) wc, NULL);
    Tcl_CreateObjCommand(interp,
                         "ttk::theme::tilegtk::getSubControlMetrics",
                         Tilegtk_GetSubControlMetrics, (ClientData) wc, NULL);
#endif
    /* Save the name of the current theme... */
    strcpy(tmpScript, "namespace eval ttk::theme::tilegtk { variable theme ");
    settings = TileGtk_gtk_settings_get_default();
    if (settings) {
      TileGtk_g_object_get(settings, "gtk-theme-name", &strval, NULL);
      strcat(tmpScript, "{");
      if (strval) {
        strcat(tmpScript, strval);
        TileGtk_g_free(strval);
      }
      strcat(tmpScript, "}");
    } else {
      strcat(tmpScript, "{}");
    }
    strcat(tmpScript, " };");
    Tcl_MutexUnlock(&tilegtkMutex);
    
    if (Tcl_Eval(interp, tmpScript) != TCL_OK) {
      return TCL_ERROR;
    }
    if (Tcl_Eval(interp, "ttk::theme::tilegtk::init") != TCL_OK) {
      return TCL_ERROR;
    }
    Tcl_PkgProvide(interp, "ttk::theme::tilegtk", PACKAGE_VERSION);
    Tcl_PkgProvide(interp, PACKAGE_NAME, PACKAGE_VERSION);
    return TCL_OK;
}; /* TileGtk_Init */

int DLLEXPORT
TileGtk_Finish(Tcl_Interp *interp)
{
    Tcl_MutexLock(&tilegtkMutex);
    if (TileGtk_GtkAppCreated < 0) {Tcl_MutexUnlock(&tilegtkMutex); return 0;}
    --TileGtk_GtkAppCreated;
    if (TileGtk_GtkAppCreated == 0) TileGtk_DestroyGtkApp();
    Tcl_MutexUnlock(&tilegtkMutex);
    return 0;
}; /* TileGtk_Finish */
