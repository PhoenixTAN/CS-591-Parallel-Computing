# 环境配置
https://code.visualstudio.com/docs/cpp/config-mingw#_prerequisites


1. Install the C/C++ extension for VS Code.

2. Install Mingw-w64 via the SourceForge website. Click Mingw-w64 to download the Windows Mingw-w64 installer.

    https://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win32/Personal%20Builds/mingw-builds/installer/mingw-w64-install.exe/download

    1. Run the installer.
    2. For Architecture select x86_64 and then select Next.
    3. Next again to use the default installation folder and install MinGW.



3. Add the path to your Mingw-w64 bin folder to the Windows PATH environment variable. Add this to the path: 
```
C:\Program Files\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin.
```
Select OK to save the updated PATH. You will need to reopen any console windows for the new PATH location to be available.

4. Check your MinGW installation
```
PS C:\Users\tanzi> g++ --version
g++.exe (x86_64-posix-seh-rev0, Built by MinGW-W64 project) 8.1.0
Copyright (C) 2018 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

PS C:\Users\tanzi> gdb --version
GNU gdb (GDB) 8.1
Copyright (C) 2018 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.  Type "show copying"
and "show warranty" for details.
This GDB was configured as "x86_64-w64-mingw32".
Type "show configuration" for configuration details.
For bug reporting instructions, please see:
<http://www.gnu.org/software/gdb/bugs/>.
Find the GDB manual and other documentation resources online at:
<http://www.gnu.org/software/gdb/documentation/>.
For help, type "help".
Type "apropos word" to search for commands related to "word".
```