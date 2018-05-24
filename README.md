# AXIControllerLibrary

### Prerequisites

In order to clone the repository and build the library following requirements are needed:

* Git
* CMake >= 3.7
* GCC >= 5

### Build instructions

Clone the repository with submodules:
```
git clone https://github.com/PUTvision/AXIControllerLibrary.git --recurse-submodules
```

Compile the project like a standard _CMake_ project:
```
cd AXIControllerLibrary
mkdir build
cd build
cmake ..
make
```

### Usage example
A small program that uses _AXIControllerLibrary_ is available in the `userprog` directory. The binary is located at `build/userprog/axi_ctrl_test` after compilation.

