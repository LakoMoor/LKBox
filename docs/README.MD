##  ![LakoMoor Dev](logo.png) 
**LKBox**

## Building the Engine
#### Windows

*   MinGW-w64 (https://sourceforge.net/projects/mingw-w64/)
*   Git (https://git-scm.com/)
*   CMake 3.20.5 minimum: [https://cmake.org/download/](https://cmake.org/download/)

#### Clone the repository 
Open Git
```shell
git clone https://github.com/LakoMoor/LKBox.git
```
#### Quick start engine setup
```shell
mkdir build
cd build
cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release ..
mingw32-make.exe
```
Copy DLL from folder lib.
