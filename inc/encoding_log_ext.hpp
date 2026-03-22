#pragma once

#include <string>
#include <iostream>
#include <iomanip>

// Use the library's pure data struct
#include <encoding_log.hpp>

#include "main_options.hpp"

inline void encodingLogToCSV(const EncodingLog& log, std::ostream& out) {
    out << containerTypeToString(mainOptions.containerType) << ","
        << encoderTypeToString(log.encoding) << ","
        << log.cloudSize << ","
        << log.boundingBoxTime << ","
        << log.encodingTime << ","
        << log.sortingTime << ","
        << mainOptions.repeats << ","
        << mainOptions.useWarmup << "\n";
}

inline void writeEncodingLogCSVHeader(std::ostream& out) {
    out  << "container,"
         << "enc_type,"
         << "cloud_size,"
         << "bbox_time,"
         << "enc_time,"
         << "sort_time,"
         << "repeats,"
         << "used_warmup\n";
}

inline std::ostream& operator<<(std::ostream& os, const EncodingLog& log) {
    os << "Encoding log:\n";
    os << std::left << std::setw(32) << "Point type:" << "Point" << "\n";
    os << std::left << std::setw(32) << "Cloud size:" << log.cloudSize << "\n";
    os << std::left << std::setw(32) << "Encoder type:" << encoderTypeToString(log.encoding) << "\n";
    os << std::left << std::setw(32) << "Find bbox. time:" << log.boundingBoxTime << " sec\n";
    os << std::left << std::setw(32) << "Encoding time:" << log.encodingTime << " sec\n";
    os << std::left << std::setw(32) << "Sorting time:" << log.sortingTime << " sec\n";
    return os;
}
