# - try to find GNOME library
#  GNOME_INCLUDE_DIR   - Directories to include to use GNOME
#  GNOME_LIBRARIES     - Files to link against to use GNOME
#  GNOME_FOUND         - GNOME was found

SET ( GNOME_ADDITIONAL_INCLUDE_PATHS
  "$ENV{ProgramFiles}/gtk+-bundle-2.12.11/include"
  "$ENV{ProgramFiles}/gtk+/include"
  "$ENV{ProgramFiles}/gtk/include"
)

SET ( GNOME_ADDITIONAL_LIBRARY_PATHS
  "$ENV{ProgramFiles}/gtk+-bundle-2.12.11/lib"
  "$ENV{ProgramFiles}/gtk+/lib"
  "$ENV{ProgramFiles}/gtk/lib"
)

FIND_PATH ( GNOME_libgnome_INCLUDE_PATH NAMES libgnome/libgnome.h
  PATH_SUFFIXES libgnome-2.0 libgnome-20 libgnome20 libgnome2
                gnome-2.0    gnome-20    gnome20    gnome2
  PATHS
  /usr/include
  /usr/openwin/share/include
  /usr/openwin/include
  /opt/gnome/include
  ${GNOME_ADDITIONAL_INCLUDE_PATHS}
)

FIND_PATH ( GNOME_libgnomeui_INCLUDE_PATH NAMES libgnomeui/libgnomeui.h
  PATH_SUFFIXES libgnomeui-2.0 libgnomeui-20 libgnomeui20 libgnomeui2
                gnomeui-2.0    gnomeui-20    gnomeui20    gnomeui2
  PATHS
  /usr/include
  /usr/openwin/share/include
  /usr/openwin/include
  /opt/gnome/include
  ${GNOME_ADDITIONAL_INCLUDE_PATHS}
)

FIND_PATH ( GNOME_libgnomecanvas_INCLUDE_PATH
  NAMES libgnomecanvas/libgnomecanvas.h
  PATH_SUFFIXES libgnomecanvas-2.0 libgnomecanvas-20
                libgnomecanvas20   libgnomecanvas2
                gnomecanvas-2.0    gnomecanvas-20
                gnomecanvas20      gnomecanvas2
  PATHS
  /usr/include
  /usr/openwin/share/include
  /usr/openwin/include
  /opt/gnome/include
  ${GNOME_ADDITIONAL_INCLUDE_PATHS}
)

FIND_PATH ( GNOME_libgnomevfs_INCLUDE_PATH
  NAMES libgnomevfs/gnome-vfs.h
  PATH_SUFFIXES libgnome-vfs-2.0 libgnome-vfs-20
                libgnome-vfs20   libgnome-vfs2
                gnome-vfs-2.0    gnome-vfs-20
                gnome-vfs20      gnome-vfs2
  PATHS
  /usr/include
  /usr/openwin/share/include
  /usr/openwin/include
  /opt/gnome/include
  ${GNOME_ADDITIONAL_INCLUDE_PATHS}
)

FIND_PATH ( GNOME_libart_INCLUDE_PATH NAMES libart_lgpl/libart.h
  PATH_SUFFIXES libart-2.0 libart-20 libart20 libart2
                art-2.0    art-20    art20    art2
  PATHS
  /usr/include
  /usr/openwin/share/include
  /usr/openwin/include
  /opt/gnome/include
  ${GNOME_ADDITIONAL_INCLUDE_PATHS}
)

FIND_PATH ( GNOME_bonobo_INCLUDE_PATH NAMES bonobo/bonobo-i18n.h
  PATH_SUFFIXES libbonobo-2.0 libbonobo-20 libbonobo20 libbonobo2
                bonobo-2.0    bonobo-20    bonobo20    bonobo2
  PATHS
  /usr/include
  /usr/openwin/share/include
  /usr/openwin/include
  /opt/gnome/include
  ${GNOME_ADDITIONAL_INCLUDE_PATHS}
)

