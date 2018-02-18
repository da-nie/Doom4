@echo off
set __opath=%path%
set path=c:\lang\watcom\bin;c:\lang\watcom\binb
set __oinc=%include%
set include=c:\lang\watcom\h
set watcom=c:\lang\watcom\.
wcl386 /s/5r/3s/otiarnlm/l=dos4g /bt=dos %1
