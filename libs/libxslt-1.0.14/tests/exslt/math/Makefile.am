## Process this file with automake to produce Makefile.in

$(top_builddir)/xsltproc/xsltproc:
	@(cd ../../../xsltproc ; $(MAKE) xsltproc)

EXTRA_DIST = 						\
	highest.1.out  highest.1.xml  highest.1.xsl	\
	highest.2.out  highest.2.xml  highest.2.xsl	\
	highest.5.out  highest.5.xml  highest.5.xsl	\
	lowest.1.out   lowest.1.xml   lowest.1.xsl	\
	lowest.2.out   lowest.2.xml   lowest.2.xsl	\
	max.1.out      max.1.xml      max.1.xsl		\
	max.2.out      max.2.xml      max.2.xsl		\
	max.5.out      max.5.xml      max.5.xsl		\
	min.1.out      min.1.xml      min.1.xsl		\
	min.2.out      min.2.xml      min.2.xsl

all:

test tests: $(top_builddir)/xsltproc/xsltproc
	@(echo > .memdump)
	@(for i in $(srcdir)/*.xsl ; do \
	  name=`basename $$i .xsl` ; \
	  if [ ! -f $(srcdir)/$$name.xml ] ; then continue ; fi ; \
	  echo $$name.xml ; \
	  $(top_builddir)/xsltproc/xsltproc $(srcdir)/$$name.xsl $(srcdir)/$$name.xml > $$name.res;\
	  if [ ! -f $(srcdir)/$$name.out ] ; then cp $$name.res $(srcdir)/$$name.out ; \
	  else if  [ ! -s $$name.res ] ; then echo "Fatal error, no $$name.res\n" ; \
	  else diff $(srcdir)/$$name.out $$name.res ; fi ; fi; \
	  grep "MORY ALLO" .memdump  | grep -v "MEMORY ALLOCATED : 0" || true;\
	  rm -f $$name.res ; \
	  done)
