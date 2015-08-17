#include "KDNode.hpp"

template <size_t D, typename V, typename E>
struct PtPqHolder {
    V dist;
    Point<D, V, E> node;
};

template <size_t D, typename V, typename E>
struct PtPqComp {
    bool operator()(const PtPqHolder& lhs, const PtPqHolder& rhs) {
        return lhs.dist < rhs.dist;
    }
}
