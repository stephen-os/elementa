@echo off

pushd ..
Lumina\Dependencies\premake\premake5.exe --file=premake5.lua vs2022
popd
pause