template <size_t D, typename ele>
class KDNode {
    public:
        KDNode(std::vector<double> _vec, ele _ele, unsigned int _sortedDim);
        KDNode<D, ele> getRight();
        KDNode<D, ele> getLeft();
        KDNode<D, ele> getElement();
        double atDim(int dim);
        size_t getDim(){ return D; }
        unsigned int getSortedDim(){ return _sortedDim; }
        double atSortedDim();
        std::shared_ptr<KDNode<D, ele>> _left;
        std::shared_ptr<KDNode<D, ele>> _right;
        void printNode();
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
