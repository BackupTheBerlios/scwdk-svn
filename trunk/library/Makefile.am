METASOURCES = AUTO
lib_LTLIBRARIES = libscwdk.la
libscwdk_la_LIBADD = $(sigc_LIBS) -ldl -lncurses -lpthread
libscwdk_la_SOURCES = eventdelegate.cpp events.cpp exstring.cpp layout.cpp \
	linuxdl.cpp mutex.cpp pxy.cpp rect.cpp size.cpp swattrgroup.cpp swcommand.cpp \
	swcontrol.cpp swdesktop.cpp swlabel.cpp swmain.cpp swncurses.cpp swobject.cpp \
	swtattr.cpp swtext.cpp swthread.cpp swuicontrol.cpp swwindow.cpp swwriter.cpp
include_HEADERS = events.h exstring.h layout.h linuxdl.h msgdefs.h mutex.h \
	pxy.h rect.h scwdk.h size.h swcommand.h swcontrol.h swdesktop.h swmain.h \
	swobject.h swtattr.h swtext.h swthread.h swuicontrol.h swwriter.h
noinst_HEADERS = eventdelegate.h swattrgroup.h swcommand.h swlabel.h swwindow.h
INCLUDES = $(sigc_CFLAGS)
