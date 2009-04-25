bin_PROGRAMS = widgetcurses
widgetcurses_SOURCES = myapplication.cpp netspeedsensortest.cpp \
	widgetcurses.cpp

# set the include path found by configure
INCLUDES = -I$(top_srcdir)/src/libWidgetCurses $(CCDELEGATES_CFLAGS) \
	$(all_includes)

# the library search path.
widgetcurses_LDFLAGS = $(all_libraries) 
SUBDIRS = libWidgetCurses
widgetcurses_LDADD = $(top_builddir)/src/libWidgetCurses/libWidgetCurses.la \
	$(CCDELEGATES_LIBS)
noinst_HEADERS = myapplication.h netspeedsensortest.h
