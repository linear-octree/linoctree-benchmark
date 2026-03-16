# Optional Dependencies Installation Scripts

This directory contains installation scripts for optional benchmarking dependencies. These are NOT required for the core library, but enable additional features in the benchmarking suite.

## Available Dependencies

### 1. PAPI (Performance API)
**File**: `install_papi.sh`

Enables hardware performance counter measurements during benchmarks. Essential if you want to collect CPU event counters (cache misses, instructions, etc.).

**Usage**: `./scripts/install_papi.sh`

**Installation location**: `lib/papi/`

**Time estimate**: ~5-10 minutes

### 2. PCL (Point Cloud Library)
**File**: `install_pcl.sh`

Installs Point Cloud Library which includes KDTree and Octree implementations. These are compiled as comparison algorithms in the benchmarks.

**Note**: This script also installs required dependencies (Boost, Eigen3, FLANN).

**Usage**: `./scripts/install_pcl.sh`

**Installation location**: 
- `lib/boost/`
- `lib/eigen3/`
- `lib/flann/`
- `lib/pcl/`

**Time estimate**: ~30-60 minutes (significant compilation time)

### 3. Picotree
**File**: `install_picotree.sh`

Installs Picotree, another spatial indexing library used for comparison in benchmarks.

**Usage**: `./scripts/install_picotree.sh`

**Installation location**: `lib/picotree/`

**Time estimate**: ~5 minutes

## Installation Methods

### Option 1: Automatic Installation (Recommended)
Run the build script and choose to install dependencies when prompted:

```bash
./build.sh
# When prompted, select which dependencies to install
```

### Option 2: Manual Installation
Install specific dependencies manually:

```bash
# Install all three
./scripts/install_papi.sh
./scripts/install_pcl.sh
./scripts/install_picotree.sh

# Or install individually
./scripts/install_papi.sh   # Just PAPI
./scripts/install_pcl.sh    # Just PCL (+ dependencies)
./scripts/install_picotree.sh  # Just Picotree
```

### Option 3: Skip Installation
If dependencies are not installed, the suite will compile with reduced functionality. Some benchmark algorithms will be disabled.

## About Dependency Isolation

All dependencies are installed to the local `lib/` directory within the suite. This means:
- ✅ No system-wide installation required
- ✅ Projects remain isolated
- ✅ Easy to clean (just `rm -rf lib/`)
- ⚠️  These dependencies are NOT copied to the parent library

## Notes

- PCL installation takes considerable time due to building Boost, Eigen3, and FLANN from source.
- Internet connection required to download source archives.
- Enough disk space needed (~5GB for PCL build artifacts).
- After installing dependencies, the suite must be rebuilt: `./build.sh`

## Troubleshooting

If an installation script fails:

1. Check internet connectivity
2. Ensure sufficient disk space
3. Review the error message
4. Try installing manually with verbose output:
   ```bash
   bash -x ./scripts/install_papi.sh
   ```

## Uninstalling Dependencies

To remove all installed dependencies:

```bash
rm -rf lib/
```

Then rebuild the suite:
```bash
./build.sh
```
