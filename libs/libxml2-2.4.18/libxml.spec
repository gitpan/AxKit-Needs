Summary: Library providing XML and HTML support
Name: libxml2
Version: 2.4.18
Release: 1
License: MIT
Group: Development/Libraries
Source: ftp://xmlsoft.org/libxml2-%{version}.tar.gz
BuildRoot: %{_tmppath}/%{name}-%{version}-root
BuildRequires: python python-devel
URL: http://xmlsoft.org/
Prefix: %{_prefix}
Docdir: %{_docdir}

%description
This library allows to manipulate XML files. It includes support 
to read, modify and write XML and HTML files. There is DTDs support
this includes parsing and validation even with complex DtDs, either
at parse time or later once the document has been modified. The output
can be a simple SAX stream or and in-memory DOM like representations.
In this case one can use the built-in XPath and XPointer implementation
to select subnodes or ranges. A flexible Input/Output mechanism is
available, with existing HTTP and FTP modules and combined to an
URI library.

%package devel
Summary: Libraries, includes, etc. to develop XML and HTML applications
Group: Development/Libraries
Requires: libxml2 = %{version}

%description devel
Libraries, include files, etc you can use to develop XML applications.
This library allows to manipulate XML files. It includes support 
to read, modify and write XML and HTML files. There is DTDs support
this includes parsing and validation even with complex DtDs, either
at parse time or later once the document has been modified. The output
can be a simple SAX stream or and in-memory DOM like representations.
In this case one can use the built-in XPath and XPointer implementation
to select subnodes or ranges. A flexible Input/Output mechanism is
available, with existing HTTP and FTP modules and combined to an
URI library.

%package python
Summary: Python bindings for the libxml2 library
Group: Development/Libraries
Requires: libxml2 = %{version}
Requires: python

%description python
The libxml2-python package contains a module that permits applications
written in the Python programming language to use the interface
supplied by the libxml2 library to manipulate XML files.

This library allows to manipulate XML files. It includes support 
to read, modify and write XML and HTML files. There is DTDs support
this includes parsing and validation even with complex DTDs, either
at parse time or later once the document has been modified.
%prep
%setup -q

%build
# Needed for snapshot releases.
if [ ! -f configure ]; then
%ifarch alpha
  CFLAGS="$RPM_OPT_FLAGS" ./autogen.sh --host=alpha-redhat-linux --prefix=%prefix --sysconfdir="/etc" --mandir=%{_mandir}
%else
  CFLAGS="$RPM_OPT_FLAGS" ./autogen.sh --prefix=%prefix --sysconfdir="/etc" --mandir=%{_mandir}
%endif
else
%ifarch alpha
  CFLAGS="$RPM_OPT_FLAGS" ./configure --host=alpha-redhat-linux --prefix=%prefix --sysconfdir="/etc" --mandir=%{_mandir}
%else
  CFLAGS="$RPM_OPT_FLAGS" ./configure --prefix=%prefix --sysconfdir="/etc" --mandir=%{_mandir}
%endif
fi

if [ "$SMP" != "" ]; then
  (make "MAKE=make -k -j $SMP"; exit 0)
  make
else
  make
fi

%install
rm -rf $RPM_BUILD_ROOT

install -d $RPM_BUILD_ROOT%{_mandir}/man1
install -d $RPM_BUILD_ROOT%{_mandir}/man4
make prefix=$RPM_BUILD_ROOT%{prefix} mandir=$RPM_BUILD_ROOT%{_mandir} install

#
# this is a bit ugly but tries to generate the bindings for all versions
# of python installed
for i in %{prefix}/include/python*
do
    py_version=`echo $i | sed "s+%{prefix}/include/python++"`
    if test -x %{prefix}/bin/python$py_version
    then
        echo generating bindings for Python $py_version
	(cd python ; make clean ; \
         make PYTHON="%{prefix}/bin/python$py_version" \
	      PYTHON_INCLUDES="%{prefix}/include/python$py_version" \
	      PYTHON_VERSION="$py_version"; \
         make PYTHON="%{prefix}/bin/python$py_version" \
	      PYTHON_INCLUDES="%{prefix}/include/python$py_version" \
	      PYTHON_VERSION="$py_version" \
	      prefix=$RPM_BUILD_ROOT%{prefix} \
	      mandir=$RPM_BUILD_ROOT%{_mandir} install)
    fi
done
%clean
rm -rf $RPM_BUILD_ROOT

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%files
%defattr(-, root, root)

%doc AUTHORS ChangeLog NEWS README Copyright TODO
%doc %{_mandir}/man1/xmllint.1*
%doc %{_mandir}/man1/xmlcatalog.1*
%doc %{_mandir}/man3/libxml.3*

%{prefix}/lib/lib*.so.*
%{prefix}/bin/xmllint
%{prefix}/bin/xmlcatalog

%files devel
%defattr(-, root, root)

%doc %{_mandir}/man1/xml2-config.1*
%doc AUTHORS ChangeLog NEWS README Copyright TODO
%doc doc/*.html doc/html doc/*.gif doc/*.png

%{prefix}/lib/lib*.so
%{prefix}/lib/*a
%{prefix}/lib/*.sh
%{prefix}/include/*
%{prefix}/bin/xml2-config
%{prefix}/share/aclocal/libxml.m4
%{prefix}/lib/pkgconfig/libxml-2.0.pc
%files python
%defattr(-, root, root)

%doc AUTHORS ChangeLog NEWS README Copyright
%{prefix}/lib/python*/site-packages/libxml2.py
%{prefix}/lib/python*/site-packages/libxml2mod.so
%doc python/TODO
%doc python/libxml2class.txt
%doc python/tests/*.py

%changelog
* Fri Feb  1 2002 Daniel Veillard <veillard@redhat.com>

- Added the python package

* Sun Nov  4 2001 Daniel Veillard <veillard@redhat.com>

- cleaned up the specfile
- 2.4.7 broke SGML catalogs badly. this fixes it.

* Thu Apr 26 2001 Toshio Kuratomi <badger@prtr-13.ucsc.edu>

[2.3.7]
- Added libxml.m4 to the distribution file list
- Moved the man pages from /usr/man to /usr/share/man to conform to FHS2.0
- Moved programmer documentation into the devel package

* Thu Sep 23 1999 Daniel Veillard <daniel@veillard.com>

- corrected the spec file alpha stuff
- switched to version 1.7.1
- Added validation, XPath, nanohttp, removed memory leaks
- Renamed CHAR to xmlChar

* Wed Jun  2 1999 Daniel Veillard <daniel@veillard.com>

- Switched to version 1.1: SAX extensions, better entities support, lots of
  bug fixes.

* Sun Oct  4 1998 Daniel Veillard <daniel@veillard.com>

- Added xml-config to the package

* Thu Sep 24 1998 Michael Fulbright <msf@redhat.com>

- Built release 0.30

