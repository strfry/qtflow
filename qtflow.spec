%build
%cmake

%make_jobs

%install
%cmake_install

%check
%ctest

