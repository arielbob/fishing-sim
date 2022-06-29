@echo off

set CommonCompilerFlags=-MD -nologo -Gm- -GR- -EHa- -Oi -W4 -wd4201 -wd4100 -wd4189 -wd4127 -FC -Z7 -EHsc
set CommonLinkerFlags=-incremental:no -opt:ref user32.lib gdi32.lib Shell32.lib glu32.lib opengl32.lib ..\src\lib\glew32.lib ..\src\lib\glfw3.lib ..\src\lib\glfw3dll.lib

IF NOT EXIST ..\build mkdir ..\build
pushd ..\build
cl %CommonCompilerFlags% ..\src\main.cpp -Fmmain.map /link %CommonLinkerFlags%
popd
