## Process this file with automake to produce Makefile.in

all:

tests: single xtchunk # Avoid doing the test in normal pass

$(top_builddir)/xsltproc/xsltproc:
	@(cd ../../xsltproc ; $(MAKE) xsltproc)

EXTRA_DIST = README VERSION

full: tests htmltests xhtmltests fotests

#
# a single tests to check that stuff ain't broken
#
single:
	@(echo > .memdump)
	@(for i in $(srcdir)/test/gdp-handbook.xml ; do \
	  echo -n "$$i : html " ; \
	  out=$(srcdir)/result/html/`basename $$i .xml`.tst; \
	  html=$(srcdir)/result/html/`basename $$i .xml`.html; \
	  $(top_builddir)/xsltproc/xsltproc --nonet $(srcdir)/html/docbook.xsl $$i > $$out ; \
	  if [ -f $$html ] ; then \
	      grep -v id < $$html > $$html.noid ; \
	      grep -v id < $$out > $$out.noid ; \
	      diff $$html.noid $$out.noid ; \
	      rm -f $$html.noid $$out.noid ; \
	  else mv $$out $$html ; fi ; \
	  grep "MORY ALLO" .memdump  | grep -v "MEMORY ALLOCATED : 0" || true; \
	  rm -f $$out ; \
	  echo -n "fo " ; \
	  out=$(srcdir)/result/fo/`basename $$i .xml`.tst; \
	  fo=$(srcdir)/result/fo/`basename $$i .xml`.fo; \
	  msg=$(srcdir)/result/fo/`basename $$i .xml`.msg; \
	  $(top_builddir)/xsltproc/xsltproc --nonet $(srcdir)/fo/docbook.xsl $$i > $$out 2> $$msg ; \
	  if [ -f $$fo ] ; then \
	      grep -v id < $$fo > $$fo.noid ; \
	      grep -v id < $$out > $$out.noid ; \
	      diff $$fo.noid $$out.noid ; \
	      rm -f $$fo.noid $$out.noid ; \
	  else mv $$out $$fo ; fi ; \
	  grep "MORY ALLO" .memdump  | grep -v "MEMORY ALLOCATED : 0" || true; \
	  rm -f $$out $$msg ; \
	  echo -n "xhtml " ; \
	  out=$(srcdir)/result/xhtml/`basename $$i .xml`.tst; \
	  xhtml=$(srcdir)/result/xhtml/`basename $$i .xml`.xhtml; \
	  $(top_builddir)/xsltproc/xsltproc --nonet $(srcdir)/xhtml/docbook.xsl $$i > $$out ; \
	  if [ -f $$xhtml ] ; then \
	      grep -v id < $$xhtml > $$xhtml.noid ; \
	      grep -v id < $$out > $$out.noid ; \
	      diff $$xhtml.noid $$out.noid ; \
	      rm -f $$xhtml.noid $$out.noid ; \
	  else mv $$out $$xhtml ; fi ; \
	  grep "MORY ALLO" .memdump  | grep -v "MEMORY ALLOCATED : 0" || true; \
	  rm -f $$out ; \
	  echo ; \
	  done )

