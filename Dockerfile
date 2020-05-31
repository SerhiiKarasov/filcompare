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

CMD mkdir build && cd build && cmake .. && cmake --build . && cd tests && ctest -V