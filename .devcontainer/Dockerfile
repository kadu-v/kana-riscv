################################################################################
# SPDX-License-Identifier: MIT
# SPDX-FileType: SOURCE
# SPDX-FileCopyrightText: (c) 2022, Marcus Andrade
################################################################################

FROM ubuntu:22.04

ENV DEBIAN_FRONTEND noninteractive
ENV LANG C.UTF-8
ENV TZ Asia/Tokyo
ENV NVIDIA_DRIVER_CAPABILITIES graphics

# Add i386 to enable the installation of multiarch binaries (x86-32)
RUN apt-get update && \
    apt-get install -y --no-install-recommends \
    xserver-xorg \
    x11-apps \
    python3-pip \
    wget \
    locales \
    patch \
    git \
    make \
    libtcmalloc-minimal4 \
    cmake

# Clean Up
RUN	apt-get autoremove -y \
    && apt-get clean -y \
    && rm -rf /var/lib/apt/lists/* 

# Install verilator
RUN apt-get update \
    && apt-get install -y verilator \
    perl-doc

# g++-13
RUN apt-get update \
    && apt-get install -y software-properties-common \
    && add-apt-repository ppa:ubuntu-toolchain-r/test
RUN apt-get update \
    && apt-get install -y gcc-13 g++-13 \
    && rm -rf /usr/bin/gcc \
    && rm -rf /usr/bin/g++ \
    && ln -s /usr/bin/gcc-13 /usr/bin/gcc \
    && ln -s /usr/bin/g++-13 /usr/bin/g++

# Install rsicv-gnutoolchain
ENV RISCV /opt/riscv64
ENV PATH $RISCV/bin:$PATH
RUN apt-get install -y autoconf automake autotools-dev curl python3 libmpc-dev \
    libmpfr-dev libgmp-dev gawk build-essential bison flex texinfo gperf libtool \
    patchutils bc zlib1g-dev libexpat-dev \
    && git clone --recursive https://github.com/riscv/riscv-gnu-toolchain \
    && git clone --recursive https://github.com/riscv/riscv-opcodes.git \
    && cd riscv-gnu-toolchain \
    && ./configure --prefix=${RISCV} --enable-multilib \
    && make

# Install verible
RUN wget https://github.com/chipsalliance/verible/releases/download/v0.0-3483-ga4d61b11/verible-v0.0-3483-ga4d61b11-linux-static-arm64.tar.gz \
    && tar -zxvf verible-v0.0-3483-ga4d61b11-linux-static-arm64.tar.gz \
    && cp -r ./verible-v0.0-3483-ga4d61b11/bin/* /usr/local/bin/

# Install googletest
RUN git clone https://github.com/google/googletest.git \
    && mkdir -p ./googletest/build \
    && cd /googletest/build \
    && cmake .. \
    && make -j8 \
    && make install