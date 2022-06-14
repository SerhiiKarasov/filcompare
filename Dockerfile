FROM ubuntu:bionic

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
 && pip3 install cpplint && \
	pip3 install cmake_format && \
	pip3 install pyyaml && \
	wget https://cmake.org/files/v3.17/cmake-3.17.3-Linux-x86_64.sh && \
	mv cmake-3.17.3-Linux-x86_64.sh /tmp/ && \
	chmod +x /tmp/cmake-3.17.3-Linux-x86_64.sh && \
	mkdir /usr/bin/cmake-3.17.3 && \
	bash /tmp/cmake-3.17.3-Linux-x86_64.sh --skip-license --prefix=/usr/bin/cmake-3.17.3 && \
	ln -s /usr/bin/cmake-3.17.3/bin/* /usr/local/bin

RUN wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | apt-key add -                                        \
 && add-apt-repository "deb http://apt.llvm.org/$(lsb_release -cs)/ llvm-toolchain-$(lsb_release -cs)-12 main"  \
 && apt-get -o Acquire::https::No-Cache=True -o Acquire::http::No-Cache=True update                             \
 && apt-get install -y                                                                                          \
    clang-12                                                                                                    \
    clang-12-doc                                                                                                \
    clang-format-12                                                                                             \
    clang-tidy-12                                                                                               \
    clang-tools-12                                                                                              \
    clangd-12                                                                                                   \
    libc++-12-dev                                                                                               \
    libc++abi-12-dev                                                                                            \
    libclang-12-dev                                                                                             \
    libclang-common-12-dev                                                                                      \
    libclang1-12                                                                                                \
    liblldb-12                                                                                                  \
    python3-clang-12                                                                                            \
    python3-lldb-12                                                                                             \
	gcovr																										\
 && update-alternatives --install /usr/bin/clang clang /usr/bin/clang-12 100                                    \
 && update-alternatives --install /usr/bin/clang++ clang++ /usr/bin/clang++-12 100                              \
 && update-alternatives --install /usr/bin/clang-tidy clang-tidy /usr/bin/clang-tidy-12 100                     \
 && update-alternatives --install /usr/bin/run-clang-tidy run-clang-tidy /usr/bin/run-clang-tidy-12 100         \
 && update-alternatives --install /usr/bin/clang-format clang-format /usr/bin/clang-format-12 100

WORKDIR /opt/project

CMD PRJ_ROOT=${PWD} && \
	mkdir build && cd build && \
	cmake .. && \
	cmake -D ENABLE_TEST_COVERAGE=1 -DCMAKE_EXPORT_COMPILE_COMMANDS=ON  -DCMAKE_VERBOSE_MAKEFILE=TRUE -DENABLE_SANITIZER_ADDRESS=TRUE . && \
	cmake --build . -- -j2 && \
	cd tests && \
	ctest -V && \
	cd ${PRJ_ROOT} && \
	mkdir build/graphviz && \
	cd build/graphviz && \
	cmake --graphviz=graph ${PRJ_ROOT} && \
	dot graph -T png -o graph.png && \
	cd ../tests  && \
    ctest -C && \
    pwd && \
    ls -la ${PWD} && \
    which gcov && \
    gcov --version && \
    gcovr --version && \
    gcovr --root ../ .