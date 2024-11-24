# Build steps

## Requirements

- Cmake version 3.13+
- libraylib version 5.0.x+
- uthash version 2+
- gcc version 13+
- WSL with Ubuntu 22.04+
- MSYS2 version 3.5.4+

## WSL installation
More information can be found here \
https://learn.microsoft.com/en-us/windows/wsl/install \
https://documentation.ubuntu.com/wsl/en/latest/guides/install-ubuntu-wsl2/ \
It is recommended to update Ubuntu to the latest LTS release 

## Compiling for Windows
### Installing raylib and uthash
1. Open MSYS2 UCRT64 shell and run the following commands inside it
2. Run the command
```shell
pacman -S ucrt64/mingw-w64-ucrt-x86_64-raylib
pacman -S mingw-w64-ucrt-x86_64-uthash
```
3. Ensure include and lib directory is in your PATH
![img.png](screenshots/build_guide.png)
4. Build, run commands in powershell
```pwsh
mkdir build
cd build
cmake ..
cmake --build ./
```

## Compiling for Linux on WSL
1. Install libraylib5-dev and uthash-dev on your WSL install
```shell
sudo add-apt-repository ppa:texus/raylib
sudo apt-get install libraylib5-dev uthash-dev -y
```
2. Build, run commands in project directory
```shell
mkdir build
cd build
cmake ..
cmake --build .
```

## Cross compiling for ARM64
1. Make sure you have the sources for arm64, create file `/etc/apt/sources.list.d/xarch-ports.sources` in your wsl environment
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
sudo dpkg --add-architecture arm64
sudo add-apt-repository ppa:texus/raylib
sudo apt-get install crossbuild-essential-arm64 libraylib5-dev:arm64 uthash-dev:arm64 build-essential -y
```
3. Build, run commands in project directory
```shell
mkdir build
cd build
cmake -DCMAKE_TOOLCHAIN_FILE=arm64-gnu-toolchain.cmake ..
cmake --build .
```

## Compiling for raspberry pi
1. Download raylib release v5.0x \
https://github.com/raysan5/raylib/releases \
2. Follow the instructions found here to compile raylib and install it \
https://github.com/raysan5/raylib/wiki/Working-on-Raspberry-Pi
3. Install the necessary packages to build the app \ 
```shell
sudo apt-get install cmake build-essential uthash-dev
```
4. Run the following commands to build the project in the project directory
```shell
mkdir build
cd build
cmake ..
cmake --build . -j4
```
The binary can will be found in <project_folder>/build