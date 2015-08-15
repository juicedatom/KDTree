template <size_t D, typename ele>
class KDNode {
    public:
        KDNode(std::vector<double> _vec, ele _ele);
        KDNode<D, ele> getRight();
        KDNode<D, ele> getLeft();
        KDNode<D, ele> getElement();
        double atDim(int dim);
        size_t getDim(){ return D; }
        std::shared_ptr<KDNode<D, ele>> _left;
        std::shared_ptr<KDNode<D, ele>> _right;

    private:
        ele _ele;
        std::vector<double> _vec;
};

template <size_t D, typename ele>
KDNode<D, ele>::KDNode(std::vector<double> _vec, ele _ele) {
    this->_ele = _ele;
    this->_vec = _vec;;
}


template <size_t D, typename ele>
double KDNode<D, ele>::atDim(int dim) {
    return this->_vec[dim];
}

template <size_t D, typename ele>
std::ostream& operator<<(std::ostream &strm, const KDNode<D, ele> &node) {
    for (size_t i = 0; i < node->getDim(); i++) {
        strm << node->atDim(i) << " "; 
    }
    return strm << "hi";
}
