Name:        libslax
Version:     0.16.16
Release:     1%{?dist}
Summary:     The SLAX language (XSLT alternative)

Group:       Development/Libraries
License:     MIT
URL:         https://github.com/Juniper/libslax
Source0:     https://github.com/Juniper/libslax/releases/0.16.16/libslax-0.16.16.tar.gz

BuildRequires:  libxml2-devel
BuildRequires:  libxslt-devel
BuildRequires:  curl-devel
BuildRequires:  libedit-devel
BuildRequires:  automake
BuildRequires:  autoconf
BuildRequires:  libtool

%description
Welcome to libslax, an implementation of the SLAX language, an alternate
syntax for XSLT that follow the patterns of C and Perl. This library
builds on the most excellent work of Daniel Veillard et al in the
libxml2 and libxslt libraries, for which I heartily give thanks.

#%package     devel
#Summary:     Development files for %{name}
#Group:       Development/Libraries
#Requires:    %{name} = %{version}-%{release}

#%description    devel
#The %{name}-devel package contains libraries and header files for
#developing applications that use %{name}.

%prep
%setup -q
#sh ./bin/setup.sh

%build
%configure
make %{?_smp_mflags}

%install
rm -rf $RPM_BUILD_ROOT
%make_install
#find $RPM_BUILD_ROOT -name '*.la' -exec rm -f {} ';'

%clean
rm -rf $RPM_BUILD_ROOT

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig


%files
%defattr(-,root,root,-)
%doc

#%files devel
#%defattr(-,root,root,-)
#%doc
#%{_includedir}/*
#%{_libdir}/*.so

%changelog
For the complete changelog, please see:
  https://github.com/Juniper/libslax/releases/

Thanks goes to John Deatherage <john@routelastresort.com> for working this up.
