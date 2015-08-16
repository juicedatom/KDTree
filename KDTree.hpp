#ifndef KDTREE
#define KDTREE

#include <vector>
#include <algorithm>
#include <memory>
#include <string>
#include "KDNode.hpp"
#include "Point.hpp"
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>

template <size_t D, typename V, typename E>
class KDTree {
    public:
        //This one is for given points
        KDTree(std::vector< Point<D, V, E> >);

        //This one is for active learning
        KDTree();

        //TODO DESTRUCTOR
        ~KDTree(){}

        void sayhi();

        size_t dim();
        size_t size();
        bool isEmpty();

        void insert(Point<D, V, E> point);
        void write(std::string fname);
        void read(std::string fname);

    private:
        boost::shared_ptr<KDNode<D,V,E>> _head;
        boost::shared_ptr<KDNode<D,V,E>> buildTree(std::vector< Point<D,V,E>> points, int depth);
        void traverse(boost::shared_ptr<KDNode<D, V, E>>, std::string gg);
        friend class boost::serialization::access;

        template<class Archive>
        void serialize(
                Archive & ar,
                const unsigned int version
                ) const {
            ar & _head;
        }
};

template <size_t D, typename V, typename E>
struct CompareByDim {
    CompareByDim(int d) {
        this-> d = d;
    }

    bool operator () (Point<D, V, E> i, Point<D, V, E> j) {
        return i[d] < j[d];
    }

    int d;
};

template <size_t D, typename V, typename E>
std::vector< Point<D, V, E> > sortByDim(std::vector<Point<D, V, E> > points, int d) {
    std::sort(points.begin(), points.end(), CompareByDim<D, V, E>(d));
    return points;
}

template <size_t D, typename V, typename E>
KDTree<D, V, E>::KDTree() {}

template <size_t D, typename V, typename E>
KDTree<D, V, E>::KDTree(std::vector<Point<D, V, E>> points) {
    boost::shared_ptr<KDNode<D, V, E>> _head = buildTree(points, 0);
    this->_head = std::move(_head);
}

//TODO should i look at the last few nodes?
template <size_t D, typename V, typename E>
boost::shared_ptr<KDNode<D, V, E>> KDTree<D, V, E>::buildTree(std::vector<Point<D, V, E>> points, int depth) {
    if (points.size() < 1) {
        return NULL;
    }

    int axis = depth % D;
    points = sortByDim(points, axis);
    int median = points.size() / 2;
    boost::shared_ptr<KDNode<D, V, E>> node(new KDNode<D, V, E>(points[median], axis));

    std::vector<Point<D, V, E>> _left(points.begin(), points.begin() + median);
    std::vector<Point<D, V, E>> _right(points.begin() + median + 1, points.end());

    depth++;
    node->_left = buildTree(_left, depth);
    node->_right = buildTree(_right, depth);
    return node;
}

template <size_t D, typename V, typename E>
void KDTree<D, V, E>::insert(Point<D, V, E> p) {
    //this is a thing
    int dim = 0;
    int left = 0;
    boost::shared_ptr<KDNode<D, V, E> > cur = _head;
    boost::shared_ptr<KDNode<D, V, E> > last = NULL;
    std::cout<<"hi!"<<std::endl;
    while(1) {

        if (cur == NULL) {
            break;
        }
        
        last = cur;
        std::cout<<"comparing "<<p[dim]<<" and "<<cur->atDim(dim)<<std::endl;
        if (p[dim] < cur->atDim(dim)) {
            cur = cur->_left;
            left = 1;
            std::cout<<"less!"<<std::endl;
        } else {
            cur = cur->_right;
            left = 0;
            std::cout<<"more!"<<std::endl;
        }
        dim = (dim + 1) % D;
    }
    std::cout<<"inserting: "<<std::endl;
    boost::shared_ptr<KDNode<D, V, E> > tmp(new KDNode<D, V, E>(p, (dim-1)%D));
    tmp->printNode();
    if (left) {
        last->_left = tmp;
    } else {
        last->_right = tmp;
    }
}

template <size_t D, typename V, typename E>
void KDTree<D, V, E>::sayhi() {
    this->traverse(this->_head, "");
}

template <size_t D, typename V, typename E>
void KDTree<D, V, E>::traverse(boost::shared_ptr<KDNode<D, V, E> > cur, std::string loc) {
    if (cur == NULL) {
        return;
    }
    cur->printNode();
    std::cout<<loc<<std::endl;
    this->traverse(cur->_left, loc + "l ");
    this->traverse(cur->_right, loc + "r ");
}

template <size_t D, typename V, typename E>
size_t KDTree<D, V, E>::dim() {
    return D;
}

template <size_t D, typename V, typename E>
void KDTree<D, V, E>::write(std::string fname) {
    std::ofstream ofs(fname);
    boost::archive::binary_oarchive oa(ofs);
    oa & this->_head;
}

template <size_t D, typename V, typename E>
void KDTree<D, V, E>::read(std::string fname) {
    std::ifstream ifs(fname);
    boost::archive::binary_iarchive ia(ifs);
    ia & this->_head;
}
#endif
