## Process this file with automake to produce Makefile.in

$(top_builddir)/xsltproc/xsltproc:
	@(cd ../../../xsltproc ; $(MAKE) xsltproc)

EXTRA_DIST = 							\
  difference.1.out     difference.1.xml     difference.1.xsl	\
  distinct.1.out       distinct.1.xml       distinct.1.xsl	\
  has-same-node.1.out  has-same-node.1.xml  has-same-node.1.xsl	\
  leading.1.out        leading.1.xml        leading.1.xsl	\
  trailing.1.out       trailing.1.xml       trailing.1.xsl

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
