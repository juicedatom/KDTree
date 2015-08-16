#include <boost/serialization/list.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/version.hpp>
#include <boost/serialization/split_member.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/vector.hpp>

template <size_t D, typename ele>
class KDNode {
    public:
        KDNode(std::vector<double> _vec, ele _ele, unsigned int _sortedDim);
        KDNode<D, ele>(){};
        KDNode<D, ele> getRight();
        KDNode<D, ele> getLeft();
        KDNode<D, ele> getElement();
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
            ar & _vec & _ele & _left & _right & _sortedDim;
        }

    private:
        ele _ele;
        std::vector<double> _vec;
        unsigned int _sortedDim;
};

template <size_t D, typename ele>
KDNode<D, ele>::KDNode(std::vector<double> _vec, ele _ele, unsigned int _sortedDim) {
    this->_ele = _ele;
    this->_vec = _vec;
    this->_sortedDim = _sortedDim;
}

template <size_t D, typename ele>
double KDNode<D, ele>::atDim(int dim) {
    return this->_vec[dim];
}

template <size_t D, typename ele>
double KDNode<D, ele>::atSortedDim() {
    return this->_vec[this->_sortedDim];
}

template <size_t D, typename ele>
void KDNode<D, ele>::printNode() {
    for( int i=0; i<D; i++ ) {
        std::cout<<this->atDim(i)<<" ";
    }
    std::cout<<std::endl;
}
