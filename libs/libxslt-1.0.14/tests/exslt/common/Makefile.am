## Process this file with automake to produce Makefile.in

$(top_builddir)/xsltproc/xsltproc:
	@(cd ../../../xsltproc ; $(MAKE) xsltproc)

EXTRA_DIST = 						\
	node-set.1.xml node-set.1.xsl node-set.1.out	\
	node-set.2.xml node-set.2.xsl node-set.2.out	\
	node-set.3.xml node-set.3.xsl node-set.3.out	\
	object-type.1.xml object-type.1.xsl object-type.1.out

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
