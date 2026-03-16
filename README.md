# octrees-benchmark-suite

![Build Status](https://img.shields.io/badge/build-passing-brightgreen) ![License](https://img.shields.io/badge/license-Custom-blue) ![Language](https://img.shields.io/badge/language-C%2B%2B20-blue) ![Platform](https://img.shields.io/badge/platform-Linux%20%7C%20macOS-lightgrey)

A comprehensive **benchmarking suite** for octree-based spatial data structures, designed to evaluate performance across multiple algorithms, encoding schemes, and hardware configurations.

## 📖 Overview

This repository is a standalone **benchmarking client** for the linoctree library. It provides:

- **Neighbor search benchmarks** - k-NN and range search performance evaluation
- **Encoding & build benchmarks** - Spatial Function Code (SFC) encoding performance analysis
- **Locality analysis** - Cache behavior and index-distance histograms
- **Memory profiling** - Theoretical and empirical memory footprint tracking
- **Comprehensive metrics** - PAPI hardware counters, cache behavior, thread scalability

### Key Features

✅ **Multiple Data Structures** - LinearOctree, PointerOctree, Nanoflann KD-Tree, Picotree KD-Tree, UNIBN Octree, PCL octrees (optional)  
✅ **Spatial Encodings** - Hilbert curves, Morton codes, Z-order curves, and more  
✅ **Hardware Profiling** - PAPI integration for cache/branch/TLB metrics  
✅ **OpenMP Parallelization** - Thread scalability analysis with dynamic scheduling  
✅ **CSV Output** - Machine-readable results for post-processing and visualization  
✅ **CLI-Driven** - Flexible command-line interface for reproducible benchmarks

---

## 🚀 Quick Start

### Prerequisites

- **C++20** compiler (GCC 10+, Clang 11+, MSVC 19.29+)
- **CMake** 3.14+
- **OpenMP** 4.0+
- **Eigen3** 3.3+
- **linoctree library**

### Installation

1. Dependency installation

```bash
bash scripts/install_all.sh
````

2. Build the benchmarking suite

```bash
cmake -B build -S . -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel $(nproc)
```

### First Benchmark

```bash
# Run neighbor search benchmarks on sample data
./build/octrees-benchmark-suite \
  -i /path/to/data.las \
  -r "0.5,1.0,2.0" \
  -s 5000 \
  --repeats 5 \
  -o results/
```

---

## 🎯 Usage Examples

### Neighbor Search Benchmarks (Default)

```bash
# Benchmark k-NN search with multiple parameters
./build-suite/octrees-benchmark-suite \
  -i /path/to/data.las \
  -e "mort,hilb" \
  -k "LinearOctree,PointerOctree,NanoflannKD" \
  -r "1.0,2.0,5.0" \
  --num-queries 5000 \
  --repeats 10 \
  --num-threads 1,4,8 \
  -o results/neighbors.csv

# Output: CSV with columns [algorithm, encoder, radius, queries, throughput, cache-misses, ...]
```

### Build & Encoding Benchmarks

```bash
# Measure build time and encoding performance
./build-suite/octrees-benchmark-suite \
  -i data/alcoy.las \
  --build-enc \
  -e "hilb,mort" \
  -k "LinearOctree,PointerOctree" \
  -o results/

# Output: CSVs for build times, encoding times, and sorting overhead
```

### Locality Analysis

```bash
# Analyze cache behavior and index-distance relationships
./build-suite/octrees-benchmark-suite \
  -i data/alcoy.las \
  --locality \
  -e "hilb" \
  -k "LinearOctree" \
  --num-queries 5000 \
  -o results/

# Output: Histogram CSV of index-distance relationships
```

### Memory Profiling

```bash
# Profile memory usage with heap profiler
./build-suite/octrees-benchmark-suite \
  -i data/alcoy.las \
  --memory \
  -k "LinearOctree" \
  -e "hilb"

# Output: Theoretical memory breakdown by component
# (Use with valgrind/heaptrack for empirical measurement)
```

### CLI Help

```bash
./build-suite/octrees-benchmark-suite -h
```

---

## 📋 Configuration & Conventions

### Deterministic Testing

- **RNG Seed**: Fixed at `42` for reproducible query sets
- **Query Precomputation**: Per-repeat index sets computed upfront
- **Warmup Strategy**: First run discarded; PAPI counters on warm cache

### OpenMP Settings

- **Default Schedule**: Dynamic (work-stealing for load balancing)
- **Thread Binding**: Set via `--num-threads` flag
- **Per-Repeat Thread Count**: Iterated for scalability analysis

### Algorithm/Encoder Tokens

CLI tokens must match installed headers:

- **Algorithms**: `LinearOctree`, `PointerOctree`, `NanoflannKD`, `UnibnOctree`, `PCLKDTree`, `PCLOctree`
- **Encodings**: `hilb` (Hilbert), `mort` (Morton), `z_order`, `sphere`, `none` (no encoding)
- **Query Types**: `range`, `knn` (for k-NN search)
---

## 🔧 Development & Architecture

### Key Design Principles

1. **Separation of Concerns**: Benchmarking orchestration lives here; core algorithms from linoctree library
2. **Pluggable Comparators**: Optional PCL/PicoTree support via compile guards
3. **Hardware-Aware**: PAPI integration for cache/branch/TLB profiling
4. **Reproducibility**: Deterministic queries and fixed seeds

---

## 📦 Dependencies

### Required

| Package | Minimum | Purpose |
|---------|---------|---------|
| **C++ Standard** | C++17 | Core language |
| **CMake** | 3.14 | Build configuration |
| **OpenMP** | 4.0 | Parallelization |
| **Eigen3** | 3.3 | Linear algebra |
| **octrees-benchmark** | Latest | Core algorithms & types |

### Optional (Auto-Detected)

| Package | Purpose | Install |
|---------|---------|---------|
| **PAPI** | Hardware counters | `./scripts/install_papi.sh` |
| **PCL** | Point Cloud Library | `./scripts/install_pcl.sh` |
| **PicoTree** | Alternative spatial index | `./scripts/install_picotree.sh` |

---

