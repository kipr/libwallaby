# Debian 13 (trixie) is the documented build platform: it ships the arm64
# libpython3.13 that the Wombat package depends on (see CMakeLists.txt).
FROM debian:13.6 AS toolchain

# Cross-compiler plus the arm64 libraries the default Wombat build links.
# libpython3.13-dev:arm64 rather than python3.13-dev:arm64: the latter pulls
# in the arm64 interpreter, which displaces the native python3 (see README).
RUN dpkg --add-architecture arm64 \
    && apt-get update \
    && apt-get install -y --no-install-recommends \
        binutils \
        build-essential \
        ca-certificates \
        cmake \
        doxygen \
        file \
        g++-aarch64-linux-gnu \
        gcc \
        gcc-aarch64-linux-gnu \
        git \
        libpython3.13-dev:arm64 \
        libx11-dev:arm64 \
        make \
        swig \
        wget \
    && rm -rf /var/lib/apt/lists/*


# Development container (.devcontainer/devcontainer.json selects this target).
FROM toolchain AS dev

# Dev tools, plus:
# - libpython3.13-dev, libx11-dev: native libraries for the desktop
#   (-DDUMMY=ON) build, alongside the arm64 ones above
# - nodejs, npm: to install the Codex CLI
# - bubblewrap: the agent CLIs' sandbox
RUN apt-get update \
    && apt-get install -y --no-install-recommends \
        bubblewrap \
        curl \
        fd-find \
        fish \
        fzf \
        jq \
        libpython3.13-dev \
        libx11-dev \
        nodejs \
        npm \
        openssh-client \
        ripgrep \
    && ln -s "$(command -v fdfind)" /usr/local/bin/fd \
    && rm -rf /var/lib/apt/lists/*

RUN groupadd --gid 1000 code \
    && useradd --uid 1000 --gid 1000 --create-home --shell /usr/bin/fish code

USER code
# Agent state lives in named volumes; a new volume inherits this ownership.
# CLAUDE_CONFIG_DIR also moves ~/.claude.json inside the volume.
# NPM_CONFIG_PREFIX puts global npm installs (Codex) in ~/.local/bin.
ENV CLAUDE_CONFIG_DIR=/home/code/.claude \
    CODEX_HOME=/home/code/.codex \
    NPM_CONFIG_PREFIX=/home/code/.local \
    PATH=/home/code/.local/bin:$PATH
RUN mkdir -p "$CLAUDE_CONFIG_DIR" "$CODEX_HOME" \
    && curl -fsSL https://claude.ai/install.sh | bash \
    && claude --version \
    && npm install -g @openai/codex \
    && codex --version


# Release builder (README "Wombat"). Kept last so `docker build .` builds it.
FROM toolchain AS builder

RUN groupadd --gid 1000 kipr \
    && useradd --uid 1000 --gid 1000 kipr

USER kipr:kipr