FIND_PATH ( GNOME_bonoboui_INCLUDE_PATH NAMES bonobo/bonobo-dock.h
  PATH_SUFFIXES libbonoboui-2.0 libbonoboui-20 libbonoboui20 libbonoboui2
                bonoboui-2.0    bonoboui-20    bonoboui20    bonoboui2
  PATHS
  /usr/include
  /usr/openwin/share/include
  /usr/openwin/include
  /opt/gnome/include
  ${GNOME_ADDITIONAL_INCLUDE_PATHS}
)

FIND_LIBRARY ( GNOME_gnome_LIBRARY
  NAMES gnome-2 gnome
  PATHS
  /usr/openwin/lib
  /opt/gnome/lib
  /usr/lib
  ${GNOME_ADDITIONAL_LIBRARY_PATHS}
)

FIND_LIBRARY ( GNOME_gnomeui_LIBRARY
  NAMES gnomeui-2 gnomeui
  PATHS
  /usr/openwin/lib
  /opt/gnome/lib
  /usr/lib
  ${GNOME_ADDITIONAL_LIBRARY_PATHS}
)

IF ( GNOME_libgnome_INCLUDE_PATH AND GNOME_gnome_LIBRARY )
  SET ( GNOME_FOUND "YES" )
  SET ( GNOME_INCLUDE_DIR ${GNOME_libgnome_INCLUDE_PATH} )
  IF ( GNOME_libgnomeui_INCLUDE_PATH )
    SET ( GNOME_INCLUDE_DIR ${GNOME_INCLUDE_DIR}
                            ${GNOME_libgnomeui_INCLUDE_PATH} )
  ENDIF ( GNOME_libgnomeui_INCLUDE_PATH )
  IF ( GNOME_libgnomecanvas_INCLUDE_PATH )
    SET ( GNOME_INCLUDE_DIR ${GNOME_INCLUDE_DIR}
                            ${GNOME_libgnomecanvas_INCLUDE_PATH} )
  ENDIF ( GNOME_libgnomecanvas_INCLUDE_PATH )
  IF ( GNOME_libgnomevfs_INCLUDE_PATH )
    SET ( GNOME_INCLUDE_DIR ${GNOME_INCLUDE_DIR}
                            ${GNOME_libgnomevfs_INCLUDE_PATH} )
  ENDIF ( GNOME_libgnomevfs_INCLUDE_PATH )
  IF ( GNOME_libart_INCLUDE_PATH )
    SET ( GNOME_INCLUDE_DIR ${GNOME_INCLUDE_DIR} ${GNOME_libart_INCLUDE_PATH} )
  ENDIF ( GNOME_libart_INCLUDE_PATH )
  IF ( GNOME_bonobo_INCLUDE_PATH )
    SET ( GNOME_INCLUDE_DIR ${GNOME_INCLUDE_DIR} ${GNOME_bonobo_INCLUDE_PATH} )
  ENDIF ( GNOME_bonobo_INCLUDE_PATH )
  IF ( GNOME_bonoboui_INCLUDE_PATH )
    SET ( GNOME_INCLUDE_DIR ${GNOME_INCLUDE_DIR} ${GNOME_bonoboui_INCLUDE_PATH})
  ENDIF ( GNOME_bonoboui_INCLUDE_PATH )


  SET ( GNOME_LIBRARIES ${GNOME_gnome_LIBRARY} )
  IF ( GNOME_gnomeui_LIBRARY )
    SET ( GNOME_LIBRARIES ${GNOME_LIBRARIES} ${GNOME_gnomeui_LIBRARY} )
  ENDIF ( GNOME_gnomeui_LIBRARY )

  MARK_AS_ADVANCED (
    GNOME_libgnome_INCLUDE_PATH
    GNOME_libgnomeui_INCLUDE_PATH
    GNOME_libgnomecanvas_INCLUDE_PATH
    GNOME_libgnomevfs_INCLUDE_PATH
    GNOME_libart_INCLUDE_PATH
    GNOME_bonobo_INCLUDE_PATH
    GNOME_bonoboui_INCLUDE_PATH
    GNOME_gnome_LIBRARY
    GNOME_gnomeui_LIBRARY
  )
ENDIF ( GNOME_libgnome_INCLUDE_PATH AND GNOME_gnome_LIBRARY )
