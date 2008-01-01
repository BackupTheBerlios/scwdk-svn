bin_PROGRAMS = scwdk
scwdk_SOURCES = scwdk.cpp

# set the include path found by configure
INCLUDES = -I$(top_srcdir)/library -I/usr/lib/sigc++-2.0/include \
	-I/usr/include/sigc++-2.0 $(all_includes)

# the library search path.
scwdk_LDFLAGS = $(all_libraries) 
scwdk_LDADD = $(top_builddir)/library/libscwdk.la
