#include <boost/serialization/list.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/version.hpp>
#include <boost/serialization/split_member.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/vector.hpp>

#include "Point.hpp"

template <size_t D, typename ele>
class KDNode {
    public:
        KDNode(Point<D, ele> p, unsigned int _sortedDim);
        KDNode<D, ele>(){};
        KDNode<D, ele> getRight();
        KDNode<D, ele> getLeft();

        double& operator[] (int x) {
            return this->_point[x];
        }

        double atDim(int dim);
        size_t getDim(){ return D; }
        unsigned int getSortedDim(){ return _sortedDim; }
        double atSortedDim();
        boost::shared_ptr<KDNode<D, ele>> _left;
        boost::shared_ptr<KDNode<D, ele>> _right;
        void printNode();

        //Everything serialization related
        friend class boost::serialization::access;

        template<class Archive>
        void serialize(Archive & ar, const unsigned int version)  {
            ar & _point & _left & _right & _sortedDim;
        }

    private:
        unsigned int _sortedDim;
        Point<D, ele> _point;
};


template <size_t D, typename ele>
KDNode<D, ele>::KDNode(Point<D, ele> p, unsigned int _sortedDim) {
    this->_point = p;
    this->_sortedDim = _sortedDim;
}

template <size_t D, typename ele>
double KDNode<D, ele>::atDim(int dim) {
    return this->_point[dim];
}

template <size_t D, typename ele>
double KDNode<D, ele>::atSortedDim() {
    return this->_point[this->_sortedDim];
}

template <size_t D, typename ele>
void KDNode<D, ele>::printNode() {
    for( int i=0; i<D; i++ ) {
        std::cout<<this->atDim(i)<<" ";
    }
    std::cout<<std::endl;
}
