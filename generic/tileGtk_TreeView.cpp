/*
 *  tileGtk_TreeView.cpp
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

#define ITEM_LAYOUT		"Item"
#define CELL_LAYOUT		"Cell"
#define HEADING_LAYOUT		"Heading"
#define ROW_LAYOUT		"Row"

/*------------------------------------------------------------------------
 * +++ TreeView Field Element.
 */
 
/*
 * Map between Tk/Tile & Gtk/GNOME state flags.
 */
static Ttk_StateTable treeview_field_statemap[] =
{
#ifdef TILEGTK_GTK_VERSION_3
    {QStyle::Style_Enabled|QStyle::Style_Down,      TTK_STATE_PRESSED, 0},
    {QStyle::Style_Enabled,                         TTK_STATE_ACTIVE, 0},
    {QStyle::Style_Enabled,                         0, 0}
#endif /* TILEGTK_GTK_VERSION_3 */
#ifdef TILEGTK_GTK_VERSION_4
    {QStyle::State_Enabled|QStyle::State_Sunken,    TTK_STATE_PRESSED, 0},
    {QStyle::State_Enabled,                         TTK_STATE_ACTIVE, 0},
    {QStyle::State_Enabled,                         0, 0}
#endif /* TILEGTK_GTK_VERSION_4 */
};

typedef struct {
} TreeViewFieldElement;


static Ttk_ElementOptionSpec TreeViewFieldElementOptions[] = {
    {NULL}
};

static void TreeViewFieldElementGeometry(
    void *clientData, void *elementRecord, Tk_Window tkwin,
    int *widthPtr, int *heightPtr, Ttk_Padding *paddingPtr)
{
    if (!TileGtk_GtkInitialised()) NO_GTK_STYLE_ENGINE;
    *paddingPtr = Ttk_UniformPadding(0);
}

static void TreeViewFieldElementDraw(
    void *clientData, void *elementRecord, Tk_Window tkwin,
    Drawable d, Ttk_Box b, unsigned state)
{
    if (!TileGtk_GtkInitialised()) NO_GTK_STYLE_ENGINE;
    NULL_PROXY_WIDGET(TileGtk_QWidget_Widget);
    Tcl_MutexLock(&tilegtkMutex);
    QPixmap      pixmap(b.width, b.height);
    QPainter     painter(&pixmap);
    TILEGTK_PAINT_BACKGROUND_BASE(b.width, b.height);
    TILEGTK_SET_FOCUS(state);
#ifdef TILEGTK_GTK_VERSION_3
    QStyle::SFlags sflags =TileGtk_StateTableLookup(treeview_field_statemap, state);
    sflags |= QStyle::Style_Horizontal;
    wc->TileGtk_Style->drawPrimitive(QStyle::PE_GroupBoxFrame, &painter,
          QRect(0, 0, b.width, b.height), qApp->palette().active(), sflags,
          QStyleOption(/*lineWidth*/1, /*midLineWidth*/0,
                       /*frameShape*/QFrame::StyledPanel,
                       /*frameShadow*/QFrame::Sunken));
    wc->TileGtk_Style->drawPrimitive(QStyle::PE_PanelLineEdit, &painter,
          QRect(0, 0, b.width, b.height), qApp->palette().active(), sflags,
          QStyleOption(1,1));
#endif /* TILEGTK_GTK_VERSION_3 */
#ifdef TILEGTK_GTK_VERSION_4
    QStyleOptionFrame option;
    option.rect = QRect(0, 0, b.width, b.height);
    option.lineWidth = 1;
    option.state |= 
      (QStyle::StateFlag) TileGtk_StateTableLookup(treeview_field_statemap, state);
    wc->TileGtk_Style->drawPrimitive(QStyle::PE_FrameGroupBox, &option,
                                    &painter);
#endif /* TILEGTK_GTK_VERSION_4 */
    TILEGTK_CLEAR_FOCUS(state);
    // printf("x=%d, y=%d, w=%d, h=%d\n", b.x, b.y, b.width, b.height);
    TileGtk_CopyGtkPixmapOnToDrawable(gdkDrawable, d, tkwin,
                                    0, 0, b.width, b.height, b.x, b.y);
    Tcl_MutexUnlock(&tilegtkMutex);
}

static Ttk_ElementSpec TreeViewFieldElementSpec = {
    TK_STYLE_VERSION_2,
    sizeof(TreeViewFieldElement),
    TreeViewFieldElementOptions,
    TreeViewFieldElementGeometry,
    TreeViewFieldElementDraw
};

/*------------------------------------------------------------------------
 * +++ TreeView Client Element.
 */
 
/*
 * Map between Tk/Tile & Gtk/GNOME state flags.
 */
