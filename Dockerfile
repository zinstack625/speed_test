# syntax=docker/dockerfile:1.2

FROM ubuntu:hirsute
LABEL description="Universal Docker image to launch testing frameworks embedded within laboratory works at BMSTU"
LABEL version="0.1"
RUN ln -s /usr/share/zoneinfo/Europe/Moscow /etc/localtime && \
    apt update && \
    apt install clang g++ cmake libgtest-dev libgmock-dev \
    doxygen \
    python3 python3-pip gcovr lcov -y && \
    pip3 install cpplint && \
    apt clean && rm -rf /var/lib/apt/lists/*
	
