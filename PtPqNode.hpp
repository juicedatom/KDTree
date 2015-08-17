#ifndef PTPQNODE_H
#define PTPQNODE_H

#include "KDNode.hpp"

template <size_t D, typename V, typename E>
class PtPqNode {
    private:
        V _dist;
        Point<D, V, E> _point;
    public:
        PtPqNode(Point<D, V, E> pt, V dist) {
            this->_dist = dist;
            this->_point = pt;
        }
    friend bool operator < (const PtPqNode<D, V, E>& lhs, const PtPqNode<D, V, E>& rhs) {
        return lhs._dist < rhs._dist;
    }
};

#endif
