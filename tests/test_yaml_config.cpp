#include <cstdio>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

#include <gtest/gtest.h>

#include "benchmarking/yaml_config.hpp"
#include "main_options.hpp"

namespace fs = std::filesystem;

// ---------- helpers ----------

/// Reset mainOptions to a fresh default-constructed state.
static void resetOptions() { mainOptions = main_options{}; }

/// Write a temporary YAML file and return its path.
/// The file is deleted automatically via RAII (caller removes after use).
static fs::path writeTempYaml(const std::string& content) {
    fs::path p = fs::temp_directory_path() / "test_yaml_cfg_XXXXXX.yaml";
    // Use unique name
    static int counter = 0;
    p = fs::temp_directory_path() / ("test_yaml_cfg_" + std::to_string(counter++) + ".yaml");
    std::ofstream f(p);
    f << content;
    f.close();
    return p;
}

/// Build a fake argv from a vector of strings and call processArgs.
static void callProcessArgs(std::vector<std::string>& args) {
    // getopt uses global state; reset it
    optind = 1;  
    opterr = 0;  // suppress getopt error messages in tests

    std::vector<char*> argv;
    // argv[0] is the program name
    std::string progName = "test";
    argv.push_back(progName.data());
    for (auto& a : args) argv.push_back(a.data());
    argv.push_back(nullptr);

    processArgs(static_cast<int>(argv.size() - 1), argv.data());
}

// ---------- tests ----------

/// Full benchmark config: every field set, compare YAML vs CLI equivalence.
TEST(YamlConfig, FullConfigMatchesCLI) {
    // ---- 1. Load via YAML ----
    const std::string yaml = R"(
input:
  file: "/tmp/fake_input.las"

output:
  directory: "/tmp/yaml_out"

container:
  type: "SoA"

benchmark:
  radii: [1.5, 3.0, 6.0]
  k_values: [5, 25, 100]
  num_searches: 500
  repeats: 3
  kernels:
    - "sphere"
    - "cube"

algorithms:
  - "neighbors"
  - "neighborsPrune"
  - "neighborsNanoflann"

encodings:
  - "mort"
  - "hilb"

threading:
  num_threads: [1, 4, 8]

advanced:
  build_enc_benchmarks: true
  locality_benchmarks: true
  cache_profiling: true
  check_results: true
  warmup: false
  sequential_searches: true
  approx_tolerances: [10.0, 25.0]
  max_points_leaf: 64
  pcl_octree_resolution: 0.25
  debug: true
)";

    auto yamlPath = writeTempYaml(yaml);
    resetOptions();
    loadConfigFromYaml(yamlPath);
    fs::remove(yamlPath);

    // Snapshot YAML-loaded state
    auto yOpts = mainOptions;

    // ---- 2. Load via CLI ----
    resetOptions();
    std::vector<std::string> cliArgs = {
        "--input", "/tmp/fake_input.las",
        "--output", "/tmp/yaml_out",
        "--container-type", "SoA",
        "--radii", "1.5,3.0,6.0",
        "--kvalues", "5,25,100",
        "--searches", "500",
        "--repeats", "3",
        "--kernels", "sphere,cube",
        "--search-algos", "neighbors,neighborsPrune,neighborsNanoflann",
        "--encodings", "mort,hilb",
        "--num-threads", "1,4,8",
        "--build-enc",
        "--locality",
        "--cache-profiling",
        "--check",
        "--no-warmup",
        "--sequential",
        "--approx-tol", "10.0,25.0",
        "--max-leaf", "64",
        "--pcl-oct-resolution", "0.25",
        "--debug",
    };
    callProcessArgs(cliArgs);
    auto cOpts = mainOptions;

    // ---- 3. Compare field by field ----

    // I/O
    EXPECT_EQ(yOpts.inputFile, cOpts.inputFile);
    EXPECT_EQ(yOpts.inputFileName, cOpts.inputFileName);
    EXPECT_EQ(yOpts.outputDirName, cOpts.outputDirName);

    // Container
    EXPECT_EQ(yOpts.containerType, cOpts.containerType);

    // Benchmark params
    EXPECT_EQ(yOpts.benchmarkRadii, cOpts.benchmarkRadii);
    EXPECT_EQ(yOpts.benchmarkKValues, cOpts.benchmarkKValues);
    EXPECT_EQ(yOpts.numSearches, cOpts.numSearches);
    EXPECT_EQ(yOpts.repeats, cOpts.repeats);

    // Kernels
    EXPECT_EQ(yOpts.kernels, cOpts.kernels);

    // Algorithms & derived structures
    EXPECT_EQ(yOpts.searchAlgos, cOpts.searchAlgos);
    EXPECT_EQ(yOpts.searchStructures, cOpts.searchStructures);

    // Encodings
    EXPECT_EQ(yOpts.encodings, cOpts.encodings);

    // Threading
    EXPECT_EQ(yOpts.numThreads, cOpts.numThreads);

    // Flags
    EXPECT_EQ(yOpts.debug, cOpts.debug);
    EXPECT_EQ(yOpts.buildEncBenchmarks, cOpts.buildEncBenchmarks);
    EXPECT_EQ(yOpts.localityBenchmarks, cOpts.localityBenchmarks);
    EXPECT_EQ(yOpts.cacheProfiling, cOpts.cacheProfiling);
    EXPECT_EQ(yOpts.checkResults, cOpts.checkResults);
    EXPECT_EQ(yOpts.useWarmup, cOpts.useWarmup);
    EXPECT_EQ(yOpts.sequentialSearches, cOpts.sequentialSearches);

    // Advanced
    EXPECT_EQ(yOpts.approximateTolerances, cOpts.approximateTolerances);
    EXPECT_EQ(yOpts.maxPointsLeaf, cOpts.maxPointsLeaf);
    EXPECT_DOUBLE_EQ(yOpts.pclOctResolution, cOpts.pclOctResolution);
}


