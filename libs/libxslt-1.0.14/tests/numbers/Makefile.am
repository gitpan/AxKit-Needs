## Process this file with automake to produce Makefile.in

$(top_builddir)/xsltproc/xsltproc:
	@(cd ../../xsltproc ; $(MAKE) xsltproc)

EXTRA_DIST = format-number.xsl format-number.xml format-number.out

all:

test tests: $(top_builddir)/xsltproc/xsltproc
	@(echo > .memdump)
	@($(top_builddir)/xsltproc/xsltproc $(srcdir)/format-number.xsl $(srcdir)/format-number.xml > format-number.res ; \
	diff $(srcdir)/format-number.out format-number.res ; \
	grep "MORY ALLO" .memdump  | grep -v "MEMORY ALLOCATED : 0" || true;\
	rm -f format-number.res)

