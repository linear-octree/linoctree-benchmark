#pragma once

#include <structures/octree.hpp>
#include "../build_log_ext.hpp"

/// @brief Octree subclass with benchmarking-specific logging functionality
template <PointContainer Container>
class LoggedOctree : public Octree<Container> {
public:
    using Octree<Container>::Octree;

    /// @brief Fill the missing data in the octree log
    void logOctreeData(std::shared_ptr<BuildLog> log) const;
};
