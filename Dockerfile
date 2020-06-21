FROM ubuntu:bionic

RUN apt-get update && \
	apt-get install -y \
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
	wget \
	clang-tidy && \
	pip3 install cpplint && \
	pip3 install cmake_format && \
	pip3 install pyyaml && \
	wget https://cmake.org/files/v3.17/cmake-3.17.3-Linux-x86_64.sh && \
	mv cmake-3.17.3-Linux-x86_64.sh /tmp/ && \
	chmod +x /tmp/cmake-3.17.3-Linux-x86_64.sh && \
	mkdir /usr/bin/cmake-3.17.3 && \
	bash /tmp/cmake-3.17.3-Linux-x86_64.sh --skip-license --prefix=/usr/bin/cmake-3.17.3 && \
	ln -s /usr/bin/cmake-3.17.3/bin/* /usr/local/bin

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
	dot graph -T png -o graph.png
