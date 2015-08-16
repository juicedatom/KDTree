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

template <size_t D, typename ele>
class KDTree {
    public:
        //This one is for given points
        KDTree(std::vector< Point<D, ele> >);

        //This one is for active learning
        KDTree();

        //TODO DESTRUCTOR
        ~KDTree(){}

        void sayhi();

        size_t dim();
        size_t size();
        bool isEmpty();

        void insert(Point<D, ele> point);
        void write(std::string fname);
        void read(std::string fname);

    private:
        boost::shared_ptr<KDNode<D, ele>> _head;
        boost::shared_ptr<KDNode<D, ele>> buildTree(std::vector< Point<D, ele> > points, int depth);
        void traverse(boost::shared_ptr<KDNode<D, ele>>, std::string gg);
        friend class boost::serialization::access;

        template<class Archive>
        void serialize(
                Archive & ar,
                const unsigned int version
                ) const {
            ar & _head;
        }
};

template <size_t D, typename ele>
struct CompareByDim {
    CompareByDim(int d) {
        this-> d = d;
    }

    bool operator () (Point<D, ele> i, Point<D, ele> j) {
        return i[d] < j[d];
    }

    int d;
};

template <size_t D, typename ele>
std::vector< Point<D, ele> > sortByDim(std::vector<Point<D, ele> > points, int d) {
    std::sort(points.begin(), points.end(), CompareByDim<D, ele>(d));
    return points;
}

template <size_t D, typename ele>
KDTree<D, ele>::KDTree() {}

template <size_t D, typename ele>
KDTree<D, ele>::KDTree(std::vector<Point<D, ele>> points) {
    boost::shared_ptr<KDNode<D, ele>> _head = buildTree(points, 0);
    this->_head = std::move(_head);
}

//TODO should i look at the last few nodes?
template <size_t D, typename ele>
boost::shared_ptr<KDNode<D, ele>> KDTree<D, ele>::buildTree(std::vector<Point<D, ele>> points, int depth) {
    if (points.size() < 1) {
        return NULL;
    }

    int axis = depth % D;
    points = sortByDim(points, axis);
    int median = points.size() / 2;
    boost::shared_ptr<KDNode<D, ele>> node(new KDNode<D, ele>(points[median], axis));

    std::vector<Point<D, ele>> _left(points.begin(), points.begin() + median);
    std::vector<Point<D, ele>> _right(points.begin() + median + 1, points.end());

    depth++;
    node->_left = buildTree(_left, depth);
    node->_right = buildTree(_right, depth);
    return node;
}

template <size_t D, typename ele>
void KDTree<D, ele>::insert(Point<D, ele> p) {
    //this is a thing
    int dim = 0;
    int left = 0;
    boost::shared_ptr<KDNode<D, ele> > cur = _head;
    boost::shared_ptr<KDNode<D, ele> > last = NULL;
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
    boost::shared_ptr<KDNode<D, ele> > tmp(new KDNode<D, ele>(p, (dim-1)%D));
    tmp->printNode();
    if (left) {
        last->_left = tmp;
    } else {
        last->_right = tmp;
    }
}

template <size_t D, typename ele>
void KDTree<D, ele>::sayhi() {
    this->traverse(this->_head, "");
}

template <size_t D, typename ele>
void KDTree<D, ele>::traverse(boost::shared_ptr<KDNode<D, ele> > cur, std::string loc) {
    if (cur == NULL) {
        return;
    }
    cur->printNode();
    std::cout<<loc<<std::endl;
    this->traverse(cur->_left, loc + "l ");
    this->traverse(cur->_right, loc + "r ");
}

template <size_t D, typename ele>
size_t KDTree<D, ele>::dim() {
    return D;
}

template <size_t D, typename ele>
void KDTree<D, ele>::write(std::string fname) {
    std::ofstream ofs(fname);
    boost::archive::binary_oarchive oa(ofs);
    oa & this->_head;
}

template<size_t D, typename ele>
void KDTree<D, ele>::read(std::string fname) {
    std::ifstream ifs(fname);
    boost::archive::binary_iarchive ia(ifs);
    ia & this->_head;
}
#endif
