@echo off
if "%1"=="lua" GOTO lua else GOTO :eof
if "%1"=="cpp" GOTO cpp else GOTO :eof
if "%1"=="zip" GOTO zip else GOTO :eof
if "%1"=="all" GOTO all else GOTO :eof

:lua
lua_raw\glue.exe ./lua_raw/srlua.exe ./lua_raw/ps.lua ./dep\lua\ps.exe
GOTO :eof

:cpp
g++ *.cpp config_parser\\*.cpp translation\\*.cpp -lWs2_32 -lComdlg32 -o GNU_build/nebulator.exe -static -std=c++2b
GOTO :eof

:zip
"C:\Program Files\WinRAR\WinRAR.exe" a -afzip .\Nebulator_BETA_v1.zip -ep .\dep .\vcruntime140.dll .\lua.dll
"C:\Program Files\WinRAR\WinRAR.exe" a -afzip .\Nebulator_BETA_v1.zip -ep .\GNU_build\nebulator.exe
GOTO :eof

:all
lua_raw\glue.exe ./lua_raw/srlua.exe ./lua_raw/ps.lua ./dep\lua\ps.exe
g++ *.cpp config_parser\\*.cpp translation\\*.cpp -lWs2_32 -lComdlg32 -o GNU_build/nebulator.exe -static -std=c++20
"C:\Program Files\WinRAR\WinRAR.exe" a -afzip .\Nebulator_BETA_v1.zip .\dep .\vcruntime140.dll .\lua.dll
"C:\Program Files\WinRAR\WinRAR.exe" a -afzip .\Nebulator_BETA_v1.zip -ep .\GNU_build\nebulator.exe