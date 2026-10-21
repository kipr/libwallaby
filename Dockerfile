FROM debian:13.6

RUN dpkg --add-architecture arm64 \
    && apt update \
    && apt install -y \
        binutils \
        build-essential \
        cmake \
        doxygen \
        g++-aarch64-linux-gnu \
        gcc \
        gcc-aarch64-linux-gnu \
        git \
        libx11-dev:arm64 \
        make \
        python3.13-dev:arm64 \
        swig \
        wget \
    && rm -rf /var/lib/apt/lists/*

RUN groupadd --gid 1000 kipr \
    && useradd --uid 1000 --gid 1000 kipr

USER kipr:kipr
