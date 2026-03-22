#pragma once

#include <iostream>
#include <iomanip>
#include <string>

// Use the library's pure data struct
#include <build_log.hpp>

#include "main_options.hpp"

/// @brief Write BuildLog data to CSV (benchmark-specific, accesses mainOptions)
inline void buildLogToCSV(const BuildLog& log, std::ostream& out) {
    out  << containerTypeToString(mainOptions.containerType) << ","
         << searchStructureToString(log.structure) << ","
         << log.maxLeafPoints << ","
         << encoderTypeToString(log.encoding) << ","
         << log.cloudSize << ","
         << log.linearOctreeLeafTime << ","
         << log.linearOctreeInternalTime << ","
         << log.buildTime << ","
         << log.memoryUsed << ","
         << log.totalNodes << ","
         << log.leafNodes << ","
         << log.internalNodes << ","
         << log.maxDepthSeen << ","
         << log.minRadiusAtMaxDepth << ","
         << mainOptions.repeats << ","
         << mainOptions.useWarmup << ","
         << log.l1dMisses << ","
         << log.l2dMisses << ","
         << log.l3Misses << "\n"; 
}

inline void writeBuildLogCSVHeader(std::ostream& out) {
    out  << "container,"
         << "structure,"
         << "max_leaf_points,"
         << "enc_type,"
         << "cloud_size,"
         << "linear_octree_leaf_time,"
         << "linear_octree_internal_time,"
         << "build_time,"
         << "memory_used,"
         << "number_of_nodes,"
         << "leaf_nodes,"
         << "internal_nodes,"
         << "max_depth_seen,"
         << "min_radii_seen,"
         << "repeats,"
         << "used_warmup,"
         << "l1d_miss,"
         << "l2d_miss,"
         << "l3_miss\n";
}

inline std::ostream& operator<<(std::ostream& os, const BuildLog& log) {
    std::string memoryUsedStr = std::to_string(log.memoryUsed / (1024.0 * 1024)) + " MB";
    os << "Encoding and octree construction log:\n";
    os << std::left << std::setw(32) << "Point type:" << "Point" << "\n";
    os << std::left << std::setw(32) << "Structure:" << searchStructureToString(log.structure) << "\n";
    os << std::left << std::setw(32) << "Max. points in leaf:" << log.maxLeafPoints << "\n";
    os << std::left << std::setw(32) << "Encoder type:" << encoderTypeToString(log.encoding) << "\n";
    os << std::left << std::setw(32) << "Cloud size:" << log.cloudSize << "\n";
    if(log.structure == SearchStructure::LINEAR_OCTREE) {
        os << std::left << std::setw(32) << "Linear octree leaves time:" << log.linearOctreeLeafTime << " sec\n";
        os << std::left << std::setw(32) << "Linear octree internal time:" << log.linearOctreeInternalTime << " sec\n";
    }
    os << std::left << std::setw(32) << "Total build time:" << log.buildTime << " sec\n\n";
    os << std::left << std::setw(32) << "Memory used:" << memoryUsedStr << "\n";
    os << std::left << std::setw(32) << "Number of nodes:" << log.totalNodes << "\n";
    os << std::left << std::setw(32) << "  Leafs:" << log.leafNodes << "\n";
    os << std::left << std::setw(32) << "  Internal nodes:" << log.internalNodes << "\n";
    os << std::left << std::setw(32) << "Max. depth seen:" << log.maxDepthSeen << "\n";
    os << std::left << std::setw(32) << "Min. radii seen:" << log.minRadiusAtMaxDepth << "\n";
    if(mainOptions.cacheProfiling) {
        os << std::left << std::setw(32) << "L1d cache misses:" << log.l1dMisses << "\n";
        os << std::left << std::setw(32) << "L2d cache misses:" << log.l2dMisses << "\n";
        os << std::left << std::setw(32) << "L3 cache misses:" << log.l3Misses << "\n";
    }
    os << "\n";
    return os;
}
