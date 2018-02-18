@echo off
set __opath=%path%
set path=e:\lang\watcom\bin;e:\lang\watcom\binb
set __oinc=%include%
set include=e:\lang\watcom\h
set watcom=e:\lang\watcom\.
wcl386 /s/5r/3s/otiarnlm/l=dos4g /bt=dos %1
