#!/bin/bash

set -e

mkdir -p ~/.config/clang

cat > ~/.config/clang/clang.cfg <<EOF
-isystem /home/linuxbrew/.linuxbrew/include
-L/home/linuxbrew/.linuxbrew/lib
-Wl,-rpath,/home/linuxbrew/.linuxbrew/lib
-Wl,--enable-new-dtags
EOF

cat > ~/.config/clang/clang++.cfg <<EOF
-nostdinc++
-isystem /home/linuxbrew/.linuxbrew/include/c++/$GCC_VERSION
-isystem /home/linuxbrew/.linuxbrew/include/c++/$GCC_VERSION/x86_64-pc-linux-gnu
-isystem /home/linuxbrew/.linuxbrew/include/c++/$GCC_VERSION/backward
-isystem /home/linuxbrew/.linuxbrew/include
-L/home/linuxbrew/.linuxbrew/lib/gcc/$GCC_VERSION
-L/home/linuxbrew/.linuxbrew/lib
-Wl,-rpath,/home/linuxbrew/.linuxbrew/lib/gcc/$GCC_VERSION
-Wl,-rpath,/home/linuxbrew/.linuxbrew/lib
-Wl,--enable-new-dtags
EOF
