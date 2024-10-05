# Build steps

## Requirements

- Cmake version 3.13+
- libnotcurses version 3+
- gcc version 13+
- WSL with Ubuntu 22.04 at least

## WSL installation
More information can be found here \
https://learn.microsoft.com/en-us/windows/wsl/install \
https://documentation.ubuntu.com/wsl/en/latest/guides/install-ubuntu-wsl2/ \
It is recommended to update Ubuntu to the latest LTS release 


## Steps
1. Install libnotcurses-dev and build tools on your WSL install if you haven't already
```shell
sudo apt-get install libnotcurses-dev build-essential -y
```
2. Build, run commands in project directory
```shell
mkdir build
cd build
cmake ..
cmake --build .
```

## Cross compiling for ARM64
1. Make sure you have the sources for arm64, file `/etc/apt/sources.list.d/xarch-ports.sources`

```
Types: deb
Architectures: arm64
URIs: http://ports.ubuntu.com/ubuntu-ports
Suites: <codename> <codename>-updates <codename>-security <codename>-backports
Components: main restricted universe multiverse
Signed-By: /usr/share/keyrings/ubuntu-archive-keyring.gpg
```
Replace \<codename> with your distro's codename. Check using `lsb_release -a`
```shell
user@WORKSTATION:~$ lsb_release -a
No LSB modules are available.
Distributor ID: Ubuntu
Description:    Ubuntu 24.04.1 LTS
Release:        24.04
Codename:       noble
```

2. Install the necessary packages for cross compilation
```shell
sudo apt-get install crossbuild-essential-arm64 libnotcurses-dev:arm64 build-essential -y
```
2. Build, run commands in project directory
```shell
mkdir build
cd build
cmake -DCMAKE_TOOLCHAIN_FILE=arm64-gnu-toolchain.cmake ..
cmake --build .
```