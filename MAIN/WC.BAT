@echo off
set __opath=%path%
set path=c:\lang\watcom10\bin;c:\lang\watcom10\binw
set __oinc=%include%
set include=c:\lang\watcom10\h
set watcom=c:\lang\watcom10\.
del *.obj
del gdrive.exe
wmake
del *.obj
rem wcl386 /s/5r/otiarnlm/l=dos4g /bt=dos %1
rem wcl386 /s/5r/otiarnlm/l=dos32a /bt=dos %1
