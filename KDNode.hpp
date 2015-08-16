#include <boost/serialization/list.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/version.hpp>
#include <boost/serialization/split_member.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/vector.hpp>

#include "Point.hpp"

template <size_t D, typename V, typename E>
class KDNode {
    public:
        KDNode(Point<D, V, E> p, unsigned int _sortedDim);
        KDNode<D, V, E>(){};
        KDNode<D, V, E> getRight();
        KDNode<D, V, E> getLeft();

        V& operator[] (int x) {
            return this->_point[x];
        }

        V atDim(int dim);
        size_t getDim(){ return D; }
        unsigned int getSortedDim(){ return _sortedDim; }
        V atSortedDim();
        boost::shared_ptr<KDNode<D, V, E>> _left;
        boost::shared_ptr<KDNode<D, V, E>> _right;
        void printNode();

        //Everything serialization related
        friend class boost::serialization::access;

        template <class Archive>
        void serialize(Archive & ar, const unsigned int version)  {
            ar & _point & _left & _right & _sortedDim;
        }

    private:
        unsigned int _sortedDim;
        Point<D, V, E> _point;
};


template <size_t D, typename V, typename E>
KDNode<D, V, E>::KDNode(Point<D, V, E> p, unsigned int _sortedDim) {
    this->_point = p;
    this->_sortedDim = _sortedDim;
}

template <size_t D, typename V, typename E>
V KDNode<D, V, E>::atDim(int dim) {
    return this->_point[dim];
}

template <size_t D, typename V, typename E>
V KDNode<D, V, E>::atSortedDim() {
    return this->_point[this->_sortedDim];
}

template <size_t D, typename V, typename E>
void KDNode<D, V, E>::printNode() {
    for( int i=0; i<D; i++ ) {
        std::cout<<this->atDim(i)<<" ";
    }
    std::cout<<std::endl;
}
