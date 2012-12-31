/*
 *  tileGtk_GtkApp.c
 * ---------------
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

#ifdef TILEGTK_ENABLE_GNOME
#include <gnome.h>
GnomeProgram *my_app = NULL;
#endif /* TILEGTK_ENABLE_GNOME */

#include "tileGtk_Utilities.h"
#include "tileGtk_TkHeaders.h"
#include <string.h>
gboolean   TileGtk_GtkInitialisedFlag = FALSE;
static int TileGtk_xlib_rgb_initialised = 0;
GtkWidget *TileGtk_GtkWindow = NULL;



/* In the following variable we store the XErrorHandler, before we install our
 * own, which filters out some XErrors... */
static int (*TileGtk_TkXErrorHandler)(Display *displayPtr,
                                     XErrorEvent *errorPtr);
static int (*TileGtk_GtkXErrorHandler)(Display *displayPtr,
                                     XErrorEvent *errorPtr);
static int TileGtk_XErrorHandler(Display *displayPtr, XErrorEvent *errorPtr);

static int  TileGtk_XEventHandler(ClientData clientdata, XEvent *eventPtr);

/*
 * TileGtk_InterpDeleteProc:
 * This function will be called when the interpreter gets deleted. It must free
 * all allocated interp-specific memory segments.
 */
static void TileGtk_InterpDeleteProc(ClientData clientData, Tcl_Interp *interp) {
  TileGtk_WidgetCache **wc_array = (TileGtk_WidgetCache **) clientData;
  TileGtk_WidgetCache *wc = wc_array[0];
  if (wc && wc->gtkWindow) {
    /*This will destroy also ALL children!*/
    TileGtk_gtk_widget_destroy(wc->gtkWindow);
  }
  // printf("Tk_DeleteGenericHandler: %p\n", interp); fflush(NULL);
  Tk_DeleteGenericHandler(&TileGtk_XEventHandler, (ClientData) interp);
  Tcl_Free((char *) wc_array[0]);
  Tcl_Free((char *) wc_array[1]);
  Tcl_Free((char *) wc_array);
}; /* TileGtk_InterpDeleteProc */

