INCLUDES = -I$(top_srcdir)/src/libWidgetCurses $(CCDELEGATES_CFLAGS) \
	$(all_includes)
METASOURCES = AUTO
lib_LTLIBRARIES = libWidgetCurses.la
include_HEADERS = StyledString.h application.h ccutils.h cursesattr.h \
	cursesterm.h events.h exstring.h label.h layout.h msgdefs.h mutex.h object.h \
	painter.h pxy.h rect.h size.h thread.h waitcontrol.h widget.h window.h
libWidgetCurses_la_SOURCES = StyledString.cpp application.cpp cursesattr.cpp \
	cursesterm.cpp events.cpp exstring.cpp label.cpp layout.cpp mutex.cpp object.cpp \
	painter.cpp pxy.cpp rect.cpp screen.cpp size.cpp thread.cpp waitcontrol.cpp \
	widget.cpp window.cpp
libWidgetCurses_la_LIBADD = -lncurses -lpthread $(CCDELEGATES_CLIBS)
noinst_HEADERS = application.h label.h screen.h widget.h window.h
_SOURCES = Window
_SOURCES = Application Window
_SOURCES = Application Layouts Window
_SOURCES = Application Layouts Widget Window
_SOURCES = Application Layouts Style Widget Window
_SOURCES = Application Layouts String Style Widget Window
_SOURCES = Application Events Layouts String Style Widget Window
_SOURCES = Application Events Layouts Object String Style Widget Window
_SOURCES = Application Events Layouts Object String Style Widget Window \
	xyCoords
_SOURCES = Application Events Layouts Object String Style Thread Widget Window \
	xyCoords
_SOURCES = Application Events Layouts Object Rect String Style Thread Widget \
	Window xyCoords
_SOURCES = Label
