FROM ubuntu:bionic

RUN apt-get update && \
	apt-get install -y \
	build-essential \
	git cmake autoconf \
	libtool pkg-config \
	libboost-all-dev \
	libboost-filesystem-dev \
	libboost-system-dev \
	libsqlite3-dev \
	zlib1g-dev \
    libsqlite3-dev \
    graphviz \
    libacl1-dev \
    libcap-dev 

WORKDIR /opt/project

CMD mkdir build && cd build && cmake .. && cmake --build . && cd tests && ctest -V