TileGtk_WidgetCache **TileGtk_CreateGtkApp(Tcl_Interp *interp) {
  /*
   * The first step is to initialise the gtk library. This must be done once
   * in the application lifetime.
   */
  Tcl_MutexLock(&tilegtkMutex);
  if (!TileGtk_GtkInitialisedFlag) {
#ifdef TILEGTK_ENABLE_GNOME
    gchar **remaining_args = NULL;
    GOptionEntry option_entries[] = {
      {G_OPTION_REMAINING, 0, 0, G_OPTION_ARG_FILENAME_ARRAY, &remaining_args,
       "Special option that collects any remaining arguments for us"},
      { NULL }
    };
    GOptionContext *option_context;
#endif /* TILEGTK_ENABLE_GNOME */
    int argc = 1;
    char **argv = TileGtk_g_new0(char*, 2);
    argv[0] = (char *) Tcl_GetNameOfExecutable();

#ifdef TILEGTK_INSTALL_XERROR_HANDLER
    TileGtk_TkXErrorHandler = XSetErrorHandler(TileGtk_XErrorHandler);
#endif /* TILEGTK_INSTALL_XERROR_HANDLER */

#ifdef TILEGTK_ENABLE_GNOME
    option_context = TileGtk_g_option_context_new("tile-gtk");
    TileGtk_g_option_context_add_main_entries(option_context,
                                              option_entries, NULL);
    /* We assume PACKAGE and VERSION are set to the program name and version
     * number respectively. Also, assume that 'option_entries' is a global
     * array of GOptionEntry structures.
     */
    my_app = gnome_program_init(PACKAGE_NAME, PACKAGE_VERSION,
                                LIBGNOMEUI_MODULE, argc, argv,
                                GNOME_PARAM_GOPTION_CONTEXT, option_context,
                                GNOME_PARAM_NONE);
    if (my_app) TileGtk_GtkInitialisedFlag = TRUE;
    if (remaining_args != NULL) {
      TileGtk_g_strfreev(remaining_args);
      remaining_args = NULL;
    }
#else  /* TILEGTK_ENABLE_GNOME */
    TileGtk_gtk_disable_setlocale();
    TileGtk_GtkInitialisedFlag = TileGtk_gtk_init_check(&argc, &argv);
#endif /* TILEGTK_ENABLE_GNOME */
    TileGtk_g_free(argv);
    if (!TileGtk_GtkInitialisedFlag) {
      Tcl_MutexUnlock(&tilegtkMutex);
      return NULL;
    }
    /* Initialise TileGtk_GtkWindow... */
    TileGtk_GtkWindow = TileGtk_gtk_window_new(GTK_WINDOW_POPUP);
    TileGtk_gtk_widget_realize(TileGtk_GtkWindow);
#ifdef TILEGTK_INSTALL_XERROR_HANDLER
    /*
     * GTK+ xerror handler will terminate the application.
     * Just get rid of that...
     */
    TileGtk_GtkXErrorHandler = XSetErrorHandler(TileGtk_XErrorHandler);
#endif /* TILEGTK_INSTALL_XERROR_HANDLER */

#ifdef TILEGTK_SYNCHRONIZE
    XSynchronize(Tk_Display(Tk_MainWindow(interp)), true);
#endif /* TILEGTK_SYNCHRONIZE */
  }
  Tcl_MutexUnlock(&tilegtkMutex);

  /*
   * Allocate the widget cache. We keep a widget cache per interpreter.
   * Each cache is an array of two elements, one for each orientation.
   */

  TileGtk_WidgetCache **wc_array = (TileGtk_WidgetCache **) 
                           Tcl_Alloc(sizeof(TileGtk_WidgetCache*)*2);
  wc_array[0] = (TileGtk_WidgetCache *) 
                           Tcl_Alloc(sizeof(TileGtk_WidgetCache));
  wc_array[1] = (TileGtk_WidgetCache *) 
                           Tcl_Alloc(sizeof(TileGtk_WidgetCache));
  Tcl_SetAssocData(interp, "tileTileGtk_gtk_widget_cache",
                   &TileGtk_InterpDeleteProc, (ClientData) wc_array);
  TileGtk_WidgetCache *wc = wc_array[0];
  memset(wc, 0, sizeof(TileGtk_WidgetCache));
  /*
   * Initialise the widget cache.
   */
  wc->TileGtk_MainInterp  = interp;
  wc->TileGtk_tkwin       = Tk_MainWindow(interp);
  if (wc->TileGtk_tkwin != NULL && wc->TileGtk_MainDisplay == None) {
    Tk_MakeWindowExist(wc->TileGtk_tkwin);
    wc->TileGtk_MainDisplay = Tk_Display(wc->TileGtk_tkwin);
  }
  if (wc->TileGtk_MainDisplay == None) {
    Tcl_MutexUnlock(&tilegtkMutex);
    Tcl_Free((char *) wc_array[0]);
    Tcl_Free((char *) wc_array[1]);
    Tcl_Free((char *) wc_array);
    return NULL;
  }
#ifndef __WIN32__
  wc->gdkDisplay = TileGtk_gdk_x11_lookup_xdisplay(wc->TileGtk_MainDisplay);
#endif
  if (!wc->gdkDisplay) {
    wc->gdkDisplay = TileGtk_gdk_display_get_default();
  }
  wc->gtkWindow = TileGtk_gtk_window_new(GTK_WINDOW_POPUP);
  if (wc->gtkWindow) TileGtk_gtk_widget_realize(wc->gtkWindow);
  wc->protoLayout = TileGtk_gtk_fixed_new();
  TileGtk_gtk_container_add((GtkContainer*)(wc->gtkWindow), wc->protoLayout);
  memcpy(wc_array[1], wc_array[0], sizeof(TileGtk_WidgetCache));
  wc_array[0]->orientation    = TTK_ORIENT_HORIZONTAL;
  wc_array[1]->orientation    = TTK_ORIENT_VERTICAL;
  wc_array[0]->gtkOrientation = GTK_ORIENTATION_HORIZONTAL;
  wc_array[1]->gtkOrientation = GTK_ORIENTATION_VERTICAL;

#ifndef __WIN32__
  Tcl_MutexLock(&tilegtkMutex);
  if (!TileGtk_xlib_rgb_initialised) {
    TileGtk_xlib_rgb_init(wc->TileGtk_MainDisplay,Tk_Screen(wc->TileGtk_tkwin));
    TileGtk_xlib_rgb_initialised = 1;
  }
  Tcl_MutexUnlock(&tilegtkMutex);
#endif
  return wc_array;
}; /* TileGtk_CreateGtkApp */

