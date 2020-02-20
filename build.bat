@echo off
tcc -impdef shell32.dll
tcc -impdef user32.dll
tcc wbrushc.c sysw32.c shell32.def user32.def
