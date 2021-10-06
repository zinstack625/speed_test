# syntax=docker/dockerfile:1.2

FROM ubuntu:focal
LABEL description="Universal Docker image to launch testing frameworks embedded within laboratory works at BMSTU"
LABEL version="0.1"
ENV TZ=Europe/Moscow
RUN apt update &&
    apt install build-essential clang gcc cmake build-essential python3 python3-pip gcovr lcov libboost-all-dev doxygen -y && \
    pip3 install cpplint && \
    apt clean && rm -rf /var/lib/apt/lists/*
	
