# Forge

A C++20 inference engine for GPT-2, built from scratch to explore transformer architecture, numerical computing, and low-level inference optimization.

## Goals

- Implement GPT-2 inference without ML inference frameworks.
- Validate numerical correctness against a PyTorch reference.
- Implement incremental generation with KV caching.
- Profile and optimize inference performance.

## Project Status

**Stage 0 complete** — Implemented a minimal FP32 tensor container with checked indexing, copy/move semantics, and CMake/CTest integration.

See [STATUS.md](STATUS.md) for progress and [ROADMAP.md](ROADMAP.md) for planned milestones.

## Build and Test

Requires CMake 3.20+ and a C++20-compatible compiler.

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

## Tech Stack

C++20 · CMake · Python · NumPy · PyTorch

## Development Approach

Correctness first, followed by profiling and evidence-based optimization. Core inference operations are implemented manually to develop a deeper understanding of the underlying systems.
