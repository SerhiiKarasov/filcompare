FROM ubuntu:bionic

RUN apt-get update && \
	apt-get install -y \
	build-essential \
	git cmake autoconf \
	libtool pkg-config \
	libsqlite3-dev \
	zlib1g-dev \
	graphviz \
	libacl1-dev \
	libcap-dev \
	cppcheck  \
	python3-pip \
	clang-tidy && \
	pip3 install cpplint && \
	pip3 install cmake_format && \
	pip3 install pyyaml

WORKDIR /opt/project

CMD PRJ_ROOT=${PWD} && \
	mkdir build && cd build && \
	cmake .. && \
	cmake -D CMAKE_EXPORT_COMPILE_COMMANDS=ON . && \
	cmake --build . -- -j2 && \
	cd tests && \
	ctest -V && \ 
	cd ${PRJ_ROOT} && \
	mkdir build/graphviz && \
	cd build/graphviz && \
	cmake --graphviz=graph ${PRJ_ROOT} && \
	dot graph -T png -o graph.png