static Ttk_StateTable treeview_client_statemap[] =
{
#ifdef TILEGTK_GTK_VERSION_3
    {QStyle::Style_Enabled|QStyle::Style_Down,      TTK_STATE_PRESSED, 0},
    {QStyle::Style_Enabled,                         TTK_STATE_ACTIVE, 0},
    {QStyle::Style_Enabled,                         0, 0}
#endif /* TILEGTK_GTK_VERSION_3 */
#ifdef TILEGTK_GTK_VERSION_4
    {QStyle::State_Enabled|QStyle::State_Sunken,    TTK_STATE_PRESSED, 0},
    {QStyle::State_Enabled,                         TTK_STATE_ACTIVE, 0},
    {QStyle::State_Enabled,                         0, 0}
#endif /* TILEGTK_GTK_VERSION_4 */
};

typedef struct {
} TreeViewClientElement;


static Ttk_ElementOptionSpec TreeViewClientElementOptions[] = {
    {NULL}
};

static void TreeViewClientElementGeometry(
    void *clientData, void *elementRecord, Tk_Window tkwin,
    int *widthPtr, int *heightPtr, Ttk_Padding *paddingPtr)
{
    if (!TileGtk_GtkInitialised()) NO_GTK_STYLE_ENGINE;
    *paddingPtr = Ttk_UniformPadding(0);
}

static void TreeViewClientElementDraw(
    void *clientData, void *elementRecord, Tk_Window tkwin,
    Drawable d, Ttk_Box b, unsigned state)
{
    if (!TileGtk_GtkInitialised()) NO_GTK_STYLE_ENGINE;
    NULL_PROXY_WIDGET(TileGtk_QWidget_Widget);
    Tcl_MutexLock(&tilegtkMutex);
    QPixmap      pixmap(b.width, b.height);
    QPainter     painter(&pixmap);
    TILEGTK_PAINT_BACKGROUND_BASE(b.width, b.height);
#if 0
#ifdef TILEGTK_GTK_VERSION_3
    QStyle::SFlags sflags =
                   TileGtk_StateTableLookup(treeview_client_statemap, state);
    sflags |= QStyle::Style_Horizontal;
    wc->TileGtk_Style->drawPrimitive(QStyle::PE_GroupBoxFrame, &painter,
          QRect(0, 0, b.width, b.height), qApp->palette().active(), sflags,
          QStyleOption(/*lineWidth*/1, /*midLineWidth*/0,
                       /*frameShape*/QFrame::StyledPanel,
                       /*frameShadow*/QFrame::Sunken));
#endif /* TILEGTK_GTK_VERSION_3 */
#ifdef TILEGTK_GTK_VERSION_4
    QStyleOptionFrame option;
    option.rect = QRect(0, 0, b.width, b.height);
    option.lineWidth = 1;
    option.state |= (QStyle::StateFlag) 
                    TileGtk_StateTableLookup(treeview_client_statemap, state);
    wc->TileGtk_Style->drawPrimitive(QStyle::PE_FrameGroupBox, &option,
                                    &painter);
#endif /* TILEGTK_GTK_VERSION_4 */
#endif
    // printf("x=%d, y=%d, w=%d, h=%d\n", b.x, b.y, b.width, b.height);
    TileGtk_CopyGtkPixmapOnToDrawable(gdkDrawable, d, tkwin,
                                    0, 0, b.width, b.height, b.x, b.y);
    Tcl_MutexUnlock(&tilegtkMutex);
}

static Ttk_ElementSpec TreeViewClientElementSpec = {
    TK_STYLE_VERSION_2,
    sizeof(TreeViewClientElement),
    TreeViewClientElementOptions,
    TreeViewClientElementGeometry,
    TreeViewClientElementDraw
};

/*------------------------------------------------------------------------
 * +++ TreeView Header Border Element.
 */
 
/*
 * Map between Tk/Tile & Gtk/GNOME state flags.
 */
static Ttk_StateTable treeheading_border_statemap[] =
{
#ifdef TILEGTK_GTK_VERSION_3
    {QStyle::Style_Enabled|QStyle::Style_Down,      TTK_STATE_PRESSED, 0},
    {QStyle::Style_Enabled,                         TTK_STATE_ACTIVE, 0},
    {QStyle::Style_Enabled,                         0, 0}
#endif /* TILEGTK_GTK_VERSION_3 */
#ifdef TILEGTK_GTK_VERSION_4
    {QStyle::State_Enabled|QStyle::State_Sunken,    TTK_STATE_PRESSED, 0},
    {QStyle::State_Enabled,                         TTK_STATE_ACTIVE, 0},
    {QStyle::State_Enabled,                         0, 0}
#endif /* TILEGTK_GTK_VERSION_4 */
};

typedef struct {
} TreeHeadingBorderElement;


static Ttk_ElementOptionSpec TreeHeadingBorderElementOptions[] = {
    {NULL}
};

static void TreeHeadingBorderElementGeometry(
    void *clientData, void *elementRecord, Tk_Window tkwin,
    int *widthPtr, int *heightPtr, Ttk_Padding *paddingPtr)
{
    if (!TileGtk_GtkInitialised()) NO_GTK_STYLE_ENGINE;
    *paddingPtr = Ttk_UniformPadding(TreeViewUniformPadding);
}

