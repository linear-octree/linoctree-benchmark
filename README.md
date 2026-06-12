# octrees-benchmark-suite

[![Ubuntu 22.04](https://img.shields.io/github/actions/workflow/status/linear-octree/linoctree-benchmark/build-and-test.yml?branch=master&job=build-and-test%20(ubuntu-22.04)&label=ubuntu-22.04)](https://github.com/linear-octree/linoctree-benchmark/actions/workflows/build-and-test.yml)
[![Ubuntu 24.04](https://img.shields.io/github/actions/workflow/status/linear-octree/linoctree-benchmark/build-and-test.yml?branch=master&job=build-and-test%20(ubuntu-24.04)&label=ubuntu-24.04)](https://github.com/linear-octree/linoctree-benchmark/actions/workflows/build-and-test.yml)
[![Arch Linux](https://img.shields.io/github/actions/workflow/status/linear-octree/linoctree-benchmark/build-and-test.yml?branch=master&job=build-and-test%20(arch)&label=arch)](https://github.com/linear-octree/linoctree-benchmark/actions/workflows/build-and-test.yml)

A comprehensive **benchmarking and testing suite** for the [**linear-octree**](https://github.com/myermo/linear-octree) spatial data structure library, offering detailed performance profiling across multiple algorithms, encoding schemes, and hardware configurations.

**Linear-Octree Library**: https://github.com/linear-octree/linear-octree

## Overview

This repository provides a **standalone benchmarking client** for the [linear-octree library](https://github.com/myermo/linear-octree), enabling systematic evaluation of octree-based spatial search algorithms. The suite measures:

- **Neighbor search performance** - k-NN and fixed-radius range search throughput
- **Spatial encoding costs** - Hilbert/Morton SFC reordering and structure build times
- **Cache locality** - Index-distance histograms and SIMD efficiency
- **Memory footprint** - Theoretical plus empirical heap profiling
- **Thread scalability** - OpenMP parallelization efficiency across core counts
- **Hardware behavior** - PAPI performance counter integration

### Key Features

- **Linear-Octree Integration** - Built on https://github.com/linear-octree/linear-octree (implicit, linearized spatial index)  
- **Multi-Structure Benchmarking** - LinearOctree, PointerOctree, Nanoflann KD-Tree, Picotree, UNIBN Octree, PCL variants  
- **Space-Filling Curves** - Hilbert & Morton encodings in 2D/3D; configurable point cloud reordering  
- **Hardware Profiling** - PAPI integration for cache misses, TLB failures, branch mispredictions  
- **OpenMP Scalability** - Dynamic scheduling; thread count sweeps for parallel efficiency analysis  
- **CSV + YAML** - Command-line args or YAML config files; machine-readable output for post-processing  
- **Comprehensive Testing** - Google Test suite validating YAML/CLI equivalence across all parameters

---

## Building from Source

### Prerequisites

- **C++20 compiler** (GCC 10+, Clang 11+)
- **CMake** 3.14+
- **OpenMP** 4.0+ (for parallelization)
- **yaml-cpp** 0.7+ (configuration parsing)
- **PAPI** (performance counter library)
- **LASLIB** (LAS point cloud I/O)
- **PCL** 1.15+ (Point Cloud Library — requires Boost headers and FLANN)
- **Boost** 1.71+ development headers (required by PCL)
- **Picotree** (KD-Tree variant)
- **Eigen3** 3.4+ (required by PCL and Picotree)
- **Linear-Octree Library** - https://github.com/linear-octree/linear-octree (required)

The following library is embedded and requires no separate installation:
- **Nanoflann** (KD-Tree, header-only, included in linear-octree)

### Installation Steps

#### 1. Install Linear-Octree Library

The benchmark suite **requires** the linear-octree library from https://github.com/linear-octree/linear-octree:

```bash
git clone https://github.com/linear-octree/linear-octree.git
cd linear-octree
bash scripts/install_laslib.sh
cmake -B build -DCMAKE_BUILD_TYPE=Release .
cmake --build build --parallel $(nproc)
cmake --install build
```

This installs the library to `$HOME/.local/lib/` and headers to `$HOME/.local/include/linoctree/`.

#### 2. Install Benchmark Suite Dependencies

```bash
cd /path/to/linoctree-benchmark
bash scripts/install_all.sh   # Installs LASlib, PAPI, Picotree, PCL + FLANN
```

> **Note:** `install_all.sh` calls `install_pcl.sh` which requires Boost development headers. On Arch Linux: `sudo pacman -S boost`. On Ubuntu/Debian: `sudo apt install libboost-all-dev`.

#### 3. Build the Benchmark Suite

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release .
cmake --build build --parallel $(nproc)
```

**Optional**: Enable tests with `-DBUILD_TESTS=ON`:
```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTS=ON .
cmake --build build --parallel $(nproc)
ctest --test-dir build/tests --output-on-failure
```

---

## Usage

### Quick Benchmark

```bash
# Neighbor search with default parameters
./build/octrees-benchmark-suite -i /path/to/data.las

# With custom radii, search counts, and output
./build/octrees-benchmark-suite \
  -i data/alcoy.las \
  -o results/ \
  -r "0.5,1.0,2.0" \
  -s 5000 \
  --repeats 5

# Output: CSV file in results/ with timing and throughput
```

### Two Configuration Methods

#### Method 1: Command-Line Arguments (CLI)

```bash
./build/octrees-benchmark-suite \
  --input data/alcoy.las \
  --output out/ \
  --container-type AoS \
  --radii 1.0,2.0,3.0 \
  --kvalues 10,50,100 \
  --searches 5000 \
  --repeats 3 \
  --kernels sphere,cube \
  --search-algos neighbors,neighborsPrune \
  --encodings mort,hilb \
  --num-threads 1,4,8,16 \
  --build-enc \
  --cache-profiling
```

#### Method 2: YAML Configuration File (Recommended for Complex Setups)

Create `config.yaml`:

```yaml
input:
  file: "data/alcoy.las"

output:
  directory: "results/"

container:
  type: "SoA"

benchmark:
  radii: [1.0, 2.0, 5.0]
  k_values: [10, 50, 250]
  num_searches: 5000
  repeats: 3
  kernels: [sphere, cube]

algorithms:
  - "neighbors"
  - "neighborsPrune"
  - "neighborsNanoflann"

encodings:
  - "mort"
  - "hilb"

threading:
  num_threads: [1, 4, 8, 16]

advanced:
  build_enc_benchmarks: true
  cache_profiling: true
  warmup: true
  sequential_searches: false
  max_points_leaf: 128
  pcl_octree_resolution: 0.1
  debug: false
```

Then run:

```bash
./build/octrees-benchmark-suite --config config.yaml
```

**CLI args override YAML values** (args processed left-to-right):

```bash
./build/octrees-benchmark-suite --config config.yaml -i other.las -r 0.5
# Uses YAML config but reads from "other.las" with radii [0.5]
```

### All Command-Line Options

```bash
./build/octrees-benchmark-suite -h
```

Key options:
- `-i, --input` — Input LAS file (required)
- `-o, --output` — Output directory (default: "out")
- `-c, --container-type` — AoS or SoA memory layout (default: AoS)
- `-r, --radii` — Range search radii, comma-separated (default: 2.5,5.0,7.5,10.0)
- `-v, --kvalues` — k-NN k values, comma-separated (default: 10,50,250,1000)
- `-s, --searches` — Number of random search queries (default: 10000; use "all" for exhaustive)
- `-t, --repeats` — Repetitions per benchmark (default: 2)
- `-a, --search-algos` — Algorithms to benchmark (default: neighbor,neighborsPrune,neighborPtr,neighborsStruct)
- `-e, --encodings` — SFC encodings: none, mort, hilb, etc. (default: all)
- `-k, --kernels` — Query shapes: sphere, cube, circle, square (default: all)
- `--num-threads` — Thread counts for scalability testing (default: max available)
- `--build-enc` — Run encoding/build benchmarks instead of search
- `--locality` — Run locality/cache analysis
- `--memory <STRUCT>` — Profile memory of a single structure (e.g., linOct)
- `--cache-profiling` — Enable PAPI hardware counter collection
- `--config <FILE>` — Load parameters from YAML file

### Algorithm/Encoder Tokens

The `--search-algos` flag accepts algorithm tokens for the linear-octree search variants:

- **Linear-octree search algorithms**: `neighbors`, `neighborsPrune`, `neighborPtr`, `neighborsStruct`
- **External structure algorithms** (used internally, selected via `--search-algos`): `neighborsNanoflann`

The `--encodings` and `--kernels` flags accept:

- **Encodings**: `hilb` (Hilbert), `mort` (Morton), `none` (no encoding)
- **Kernels**: `sphere`, `cube`, `circle`, `square`
- **Query types**: `range` (fixed-radius), `knn` (k-nearest neighbours)
---
