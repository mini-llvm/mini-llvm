FROM ubuntu:latest

ARG GCC_VERSION=15
ARG LLVM_VERSION=20

RUN apt-get update && \
    DEBIAN_FRONTEND=noninteractive apt-get upgrade -y && \
    DEBIAN_FRONTEND=noninteractive apt-get install -y build-essential curl file git procps sudo && \
    rm -rf /var/lib/apt/lists/*

RUN echo 'ubuntu ALL=(ALL) NOPASSWD:ALL' >> /etc/sudoers

USER ubuntu
WORKDIR /home/ubuntu

RUN NONINTERACTIVE=1 bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)" && \
    echo 'eval "$(/home/linuxbrew/.linuxbrew/bin/brew shellenv)"' >> ~/.bashrc

RUN /home/linuxbrew/.linuxbrew/bin/brew install binutils gcc@$GCC_VERSION llvm@$LLVM_VERSION cmake ninja && \
    echo 'export PATH="/home/linuxbrew/.linuxbrew/opt/binutils/bin:$PATH"' >> ~/.bashrc && \
    echo 'export PATH="/home/linuxbrew/.linuxbrew/opt/llvm@'$LLVM_VERSION'/bin:$PATH"' >> ~/.bashrc

COPY --chown=ubuntu:ubuntu ./configure_clang.sh /tmp/configure_clang.sh

RUN /tmp/configure_clang.sh && rm /tmp/configure_clang.sh
