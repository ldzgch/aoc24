@echo off
cl /Zi /fsanitize=address aoc24.c && aoc24.exe %1 %2