void TileGtk_DestroyGtkApp(void) {
  Tcl_MutexLock(&tilegtkMutex);
  if (TileGtk_GtkInitialisedFlag) {
    // XSetErrorHandler(TileGtk_TkXErrorHandler);
    TileGtk_GtkInitialisedFlag = FALSE;
  }
  Tcl_MutexUnlock(&tilegtkMutex);
}; /* TileGtk_DestroyGtkApp */

/*
 * TileGtk_XErrorHandler:
 * This XError handler just prints some debug information and then calls
 * Tk's XError handler...
 */
static int TileGtk_XErrorHandler(Display *displayPtr, XErrorEvent *errorPtr) {
#ifdef TILEGTK_VERBOSE_XERROR_HANDLER
  char buf[64];
  XGetErrorText (displayPtr, errorPtr->error_code, buf, 63);
  printf("===============================================================\n");
  printf("  TileGtk_XErrorHandler:\n");
  printf("    error_code   = %s (%d)\n", buf, errorPtr->error_code);
  printf("    request_code = %d\n", errorPtr->request_code);
  printf("    minor_code   = %d\n", errorPtr->minor_code);
  printf("===============================================================\n");
#endif /* TILEGTK_VERBOSE_XERROR_HANDLER */
  return TileGtk_TkXErrorHandler(displayPtr, errorPtr);
}; /* TileGtk_XErrorHandler */

static int TileGtk_XEventHandler(ClientData clientData, XEvent *eventPtr) {
  const char *tcl_callback;
  int status;
  if (eventPtr->type != ClientMessage) return 0;
  // Atom TileGtk_KIPC_COMM_ATOM = XInternAtom(eventPtr->xclient.display,
  //                                          "KIPC_COMM_ATOM" , false);
  Atom TileGtk_KIPC_COMM_ATOM = None;
  if (eventPtr->xclient.message_type != TileGtk_KIPC_COMM_ATOM) return 0;
  /* The following data variable contains the type of the KIPC message,
   * As defined in gnomelibs/gnomecore/kipc.h:
   * PaletteChanged      = 0
   * StyleChanged        = 2
   * ToolbarStyleChanged = 6
   */
  switch (eventPtr->xclient.data.l[0]) {
    case 0:   /* PaletteChanged      */
      tcl_callback = "tile::theme::tilegtk::gnomePaletteChangeNotification";
      break;
    case 2:   /* StyleChanged        */
    case 6: { /* ToolbarStyleChanged */
      tcl_callback = "tile::theme::tilegtk::gnomeStyleChangeNotification";
      break;
    }
    default: {
      return 0;
    }
  }
  Tcl_Interp *interp = (Tcl_Interp *) clientData;
  if (interp == NULL) return 0;
  // printf("TileGtk_XEventHandler: %p\n", interp); fflush(NULL);
  /* Notify the tile engine about the change... */
  status = Tcl_Eval(interp, tcl_callback);
  if (status != TCL_OK) Tcl_BackgroundError(interp);
  /* Do not remove True: As many interpreters may have registered this event
   * handler, allow Tk to call all of them! */
  return True;
} /* TileGtk_XEventHandler */
