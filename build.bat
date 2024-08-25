@ECHO OFF
SetLocal EnableDelayedExpansion

REM Make build dir if it doesnt exist
IF NOT EXIST .\build\NUL MKDIR .\build

REM Get a list of all the .c files and corresponding object files
SET cFilenames=
FOR /R %%f IN (*.c) DO (
    SET cFilenames=!cFilenames! %%f
)

SET assembly=dvdify
@REM add back -g flag for debugging
SET compilerFlags=-g -Wvarargs -Wall -O0
SET includeFlags=-Isrc
SET linkerFlags=-luser32
SET defines=-D_DEBUG -D_CRT_SECURE_NO_WARNINGS 

REM Link all the object files
ECHO "Building %assembly%%..."
clang %cFilenames% %compilerFlags% -o ./build/%assembly%.exe %defines% %includeFlags% %linkerFlags%

ECHO Build complete.