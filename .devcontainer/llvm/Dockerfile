FROM ubuntu:latest

ARG GCC_VERSION=15
ARG LLVM_VERSION=21

RUN set -ex; \
    apt-get update; \
    DEBIAN_FRONTEND=noninteractive apt-get upgrade -y; \
    DEBIAN_FRONTEND=noninteractive apt-get install -y build-essential curl file git procps sudo; \
    rm -rf /var/lib/apt/lists/*

RUN echo 'ubuntu ALL=(ALL) NOPASSWD:ALL' >> /etc/sudoers

USER ubuntu
WORKDIR /home/ubuntu

RUN set -ex; \
    NONINTERACTIVE=1 bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"; \
    echo 'eval "$(/home/linuxbrew/.linuxbrew/bin/brew shellenv)"' >> ~/.bashrc

RUN set -ex; \
    eval "$(/home/linuxbrew/.linuxbrew/bin/brew shellenv)"; \
    brew install binutils gcc@$GCC_VERSION; \
    brew link -f binutils; \
    for f in /home/linuxbrew/.linuxbrew/opt/gcc@$GCC_VERSION/bin/*-$GCC_VERSION; do \
        name="$(basename "$f")"; \
        ln -sf "$f" "/home/linuxbrew/.linuxbrew/bin/${name%-$GCC_VERSION}"; \
    done

RUN set -ex; \
    eval "$(/home/linuxbrew/.linuxbrew/bin/brew shellenv)"; \
    brew install llvm@$LLVM_VERSION; \
    brew link -f llvm@$LLVM_VERSION

COPY ./configure_clang.sh ./configure_clang.sh

RUN ./configure_clang.sh && rm ./configure_clang.sh

RUN set -ex; \
    eval "$(/home/linuxbrew/.linuxbrew/bin/brew shellenv)"; \
    brew install cmake ninja
