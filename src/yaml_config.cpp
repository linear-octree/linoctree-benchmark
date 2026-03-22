#include "benchmarking/yaml_config.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include <yaml-cpp/yaml.h>

#include "encoding/point_encoder_factory.hpp"
#include "kernels/kernel_factory.hpp"
#include "main_options.hpp"

// Reuse parsing functions declared in main_options.hpp
// parseKernelOptions, parseSearchAlgoOptions, parseEncodingOptions

namespace {

/// @brief Collect a YAML sequence (or scalar) node into a comma-separated string
/// so that the existing parse*Options helpers can consume it unchanged.
std::string yamlNodeToCommaSeparated(const YAML::Node& node) {
    if (node.IsScalar()) return node.as<std::string>();

    std::ostringstream oss;
    for (std::size_t i = 0; i < node.size(); ++i) {
        if (i > 0) oss << ',';
        oss << node[i].as<std::string>();
    }
    return oss.str();
}

} // anonymous namespace


void loadConfigFromYaml(const std::filesystem::path& configPath) {
    if (!std::filesystem::exists(configPath)) {
        throw std::runtime_error("Config file not found: " + configPath.string());
    }

    YAML::Node config = YAML::LoadFile(configPath.string());

    // --- input ---
    if (auto n = config["input"]) {
        if (n["file"]) {
            mainOptions.inputFile = std::filesystem::path(n["file"].as<std::string>());
            mainOptions.inputFileName = mainOptions.inputFile.stem().string();
        }
    }

    // --- output ---
    if (auto n = config["output"]) {
        if (n["directory"])
            mainOptions.outputDirName = std::filesystem::path(n["directory"].as<std::string>());
    }

    // --- container ---
    if (auto n = config["container"]) {
        if (n["type"]) {
            std::string ct = n["type"].as<std::string>();
            std::transform(ct.begin(), ct.end(), ct.begin(),
                           [](unsigned char c) { return std::tolower(c); });
            if (ct == "aos")
                mainOptions.containerType = ContainerType::AoS;
            else if (ct == "soa")
                mainOptions.containerType = ContainerType::SoA;
            else
                throw std::runtime_error("Invalid container type in config: " + ct + ". Use 'AoS' or 'SoA'.");
        }
    }

    // --- benchmark ---
    if (auto bench = config["benchmark"]) {
        if (bench["radii"]) {
            mainOptions.benchmarkRadii.clear();
            for (const auto& v : bench["radii"])
                mainOptions.benchmarkRadii.push_back(v.as<float>());
        }

        if (bench["k_values"]) {
            mainOptions.benchmarkKValues.clear();
            for (const auto& v : bench["k_values"])
                mainOptions.benchmarkKValues.push_back(v.as<size_t>());
        }

        if (bench["num_searches"]) {
            auto val = bench["num_searches"].as<std::string>();
            if (val == "all") {
                mainOptions.searchAll = true;
                mainOptions.numSearches = 0;
            } else {
                mainOptions.numSearches = std::stoul(val);
            }
        }

        if (bench["repeats"])
            mainOptions.repeats = bench["repeats"].as<size_t>();

        if (bench["kernels"]) {
            std::string kernelStr = yamlNodeToCommaSeparated(bench["kernels"]);
            mainOptions.kernels = parseKernelOptions(kernelStr);
        }
    }

    // --- algorithms ---
    if (config["algorithms"]) {
        std::string algoStr = yamlNodeToCommaSeparated(config["algorithms"]);
        mainOptions.searchAlgos = parseSearchAlgoOptions(algoStr);
        // Derive structures from algorithms, same as CLI path
        mainOptions.searchStructures.clear();
        for (SearchAlgo algo : mainOptions.searchAlgos) {
            mainOptions.searchStructures.insert(algoToStructure(algo));
        }
    }

    // --- encodings ---
    if (config["encodings"]) {
        std::string encStr = yamlNodeToCommaSeparated(config["encodings"]);
        mainOptions.encodings = parseEncodingOptions(encStr);
    }

    // --- threading ---
    if (auto th = config["threading"]) {
        if (th["num_threads"]) {
            mainOptions.numThreads.clear();
            for (const auto& v : th["num_threads"])
                mainOptions.numThreads.push_back(v.as<int>());
        }
    }

    // --- advanced ---
    if (auto adv = config["advanced"]) {
        if (adv["build_enc_benchmarks"])
            mainOptions.buildEncBenchmarks = adv["build_enc_benchmarks"].as<bool>();

        if (auto mem = adv["memory_profiling"]) {
            bool enabled = mem["enabled"] && mem["enabled"].as<bool>();
            if (enabled && mem["structure"]) {
                std::string structStr = mem["structure"].as<std::string>();
                // structureFromString is defined in main_options.cpp; reuse the structureMap
                for (const auto& [key, val] : structureMap) {
                    if (val == structStr) {
                        mainOptions.memoryStructure.emplace(key);
                        break;
                    }
                }
            }
        }

        if (adv["locality_benchmarks"])
            mainOptions.localityBenchmarks = adv["locality_benchmarks"].as<bool>();

        if (adv["cache_profiling"])
            mainOptions.cacheProfiling = adv["cache_profiling"].as<bool>();
        // use_papi is an alias for cache_profiling
        if (adv["use_papi"])
            mainOptions.cacheProfiling = adv["use_papi"].as<bool>();

        if (adv["check_results"])
            mainOptions.checkResults = adv["check_results"].as<bool>();

        if (adv["warmup"])
            mainOptions.useWarmup = adv["warmup"].as<bool>();

        if (adv["sequential_searches"])
            mainOptions.sequentialSearches = adv["sequential_searches"].as<bool>();

        if (adv["search_all"]) {
            bool all = adv["search_all"].as<bool>();
            if (all) {
                mainOptions.searchAll = true;
                mainOptions.numSearches = 0;
            }
        }

        if (adv["approx_tolerances"]) {
            mainOptions.approximateTolerances.clear();
            for (const auto& v : adv["approx_tolerances"])
                mainOptions.approximateTolerances.push_back(v.as<double>());
        }

        if (adv["max_points_leaf"])
            mainOptions.maxPointsLeaf = adv["max_points_leaf"].as<size_t>();

        if (adv["pcl_octree_resolution"])
            mainOptions.pclOctResolution = adv["pcl_octree_resolution"].as<double>();

        if (adv["debug"])
            mainOptions.debug = adv["debug"].as<bool>();
    }
}
