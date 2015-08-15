template <size_t D, typename ele>
class KDNode {
    public:
        KDNode(std::vector<double> _vec, ele _ele);
        KDNode<D, ele> getRight();
        KDNode<D, ele> getLeft();
        KDNode<D, ele> getElement();
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
