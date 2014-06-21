Some weird experiments happened here :-)

In this repo you can find:

1. Custom printf() suitable for embedded usage in absence of libc.
2. Written from scratch some-sort-of-optimized (however, no benchmarking was done)
   floating point-to-string conversion routines: either fast and based on precomputed
   tables, or slow with runtime computation. Both float and double are supported
   (separately).
3. GNU Make-based build system. Not completed.. But who knows, maybe it is useful in some way?
4. STM32F4 crt0-like startup code and linker script to run C program without libc.


Licensed under LGPL v2