static void TreeHeadingBorderElementDraw(
    void *clientData, void *elementRecord, Tk_Window tkwin,
    Drawable d, Ttk_Box b, unsigned state)
{
    if (!TileGtk_GtkInitialised()) NO_GTK_STYLE_ENGINE;
    NULL_PROXY_WIDGET(TileGtk_QWidget_Widget);
    Tcl_MutexLock(&tilegtkMutex);
    QPixmap      pixmap(b.width, b.height);
    QPainter     painter(&pixmap);
    TILEGTK_PAINT_BACKGROUND(b.width, b.height);
#ifdef TILEGTK_GTK_VERSION_3
    QStyle::SFlags sflags = TileGtk_StateTableLookup(treeheading_border_statemap,
                                                 state);
    sflags |= QStyle::Style_Horizontal;
    wc->TileGtk_Style->drawPrimitive(QStyle::PE_HeaderSection, &painter,
          QRect(0, 0, b.width, b.height), qApp->palette().active(), sflags);
#endif /* TILEGTK_GTK_VERSION_3 */
#ifdef TILEGTK_GTK_VERSION_4
    QStyleOptionHeader option;
    option.rect = QRect(0, 0, b.width, b.height);
    option.state |= 
      (QStyle::StateFlag) TileGtk_StateTableLookup(treeview_client_statemap, state);
    wc->TileGtk_Style->drawControl(QStyle::CE_HeaderSection, &option,
                                  &painter);
#endif /* TILEGTK_GTK_VERSION_4 */
    // printf("x=%d, y=%d, w=%d, h=%d\n", b.x, b.y, b.width, b.height);
    TileGtk_CopyGtkPixmapOnToDrawable(gdkDrawable, d, tkwin,
                                    0, 0, b.width, b.height, b.x, b.y);
    Tcl_MutexUnlock(&tilegtkMutex);
}

static Ttk_ElementSpec TreeHeadingBorderElementSpec = {
    TK_STYLE_VERSION_2,
    sizeof(TreeHeadingBorderElement),
    TreeHeadingBorderElementOptions,
    TreeHeadingBorderElementGeometry,
    TreeHeadingBorderElementDraw
};


/*------------------------------------------------------------------------
 * +++ Widget layout.
 */

#if 0
TTK_BEGIN_LAYOUT(TreeviewLayout)
    TTK_GROUP("Treeview.field", TTK_FILL_BOTH|TTK_BORDER,
	TTK_GROUP("Treeview.padding", TTK_FILL_BOTH,
	    TTK_NODE("Treeview.client", TTK_FILL_BOTH)))
TTK_END_LAYOUT
#endif

TTK_BEGIN_LAYOUT(ItemLayout)
    TTK_GROUP("Treeitem.padding", TTK_FILL_BOTH,
	TTK_NODE("Treeitem.indicator", TTK_PACK_LEFT)
	TTK_NODE("Treeitem.image", TTK_PACK_LEFT)
	TTK_GROUP("Treeitem.focus", TTK_PACK_LEFT,
	    TTK_NODE("Treeitem.text", TTK_PACK_LEFT)))
TTK_END_LAYOUT

TTK_BEGIN_LAYOUT(CellLayout)
    TTK_GROUP("Treedata.padding", TTK_FILL_BOTH,
	TTK_NODE("Treeitem.label", TTK_FILL_BOTH))
TTK_END_LAYOUT

TTK_BEGIN_LAYOUT(HeadingLayout)
    TTK_NODE("Treeheading.cell", TTK_FILL_BOTH)
    TTK_GROUP("Treeheading.border", TTK_FILL_BOTH,
	TTK_NODE("Treeheading.image", TTK_PACK_RIGHT)
	TTK_NODE("Treeheading.text", 0))
TTK_END_LAYOUT

TTK_BEGIN_LAYOUT(RowLayout)
    TTK_NODE("Treeitem.row", TTK_FILL_BOTH)
TTK_END_LAYOUT

int TileGtk_Init_TreeView(Tcl_Interp *interp,
                       TileGtk_WidgetCache **wc, Ttk_Theme themePtr)
{
    /*
     * Register elements:
     */
    Ttk_RegisterElement(interp, themePtr, "Treeview.field",
            &TreeViewFieldElementSpec, (void *) wc[0]);
    //Ttk_RegisterElement(interp, themePtr, "Treeview.client",
    //        &TreeViewClientElementSpec, (void *) wc[0]);
    Ttk_RegisterElement(interp, themePtr, "Treeheading.border",
            &TreeHeadingBorderElementSpec, (void *) wc[0]);
    
    /*
     * Register layouts:
     */
    Ttk_RegisterLayout(themePtr, ITEM_LAYOUT, ItemLayout);
    Ttk_RegisterLayout(themePtr, CELL_LAYOUT, CellLayout);
    Ttk_RegisterLayout(themePtr, HEADING_LAYOUT, HeadingLayout);
    Ttk_RegisterLayout(themePtr, ROW_LAYOUT, RowLayout);

    return TCL_OK;
}; /* TileGtk_Init_TreeView */
