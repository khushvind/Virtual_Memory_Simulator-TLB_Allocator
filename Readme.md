# Virtual Memory Simulator - TLB Allocator

This project implements advanced memory management techniques, including TLB replacement algorithms and custom memory allocation functions in C++.

## Features

### 1. TLB Replacement Algorithm Simulator

- Implements four TLB replacement algorithms:
  - FIFO (First-In-First-Out)
  - LIFO (Last-In-First-Out)
  - LRU (Least Recently Used)
  - Optimal Algorithm
- Simulates memory access patterns and calculates TLB hits for each algorithm
- Handles multiple test cases with varying address space sizes, page sizes, and TLB sizes

### 2. Custom Memory Allocation Library

- Implements custom versions of `malloc`, `calloc`, and `free` functions
- Uses system calls like `mmap`, `brk`, and `sbrk` for low-level memory management
- Designed to be a drop-in replacement for standard memory allocation functions