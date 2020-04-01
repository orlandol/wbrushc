@echo off
tcc wbrushc.c win32\app.c win32\tablet.c win32\canvas.c win32\ui.c win32\clipbrd.c win32\imports.def -I.\
if exist wbrushc.exe .\tools\verpatch\verpatch wbrushc.exe /va 0.0.1.1 /pv 0.0.1.1 /s copyright "2020 Orlando Llanes"
