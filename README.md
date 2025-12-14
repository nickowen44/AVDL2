# AVDL2
Formula Autonomous Datalogger V2

## Building

Use QtCreator, open the `.pro` project file and go.

### VSCode

If you want to use VSCode, follow these steps

- Install [MSYS](https://www.msys2.org/)
- Setup the environment variables to point to QT, and MSYS:
  - `Qt6_ROOT=C:\Qt\6.10.1\mingw_64` (or similar)
  - `MSYS_ROOT=C:\msys64` (or similar)
- Launch a Mingw64 MSYS terminal, either from the start menu, or via the custom `QtBash` VSCode terminal profile
- Install the required packages: `pacman -S mingw-w64-x86_64-toolchain mingw-w64-x86_64-cmake mingw-w64-x86_64-ninja git`
- Configure cmake: `cmake -B build . -DCMAKE_BUILD_TYPE=Debug`
- Build cmake: `cmake --build build`
- Ensure the required DLL's are copied over: `$Qt6_ROOT/bin/windeployqt.exe build/YourAppName.exe`