/// Minimal config: only input, everything else stays at defaults.
TEST(YamlConfig, MinimalConfigMatchesCLI) {
    const std::string yaml = R"(
input:
  file: "/tmp/minimal.las"
)";

    auto yamlPath = writeTempYaml(yaml);
    resetOptions();
    loadConfigFromYaml(yamlPath);
    fs::remove(yamlPath);
    auto yOpts = mainOptions;

    resetOptions();
    std::vector<std::string> cliArgs = {"--input", "/tmp/minimal.las"};
    callProcessArgs(cliArgs);
    auto cOpts = mainOptions;

    EXPECT_EQ(yOpts.inputFile, cOpts.inputFile);
    EXPECT_EQ(yOpts.inputFileName, cOpts.inputFileName);

    // Everything else should be default
    EXPECT_EQ(yOpts.benchmarkRadii, cOpts.benchmarkRadii);
    EXPECT_EQ(yOpts.benchmarkKValues, cOpts.benchmarkKValues);
    EXPECT_EQ(yOpts.numSearches, cOpts.numSearches);
    EXPECT_EQ(yOpts.repeats, cOpts.repeats);
    EXPECT_EQ(yOpts.kernels, cOpts.kernels);
    EXPECT_EQ(yOpts.searchAlgos, cOpts.searchAlgos);
    EXPECT_EQ(yOpts.encodings, cOpts.encodings);
    EXPECT_EQ(yOpts.useWarmup, cOpts.useWarmup);
    EXPECT_EQ(yOpts.debug, cOpts.debug);
    EXPECT_EQ(yOpts.containerType, cOpts.containerType);
}


/// num_searches: "all" in YAML vs -s all in CLI.
TEST(YamlConfig, SearchAllMatchesCLI) {
    const std::string yaml = R"(
input:
  file: "/tmp/all.las"
benchmark:
  num_searches: "all"
)";

    auto yamlPath = writeTempYaml(yaml);
    resetOptions();
    loadConfigFromYaml(yamlPath);
    fs::remove(yamlPath);
    auto yOpts = mainOptions;

    resetOptions();
    std::vector<std::string> cliArgs = {"--input", "/tmp/all.las", "--searches", "all"};
    callProcessArgs(cliArgs);
    auto cOpts = mainOptions;

    EXPECT_EQ(yOpts.searchAll, cOpts.searchAll);
    EXPECT_TRUE(yOpts.searchAll);
    EXPECT_EQ(yOpts.numSearches, cOpts.numSearches);
    EXPECT_EQ(yOpts.numSearches, 0u);
}


/// Memory profiling config.
TEST(YamlConfig, MemoryProfilingMatchesCLI) {
    const std::string yaml = R"(
input:
  file: "/tmp/mem.las"
advanced:
  memory_profiling:
    enabled: true
    structure: "linOct"
)";

    auto yamlPath = writeTempYaml(yaml);
    resetOptions();
    loadConfigFromYaml(yamlPath);
    fs::remove(yamlPath);
    auto yOpts = mainOptions;

    resetOptions();
    std::vector<std::string> cliArgs = {"--input", "/tmp/mem.las", "--memory", "linOct"};
    callProcessArgs(cliArgs);
    auto cOpts = mainOptions;

    ASSERT_TRUE(yOpts.memoryStructure.has_value());
    ASSERT_TRUE(cOpts.memoryStructure.has_value());
    EXPECT_EQ(yOpts.memoryStructure.value(), cOpts.memoryStructure.value());
    EXPECT_EQ(yOpts.memoryStructure.value(), SearchStructure::LINEAR_OCTREE);
}


/// YAML missing a field preserves defaults.
TEST(YamlConfig, MissingFieldsPreserveDefaults) {
    const std::string yaml = R"(
input:
  file: "/tmp/defaults.las"
benchmark:
  radii: [99.0]
)";

    auto yamlPath = writeTempYaml(yaml);
    resetOptions();
    loadConfigFromYaml(yamlPath);
    fs::remove(yamlPath);

    main_options defaults{};

    // radii was overridden
    ASSERT_EQ(mainOptions.benchmarkRadii.size(), 1u);
    EXPECT_FLOAT_EQ(mainOptions.benchmarkRadii[0], 99.0f);

    // everything else should still be default
    EXPECT_EQ(mainOptions.benchmarkKValues, defaults.benchmarkKValues);
    EXPECT_EQ(mainOptions.numSearches, defaults.numSearches);
    EXPECT_EQ(mainOptions.repeats, defaults.repeats);
    EXPECT_EQ(mainOptions.kernels, defaults.kernels);
    EXPECT_EQ(mainOptions.searchAlgos, defaults.searchAlgos);
    EXPECT_EQ(mainOptions.encodings, defaults.encodings);
    EXPECT_EQ(mainOptions.useWarmup, defaults.useWarmup);
    EXPECT_EQ(mainOptions.debug, defaults.debug);
    EXPECT_EQ(mainOptions.containerType, defaults.containerType);
    EXPECT_EQ(mainOptions.maxPointsLeaf, defaults.maxPointsLeaf);
}


/// Nonexistent config file throws.
TEST(YamlConfig, NonexistentFileThrows) {
    EXPECT_THROW(loadConfigFromYaml("/tmp/does_not_exist_42.yaml"), std::runtime_error);
}
