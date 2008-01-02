bin_PROGRAMS = scwdk
scwdk_SOURCES = scwdk.cpp

# set the include path found by configure
INCLUDES = -I$(top_srcdir)/library $(all_includes) \
	-I/usr/local/include/sigc++-2.0 -I/usr/local/lib/sigc++-2.0/include

# the library search path.
scwdk_LDFLAGS = $(all_libraries) 
scwdk_LDADD = $(top_builddir)/library/libscwdk.la
