@echo off
if "%1"=="lua" GOTO lua else GOTO :eof
if "%1"=="cpp" GOTO cpp else GOTO :eof
if "%1"=="zip" GOTO zip else GOTO :eof
if "%1"=="all" GOTO all else GOTO :eof

:lua
lua_raw\glue.exe ./lua_raw/srlua.exe ./lua_raw/ps.lua ./dep\lua\ps.exe
GOTO :eof

:cpp
g++ *.cpp config_parser\\*.cpp translation\\*.cpp -lWs2_32 -lComdlg32 -o nebulator.exe -static
GOTO :eof

:zip
"C:\Program Files\WinRAR\WinRAR.exe" a -afzip .\Nebulator_BETA_v1.zip .\nebulator.exe .\dep .\vcruntime140.dll .\lua.dll
GOTO :eof

:all
lua_raw\glue.exe ./lua_raw/srlua.exe ./lua_raw/ps.lua ./dep\lua\ps.exe
g++ *.cpp config_parser\\*.cpp translation\\*.cpp -lWs2_32 -lComdlg32 -o nebulator.exe -static
"C:\Program Files\WinRAR\WinRAR.exe" a -afzip .\Nebulator_BETA_v1.zip .\nebulator.exe .\dep .\vcruntime140.dll .\lua.dll