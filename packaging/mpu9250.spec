Name:           mpu9250
Version:        1.0.0
Release:        0
License:        Apache-2.0
Summary:        MPU9250 IMU sample code
Group:          Application Framework/Libraries
Source0:        %{name}-%{version}.tar.gz
BuildRequires:  cmake
BuildRequires:  libi2c-devel


%description
MPU9250 IMU sample code package


%prep
%setup -q

%build
CFLAGS="$CFLAGS -fpic"


%cmake . -DLIBDIR=%{_libdir}

%__make %{?_smp_mflags}

%install
%make_install

%post
ldconfig

%postun
ldconfig

%files
%{_includedir}/*
%{_libdir}/*.so
%{_bindir}/*
%{_datadir}/mpu9250/*
%{_sysconfdir}/init.d/mpu9250.sh
%attr(0775,root,root) %{_bindir}/mpu9250_sample
%attr(0775,root,root) %{_sysconfdir}/init.d/mpu9250.sh
