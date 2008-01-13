INCLUDES = -I/usr/lib/sigc++-2.0/include -I/usr/include/sigc++-2.0
METASOURCES = AUTO
lib_LTLIBRARIES = libscwdk.la
libscwdk_la_LIBADD = -lncurses -ldl -lpthread -lsigc-2.0
libscwdk_la_SOURCES = events.cpp exstring.cpp layout.cpp linuxdl.cpp mutex.cpp \
	pxy.cpp rect.cpp size.cpp swcommand.cpp swcontrol.cpp swdesktop.cpp swmain.cpp \
	swncurses.cpp swobject.cpp swtattr.cpp swtext.cpp swthread.cpp swuicontrol.cpp \
	swwriter.cpp
include_HEADERS = events.h exstring.h layout.h linuxdl.h msgdefs.h mutex.h \
	pxy.h rect.h scwdk.h size.h swcommand.h swcontrol.h swdesktop.h swmain.h \
	swobject.h swtattr.h swtext.h swthread.h swuicontrol.h swwriter.h
noinst_HEADERS = swcommand.h
