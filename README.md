# Nebulator
Automatically translates startup-config from standalone Zyxel firewalls and sends it to nebula.
Program uses Lua to run powershell scripts, since running powershell scripts in C++ are a bit clunky...
Probably should have written the program in like C# or python, but no... I like C++

## Dependencies
### A JSON parser from nlohmann 
https://github.com/nlohmann/json
### Lua to exe converter, srLua
https://github.com/LuaDist/srlua
### Lua
https://github.com/lua/lua
