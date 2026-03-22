#include "structures/octree_logged.hpp"
#include <algorithm>
#include <memory>

template <PointContainer Container>
void LoggedOctree<Container>::logOctreeData(std::shared_ptr<BuildLog> log) const {
    std::vector<std::pair<std::reference_wrapper<const Octree<Container>>, size_t>> toVisit;
    toVisit.emplace_back(std::cref(*this), 0);

    while (!toVisit.empty()) {
        auto [octant_ref, depth] = toVisit.back();
        toVisit.pop_back();
        const Octree<Container>& octant = octant_ref.get();

        // Add size of Octree node itself
        log->memoryUsed += sizeof(Octree<Container>);

        // For leaf nodes, count memory used by indices
        if (octant.isLeaf()) {
            log->leafNodes++;
            const auto& pointIndices = octant.getPoints();
            log->memoryUsed += sizeof(size_t) * pointIndices.size();  // each index is a size_t

            if (depth > log->maxDepthSeen) {
                log->maxDepthSeen = depth;
                auto minRadii = octant.getRadii();
                log->minRadiusAtMaxDepth = std::min({minRadii.getX(), minRadii.getY(), minRadii.getZ()});
            }
        } else {
            log->internalNodes++;
            for (const auto& child : octant.getOctants()) {
                toVisit.emplace_back(std::cref(child), depth + 1);
            }
        }
    }

    log->totalNodes = log->leafNodes + log->internalNodes;
}

// Explicit template instantiation
template class LoggedOctree<PointsAoS>;
template class LoggedOctree<PointsSoA>;
