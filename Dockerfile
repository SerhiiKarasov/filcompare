FROM ubuntu:jammy

ARG GCC_VERSION="11"

RUN apt-get update && \
	apt-get install -y \
	apt-transport-https \
	build-essential \
	git autoconf \
	libtool pkg-config \
	libsqlite3-dev \
	zlib1g-dev \
	graphviz \
	libacl1-dev \
	libcap-dev \
	cppcheck  \
	python3-pip \
	lsb-release \
	software-properties-common \
	wget \
	gcc-${GCC_VERSION} \
	g++-${GCC_VERSION} \
    clang \
#    clang-doc \
    clang-format \
    clang-tidy \
    clang-tools \
    clangd \
    libc++-dev \
    libc++abi-dev \
    libclang-dev \
#    libclang-common-dev \
    libclang1 \
#    liblldb \
    python3-clang \
    python3-lldb \
 && update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-${GCC_VERSION} 10 \
 && update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-${GCC_VERSION} 10 \
 && pip3 install cpplint && \
	pip3 install cmake_format && \
	pip3 install pyyaml \
&& mkdir /opt/project

# Install cmake v3.25 for SYSTEM property
# https://cmake.org/cmake/help/latest/prop_dir/SYSTEM.html
RUN wget -O /tmp/cmake.sh https://github.com/Kitware/CMake/releases/download/v3.25.0-rc2/cmake-3.25.0-rc2-linux-x86_64.sh \
 && bash /tmp/cmake.sh --skip-license --prefix=/usr --exclude-subdir \
 && rm /tmp/cmake.sh
WORKDIR /opt/project

CMD echo ${PWD} && \
    PRJ_ROOT=${PWD} && \
    cmake --version && \
    gcc --version && \
    g++ --version && \
	clang --version && \
	echo ${CXX} && \
	echo ${CC}     \
 && rm -rf build && mkdir -vp build               \
 && cmake                                         \
      -DCMAKE_BUILD_TYPE=Debug                    \
      -DWARNINGS_AS_ERRORS=OFF                    \
      -DCMAKE_VERBOSE_MAKEFILE=TRUE               \
      -DENABLE_SANITIZER_ADDRESS=TRUE             \
      -DENABLE_COVERAGE=ON                        \
      -DCMAKE_EXPORT_COMPILE_COMMANDS=ON          \
      -S.                                         \
      -Bbuild                                     \
 && cmake                                         \
      --build build                               \
      --config Debug                              \
      --parallel                                  \
 && cd tests && \
	ctest -V && \
	cd ${PRJ_ROOT} && \
	mkdir build/graphviz && \
	cd build/graphviz && \
	cmake --graphviz=graph ${PRJ_ROOT} && \
	dot graph -T png -o graph.png