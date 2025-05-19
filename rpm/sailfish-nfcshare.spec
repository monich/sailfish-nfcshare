Name:       sailfish-nfcshare
Version:    1.0.0
Release:    1
License:    BSD
Summary:    NFC share plugin
URL:        https://github.com/sailfishos/sailfish-nfcshare
Source0:    %{name}-%{version}.tar.gz

Requires: sailfishshare-components
Requires: nfcd >= 1.2

BuildRequires:  pkgconfig(Qt5Core)
BuildRequires:  pkgconfig(Qt5DBus)
BuildRequires:  pkgconfig(Qt5Qml)
BuildRequires:  pkgconfig(Qt5Quick)
BuildRequires:  pkgconfig(libnfcdef)
BuildRequires:  pkgconfig(nemotransferengine-qt5) >= 2

%define nfcshare_uidir /usr/share/nemo-transferengine/plugins/nfcshare
%define nfcshare_qmlplugindir %{_libdir}/qt5/qml/org/sailfishos/nfcshare
%define transferengine_plugindir %{_libdir}/nemo-transferengine/plugins

%description
%{summary}.

%prep
%setup -q -n %{name}-%{version}

%build
%qmake5
%make_build

%install
%qmake5_install

%files
%license LICENSE
%{transferengine_plugindir}/sharing/libnfcshareplugin.so
%{nfcshare_qmlplugindir}
%{nfcshare_uidir}
