#pragma once

#include <filesystem>
#include <string>

#include "main_options.hpp"

/// @brief Load benchmark configuration from a YAML file into mainOptions.
/// Populates all fields exactly as if the equivalent CLI arguments had been passed.
/// Fields not present in the YAML file retain their default values.
/// @param configPath Path to the YAML configuration file.
void loadConfigFromYaml(const std::filesystem::path& configPath);
