#ifndef KDTREE
#define KDTREE

#include <vector>
#include <algorithm>
#include <memory>
#include <string>
#include <cmath>
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
        Point<D, V, E> nnSearch(Point<D, V, E> p);

    private:
        boost::shared_ptr<KDNode<D,V,E>> _head;
        boost::shared_ptr<KDNode<D,V,E>> buildTree(std::vector< Point<D,V,E>> points, int depth);
        Point<D, V, E> nnTraverse(boost::shared_ptr<KDNode<D, V, E>> cur, Point<D, V, E> p, V best, Point<D, V, E> ret, int level);
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
    while(1) {

        if (cur == NULL) {
            break;
        }
        
        last = cur;
        if (p[dim] < cur->atDim(dim)) {
            cur = cur->_left;
            left = 1;
        } else {
            cur = cur->_right;
            left = 0;
        }
        dim = (dim + 1) % D;
    }
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
Point<D, V, E> KDTree<D, V, E>::nnTraverse(boost::shared_ptr<KDNode<D, V, E>> cur, Point<D, V, E> p, V best, Point<D, V, E> ret, int level) {
    std::cout<<"hellomynameiselderprice"<<std::endl;
    if (cur == NULL) {
        return ret;
    }

    V distance = cur->getPoint().eucl(p);

    if (distance < best) {
        best = distance;
        ret = cur->getPoint();
    }

    V axis = cur->getSortedDim(); 
    bool left = true;
    if (p[axis] < cur->atDim(axis)) {
        ret = nnTraverse(cur->_left, p, best, ret, level++);
    } else {
        ret = nnTraverse(cur->_right, p, best, ret, level++);
        left = false;
    }

    best = p.eucl(ret);

    if (std::abs(cur->getPoint()[axis] - p[axis]) < best) {
        if (left) {
            ret = nnTraverse(cur->_right, p, best, ret, level++);
        } else {
            ret = nnTraverse(cur->_left, p, best, ret, level++);
        }
    }
    return ret;
}

template <size_t D, typename V, typename E>
Point<D, V, E> KDTree<D, V, E>::nnSearch(Point<D, V, E> p) {
    boost::shared_ptr<KDNode<D, V, E>> cur = this->_head;
    V best = p.eucl(cur->getPoint());
    return this->nnTraverse(cur, p, best, cur->getPoint(), 0);
}

template <size_t D, typename V, typename E>
void KDTree<D, V, E>::read(std::string fname) {
    std::ifstream ifs(fname);
    boost::archive::binary_iarchive ia(ifs);
    ia & this->_head;
}
#endif
