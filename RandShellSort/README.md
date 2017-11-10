FAQ:


Q: Which libraries do I need?

A: You will need a C++11 compatible compiler, most likely g++4.9.

A2: You will need a SSE4.1 intrinsic-compatible compiler (again g++4.9 is your best choice)

A3: You will need a CUDA C++ compiler, available in the CUDA toolkit

A4: You will need make (or, just load the project into Netbeans, which should take care of that)

A5: For running the tests, you will need a CUDA-enabled GPU, an SSE4.1 compatible CPU, python 3.4, perf and gnuplot

Q: My code never finished building on anything but -O0

A: Use gcc-4.9, 4.8 has some nasty bug that makes compile-time exponenetial

Q: Why is do some of the sorting networks look like a template-ridden hellscape

A: A 30% speed-up

Q: When using SIMD/BitonicSort/OddEvenMergesort I get segfaults everywhere

A: Most of the algorithms, especially when using SIMD, assume N to be a power of 2. SIMD requires SSE4.1. For something that should work on everything, even with SIMD, try Randomized Shellsort

Q: When using CUDA and small input sizes, I get seqfaults/my GPU dies

A: CUDA is for n>512

Q: My compiler complains about SSE4.1

A: You are probably compiling on a system without SSE4.1 instructions, just disable SIMD, or cross-compile from a never pc

Q: With CUDA enabled, everything segfaults immediately

A: Try running the CUDA samples, you might not have the right driver. Alternatively, try changing your CUDA arch levels in the compiler setting for nvcc.
