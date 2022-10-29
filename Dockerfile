FROM ubuntu:jammy

ARG GCC_VERSION="11"
ARG CLANG_VERSION="14"

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
	sudo \
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

# Install clang
RUN . /etc/os-release \
 && wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | sudo apt-key add - \
 && sudo echo "deb http://apt.llvm.org/${UBUNTU_CODENAME}/ llvm-toolchain-${UBUNTU_CODENAME}-${CLANG_VERSION} main" | sudo tee /etc/apt/sources.list.d/llvm.list \
 && sudo apt-get update                \
 && sudo apt-get install -y            \
  clang-${CLANG_VERSION}               \
  clang-${CLANG_VERSION}-doc           \
  clang-format-${CLANG_VERSION}        \
  clang-tidy-${CLANG_VERSION}          \
  clang-tools-${CLANG_VERSION}         \
  clangd-${CLANG_VERSION}              \
  libclang-${CLANG_VERSION}-dev        \
  libclang-common-${CLANG_VERSION}-dev \
  libclang1-${CLANG_VERSION}           \
  llvm-${CLANG_VERSION}-dev            \
  python3-clang-${CLANG_VERSION}       \
 && sudo update-alternatives --install /usr/bin/clang             clang             /usr/bin/clang-${CLANG_VERSION}             10 \
 && sudo update-alternatives --install /usr/bin/clang++           clang++           /usr/bin/clang++-${CLANG_VERSION}           10 \
 && sudo update-alternatives --install /usr/bin/clang-tidy        clang-tidy        /usr/bin/clang-tidy-${CLANG_VERSION}        10 \
 && sudo update-alternatives --install /usr/bin/clang-format      clang-format      /usr/bin/clang-format-${CLANG_VERSION}      10 \
 && sudo update-alternatives --install /usr/bin/clang-format-diff clang-format-diff /usr/bin/clang-format-diff-${CLANG_VERSION} 10 \
 && sudo update-alternatives --install /usr/bin/git-clang-format  git-clang-format  /usr/bin/git-clang-format-${CLANG_VERSION}  10 \
 && clang-tidy --version               \
 && clang-format --version

# IWYU
RUN cd /tmp \
 && git clone https://github.com/include-what-you-use/include-what-you-use.git \
 && cd include-what-you-use \
 && git switch clang_${CLANG_VERSION} \
 && cmake -G "Unix Makefiles" -DCMAKE_PREFIX_PATH=/usr/lib/llvm-${CLANG_VERSION} -S. -B/tmp/iwyu/build \
 && cmake --build /tmp/iwyu/build --parallel \
 && cmake --install /tmp/iwyu/build

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
 && cmake --build build --config Debug --parallel \
 && cd tests && \
	ctest -V && \
	cd ${PRJ_ROOT} && \
	mkdir build/graphviz && \
	cd build/graphviz && \
	cmake --graphviz=graph ${PRJ_ROOT} && \
	dot graph -T png -o graph.png