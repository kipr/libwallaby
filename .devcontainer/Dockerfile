FROM debian:13.6

RUN dpkg --add-architecture arm64 \
    && apt update \
    && apt install -y \
        binutils \
        build-essential \
        cmake \
        doxygen \
        fd-find \
        file \
        fish \
        fzf \
        g++-aarch64-linux-gnu \
        gcc \
        gcc-aarch64-linux-gnu \
        git \
        gnupg \
        libx11-dev:arm64 \
        make \
        libpython3.13-dev:arm64 \
        ripgrep \
        swig \
        wget \
    && rm -rf /var/lib/apt/lists/*

RUN groupadd --gid 1000 code \
    && useradd --uid 1000 --gid 1000 --create-home --shell /bin/bash code
