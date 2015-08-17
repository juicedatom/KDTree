#ifndef TESTCLASS_H
#define TESTCLASS_H

template <size_t D, typename V, typename E>

class PtPqNode {
    private:
        V _dist;
        std::unique_ptr<Point<D, V, E>> _point;
    public:
        PtPqNode(Point<D, V, E> pt, V dist) {
            this->_dist = dist;
            this->_point = std::make_unique<Point<D, V, E>>(pt);

    friend bool operator < (const PtPqNode<D, V, E>& rhs) {
        return this->dist < rhs.dist;
    }
};

#endif
