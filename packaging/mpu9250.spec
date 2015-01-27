Name:           mpu9250
Version:        1.0.0
Release:        0
License:        GPL
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


%cmake .  

%__make %{?_smp_mflags}

%install
%make_install

%post
ldconfig

%postun
ldconfig

%files
#%{_includedir}/*
#%{_libdir}/*.so
#%{_bindir}/*
#%{_datadir}/mpu9250/*
