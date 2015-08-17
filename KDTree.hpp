#ifndef KDTREE
#define KDTREE

#include <vector>
#include <algorithm>
#include <memory>
#include <string>
#include <cmath>
#include <map>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>

#include "KDNode.hpp"
#include "Point.hpp"

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
        std::unique_ptr<std::multimap<V, Point<D, V, E>>> search(
                Point<D, V, E> p,
                const unsigned int k,
                const bool bbf = false,
                const unsigned int maxlevel = 0);

    private:
        boost::shared_ptr<KDNode<D,V,E>> _head;
        boost::shared_ptr<KDNode<D,V,E>> buildTree(std::vector< Point<D,V,E>> points, int depth);
        std::unique_ptr<std::multimap<V, Point<D, V, E>>> knnTraverse(
                boost::shared_ptr<KDNode<D, V, E>> cur,
                Point<D, V, E> p, std::unique_ptr<std::multimap<V, Point<D, V, E>>> pq,
                const unsigned int k,
                unsigned int level,
                const bool bbf = false,
                const unsigned int maxlevel = 0);

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
std::unique_ptr<std::multimap<V, Point<D, V, E>>> KDTree<D, V, E>::knnTraverse(
        boost::shared_ptr<KDNode<D, V, E>> cur,
        Point<D, V, E> p,
        std::unique_ptr<std::multimap<V, Point<D, V, E>>> pq,
        unsigned int k,
        unsigned int level,
        const bool bbf,
        const unsigned int maxlevel) {

    std::cout<<"hellomynameiselderprice"<<std::endl;

    bool levelmaxReached = (maxlevel !=0) && (level > maxlevel);

    if (levelmaxReached || cur == NULL) {
        return pq;
    }

    V distance = cur->getPoint().eucl(p);
    pq->insert(std::pair<V, Point<D, V, E>>(distance, cur->getPoint()));

    if (pq->size() > k) {
        std::cout<<"erasing!"<<std::endl;
        pq->erase(std::prev(pq->end()));
    }

    V axis = level % D;
    bool left = true;
    if (p[axis] < cur->atDim(axis)) {
        pq = knnTraverse(cur->_left, p, std::move(pq), k, level++, bbf, maxlevel);
    } else {
        pq = knnTraverse(cur->_right, p, std::move(pq), k, level++, bbf, maxlevel);
        left = false;
    }

    if (!bbf) { 
        std::cout<<"check the other tree"<<std::endl;
        V best = pq->begin()->first;
        if (pq->size() < k || std::abs(cur->getPoint()[axis] - p[axis]) < best) {
            if (left) {
                pq = knnTraverse(cur->_right, p,  std::move(pq), k, level++, bbf, maxlevel);
            } else {
                pq = knnTraverse(cur->_left, p, std::move(pq), k, level++, bbf, maxlevel);
            }
        }
    }

   return pq;
}

template <size_t D, typename V, typename E>
std::unique_ptr<std::multimap<V, Point<D, V, E>>> KDTree<D, V, E>::search(
        Point<D, V, E> p,
        unsigned int k,
        const bool bbf,
        const unsigned int maxlevel) {

    boost::shared_ptr<KDNode<D, V, E>> cur = this->_head;
    std::unique_ptr<std::multimap<V, Point<D, V, E>>> pq = std::make_unique<std::multimap<V, Point<D, V, E>>>();
    pq = this->knnTraverse(cur, p, std::move(pq), k, 0, bbf, maxlevel);
    return pq;
}

template <size_t D, typename V, typename E>
void KDTree<D, V, E>::read(std::string fname) {
    std::ifstream ifs(fname);
    boost::archive::binary_iarchive ia(ifs);
    ia & this->_head;
}
#endif
