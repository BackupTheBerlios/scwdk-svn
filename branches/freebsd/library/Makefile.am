INCLUDES = -I/usr/local/include/sigc++-2.0 -I/usr/local/lib/sigc++-2.0/include
METASOURCES = AUTO
lib_LTLIBRARIES = libscwdk.la
libscwdk_la_LIBADD = -lncurses -lpthread -L/usr/local/lib -lsigc-2.0
libscwdk_la_SOURCES = events.cpp exstring.cpp layout.cpp linuxdl.cpp mutex.cpp \
	pxy.cpp rect.cpp size.cpp swcontrol.cpp swdesktop.cpp swmain.cpp swncurses.cpp \
	swobject.cpp swtattr.cpp swtext.cpp swthread.cpp swuicontrol.cpp swwriter.cpp
include_HEADERS = events.h exstring.h layout.h linuxdl.h msgdefs.h mutex.h \
	pxy.h rect.h scwdk.h size.h swcontrol.h swdesktop.h swmain.h swobject.h \
	swtattr.h swtext.h swthread.h swuicontrol.h swwriter.h
