## Process this file with automake to produce Makefile.in

$(top_builddir)/xsltproc/xsltproc:
	@(cd ../../xsltproc ; $(MAKE) xsltproc)

EXTRA_DIST = doc.xsl doc.xml doc.dtd result.xml

all:

test tests: $(top_builddir)/xsltproc/xsltproc
	@(echo > .memdump)
	@($(top_builddir)/xsltproc/xsltproc $(srcdir)/doc.xsl $(srcdir)/doc.xml > doc.res ; \
	diff $(srcdir)/result.xml doc.res ; \
	grep "MORY ALLO" .memdump  | grep -v "MEMORY ALLOCATED : 0" || true;\
	rm -f doc.res)

