INCLUDES = -I$(top_srcdir) -I$(top_srcdir)/libxslt -I$(top_srcdir)/libexslt \
           -I$(top_builddir) -I$(top_builddir)/libxslt \
	   -I$(top_builddir)/libexslt $(LIBXML_CFLAGS) $(CFLAGS)

EXTRA_PROGRAMS=
bin_PROGRAMS = xsltproc $(XSLTPROCDV)

xsltproc_SOURCES = xsltproc.c
xsltproc_LDFLAGS =
xsltproc_DEPENDENCIES = $(DEPS)
xsltproc_LDADD = $(LDADDS)

DEPS = $(top_builddir)/libxslt/libxslt.la \
	$(top_builddir)/libexslt/libexslt.la 

LDADDS = $(top_builddir)/libxslt/libxslt.la \
         $(top_builddir)/libexslt/libexslt.la \
	@LIBXML_LIBS@ $(EXTRA_LIBS)

xsltproc.dv: xsltproc.o
	$(CC) $(CFLAGS) -o xsltproc xsltproc.o ../libexslt/.libs/libexslt.a ../libxslt/.libs/libxslt.a @LIBXML_LIBS@ $(EXTRA_LIBS)