xtchunk:
	@(echo > .memdump)
	@(for i in $(srcdir)/test/gdp-handbook.xml ; do \
	  echo "Testing HTML chunking on $$i :" ; \
	  html=$(srcdir)/result/html/`basename $$i .xml`.html; \
	  $(top_builddir)/xsltproc/xsltproc -o $(srcdir)/result/xtchunk/html/gdp-handbook $(srcdir)/html/xtchunk.xsl $$i ; \
	  for html in $(srcdir)/result/xtchunk/html/*.html ; do \
	      orig=$(srcdir)/result/xtchunk/html/`basename $$html .html`.orig ;\
	      if [ -f $$orig ] ; then \
		  grep -v id < $$html > $$html.noid ; \
		  grep -v id < $$orig > $$orig.noid ; \
		  diff $$orig.noid $$html.noid ; \
		  rm -f $$html.noid $$orig.noid $$html; \
	      else mv $$html $$orig ; fi ; \
	  done ; \
	  grep "MORY ALLO" .memdump  | grep -v "MEMORY ALLOCATED : 0" || true; \
	  rm -f $(srcdir)/result/xtchunk/html/gdp-handbook ; \
	  done )

#
# The full set of tests
#
htmltests: $(top_builddir)/xsltproc/xsltproc
	@(echo > .memdump)
	@echo "##"
	@echo "## HTML stylesheets regression tests"
	@echo "##"
	@(for i in $(srcdir)/test/*.xml ; do \
	  echo $$i ; \
	  out=$(srcdir)/result/html/`basename $$i .xml`.tst; \
	  html=$(srcdir)/result/html/`basename $$i .xml`.html; \
	  $(top_builddir)/xsltproc/xsltproc --nonet $(srcdir)/html/docbook.xsl $$i > $$out ; \
	  if [ -f $$html ] ; then \
	      grep -v id < $$html > $$html.noid ; \
	      grep -v id < $$out > $$out.noid ; \
	      diff $$html.noid $$out.noid ; \
	      rm -f $$html.noid $$out.noid ; \
	  else mv $$out $$html ; fi ; \
	  grep "MORY ALLO" .memdump  | grep -v "MEMORY ALLOCATED : 0" || true;\
	  rm -f $$out ; done )

fotests: $(top_builddir)/xsltproc/xsltproc
	@(echo > .memdump)
	@echo "##"
	@echo "## XSL FO stylesheets regression tests"
	@echo "##"
	@(for i in $(srcdir)/test/*.xml ; do \
	  echo $$i ; \
	  out=$(srcdir)/result/fo/`basename $$i .xml`.tst; \
	  fo=$(srcdir)/result/fo/`basename $$i .xml`.fo; \
	  msg=$(srcdir)/result/fo/`basename $$i .xml`.msg; \
	  $(top_builddir)/xsltproc/xsltproc --nonet $(srcdir)/fo/docbook.xsl $$i > $$out  2> $$msg ; \
	  if [ -f $$fo ] ; then \
	      grep -v id < $$fo > $$fo.noid ; \
	      grep -v id < $$out > $$out.noid ; \
	      diff $$fo.noid $$out.noid ; \
	      rm -f $$fo.noid $$out.noid ; \
	  else mv $$out $$fo ; fi ; \
	  grep "MORY ALLO" .memdump  | grep -v "MEMORY ALLOCATED : 0" || true;\
	  rm -f $$out $$msg ; \
	  done )

xhtmltests: $(top_builddir)/xsltproc/xsltproc
	@(echo > .memdump)
	@echo "##"
	@echo "## XHTML stylesheets regression tests"
	@echo "##"
	@(for i in $(srcdir)/test/*.xml ; do \
	  echo $$i ; \
	  out=$(srcdir)/result/xhtml/`basename $$i .xml`.tst; \
	  xhtml=$(srcdir)/result/xhtml/`basename $$i .xml`.xhtml; \
	  $(top_builddir)/xsltproc/xsltproc --nonet $(srcdir)/xhtml/docbook.xsl $$i > $$out ; \
	  if [ -f $$xhtml ] ; then \
	      grep -v id < $$xhtml > $$xhtml.noid ; \
	      grep -v id < $$out > $$out.noid ; \
	      diff $$xhtml.noid $$out.noid ; \
	      rm -f $$xhtml.noid $$out.noid ; \
	  else mv $$out $$xhtml ; fi ; \
	  grep "MORY ALLO" .memdump  | grep -v "MEMORY ALLOCATED : 0" || true;\
	  rm -f $$out ; done )

dist-hook:
	@(cd $(srcdir) ; tar -cf - --exclude CVS common html dtd lib xhtml fo doc images template htmlhelp result test) | (cd $(distdir); tar xf -)
