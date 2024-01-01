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
    libtcmalloc-minimal4

# Clean Up
RUN	apt-get autoremove -y \
    && apt-get clean -y \
    && rm -rf /var/lib/apt/lists/* 

# Install verilator
RUN apt-get update \
    && apt-get install -y verilator \
    perl-doc

# g++-13
RUN apt-get upate \
    && apt-get install -y software-properties-common \
    && add-apt-repository ppa:ubuntu-toolchain-r/test
RUN apt-get update \
    && apt-get install -y install gcc-13 g++-13 \
    && ln -s /usr/bin/gcc-13 /usr/bin/gcc \
    && ln -s /usr/bin/g++-13 /usr/bin/g++